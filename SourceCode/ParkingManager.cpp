#include "ParkingManager.h"
#include <algorithm>
#include <cmath>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <unordered_set>
#include <utility>

namespace fs = std::filesystem;
namespace {
std::string cleanField(std::string s) {
    std::replace(s.begin(), s.end(), ',', ' ');
    std::replace(s.begin(), s.end(), '\n', ' ');
    std::replace(s.begin(), s.end(), '\r', ' ');
    return s;
}
std::vector<std::string> split(const std::string& line) {
    std::vector<std::string> out; std::stringstream ss(line); std::string item;
    while (std::getline(ss, item, ',')) out.push_back(item);
    return out;
}
std::string money(double amount) {
    std::ostringstream out; out << "Rs. " << std::fixed << std::setprecision(2) << amount; return out.str();
}
}
ParkingManager::ParkingManager(std::string dataDirectory)
    : dataDirectory_(std::move(dataDirectory)), nextSlotId_(1) {
    fs::create_directories(dataDirectory_);
    loadAll();
    if (slots_.empty()) initialiseDefaultSlots();
}
ParkingSlot* ParkingManager::findSlot(int id) {
    for (auto& s : slots_) if (s.id() == id) return &s;
    return nullptr;
}
const ParkingSlot* ParkingManager::findSlot(int id) const {
    for (const auto& s : slots_) if (s.id() == id) return &s;
    return nullptr;
}
ActiveParking* ParkingManager::findActive(const std::string& registration) {
    for (auto& a : active_) if (a.vehicle->registration() == registration) return &a;
    return nullptr;
}
void ParkingManager::initialiseDefaultSlots() {
    for (int i=0;i<8;++i) slots_.emplace_back(nextSlotId_++, "Standard");
    for (int i=0;i<5;++i) slots_.emplace_back(nextSlotId_++, "TwoWheeler");
    for (int i=0;i<2;++i) slots_.emplace_back(nextSlotId_++, "Heavy");
    saveAll();
}
int ParkingManager::parkVehicle(const std::string& type, const std::string& registration,
                                const std::string& owner) {
    if (registration.empty() || owner.empty()) throw std::invalid_argument("Registration and owner are required.");
    if (findActive(registration)) throw std::logic_error("This vehicle is already parked.");
    auto vehicle = makeVehicle(type, registration, owner);
    ParkingSlot* chosen = nullptr;
    for (auto& slot : slots_) {
        if (!slot.occupied() && slot.isCompatible(vehicle->requiredSlotType())) { chosen = &slot; break; }
    }
    if (!chosen) throw std::runtime_error("No compatible slot is available for this vehicle.");
    chosen->park(registration);
    int id = chosen->id();
    active_.push_back({std::move(vehicle), id, std::time(nullptr)});
    saveAll();
    return id;
}
ParkingReceipt ParkingManager::exitVehicle(const std::string& registration, std::time_t exitTime) {
    ActiveParking* active = findActive(registration);
    if (!active) throw std::runtime_error("No active parking record found for this registration.");
    if (exitTime < active->entryTime) throw std::invalid_argument("Exit time cannot be earlier than entry time.");
    const auto entry = active->entryTime;
    const auto type = active->vehicle->type();
    const auto rate = active->vehicle->hourlyRate();
    const auto slotId = active->slotId;
    const auto mins = static_cast<long long>(std::difftime(exitTime, entry) / 60.0);
    const double hours = std::max(1.0, std::ceil(std::difftime(exitTime, entry) / 3600.0));
    ParkingReceipt receipt{registration, type, slotId, mins, hours * rate, entry, exitTime};
    ParkingSlot* slot = findSlot(slotId);
    if (!slot) throw std::logic_error("Parking slot consistency error.");
    slot->release();
    history_.push_back(receipt);
    active_.erase(std::remove_if(active_.begin(), active_.end(),
        [&](const ActiveParking& a) { return a.vehicle->registration() == registration; }), active_.end());
    saveAll();
    return receipt;
}
void ParkingManager::addSlot(const std::string& category) {
    if (category != "Standard" && category != "TwoWheeler" && category != "Heavy")
        throw std::invalid_argument("Category must be Standard, TwoWheeler, or Heavy.");
    slots_.emplace_back(nextSlotId_++, category);
    saveAll();
}
void ParkingManager::showSlots() const {
    std::cout << "\n--- SLOT DIRECTORY ---\n";
    for (const auto& s : slots_)
        std::cout << "Slot " << std::setw(2) << s.id() << " | " << std::setw(10) << s.category()
                  << " | " << (s.occupied() ? "OCCUPIED (" + s.registration() + ")" : "AVAILABLE") << '\n';
}
void ParkingManager::showActiveVehicles() const {
    std::cout << "\n--- ACTIVE PARKING ---\n";
    if (active_.empty()) { std::cout << "No vehicles currently parked.\n"; return; }
    for (const auto& a : active_)
        std::cout << a.vehicle->registration() << " | " << a.vehicle->type()
                  << " | Owner: " << a.vehicle->owner() << " | Slot: " << a.slotId
                  << " | Entered: " << std::put_time(std::localtime(&a.entryTime), "%Y-%m-%d %H:%M:%S") << '\n';
}
void ParkingManager::showOccupancyReport() const {
    int occupied=0, standard=0, standardOcc=0, bikes=0, bikesOcc=0, heavy=0, heavyOcc=0;
    for (const auto& s : slots_) {
        occupied += s.occupied();
        if (s.category()=="Standard") { ++standard; standardOcc += s.occupied(); }
        else if (s.category()=="TwoWheeler") { ++bikes; bikesOcc += s.occupied(); }
        else if (s.category()=="Heavy") { ++heavy; heavyOcc += s.occupied(); }
    }
    auto pct=[](int o,int total){ return total ? 100.0*o/total : 0.0; };
    std::cout << "\n========== OCCUPANCY REPORT ==========\n"
              << "Total slots: " << slots_.size() << " | Occupied: " << occupied
              << " | Available: " << slots_.size()-occupied << " | Occupancy: "
              << std::fixed << std::setprecision(1) << pct(occupied, static_cast<int>(slots_.size())) << "%\n"
              << "Standard:   " << standardOcc << "/" << standard << " occupied\n"
              << "TwoWheeler: " << bikesOcc << "/" << bikes << " occupied\n"
              << "Heavy:      " << heavyOcc << "/" << heavy << " occupied\n"
              << "======================================\n";
}
void ParkingManager::showHistory() const {
    std::cout << "\n--- COMPLETED PARKING / RECEIPTS ---\n";
    if (history_.empty()) { std::cout << "No completed parking records yet.\n"; return; }
    for (const auto& r : history_) {
        std::cout << r.registration << " | " << r.vehicleType << " | Slot " << r.slotId
                  << " | " << r.durationMinutes << " min | " << money(r.fee) << '\n';
    }
}
void ParkingManager::searchVehicle(const std::string& registration) const {
    for (const auto& a : active_) if (a.vehicle->registration()==registration) {
        std::cout << "FOUND: " << registration << " is parked in slot " << a.slotId
                  << " (" << a.vehicle->type() << ", owner " << a.vehicle->owner() << ").\n"; return;
    }
    for (const auto& r : history_) if (r.registration==registration) {
        std::cout << "FOUND IN HISTORY: " << registration << " last completed session used slot "
                  << r.slotId << ", fee " << money(r.fee) << ".\n"; return;
    }
    std::cout << "No record found for " << registration << ".\n";
}
std::size_t ParkingManager::activeCount() const { return active_.size(); }
std::size_t ParkingManager::slotCount() const { return slots_.size(); }

void ParkingManager::saveAll() const {
    fs::create_directories(dataDirectory_);
    std::ofstream sf(dataDirectory_ + "/slots.csv");
    std::ofstream af(dataDirectory_ + "/active.csv");
    std::ofstream hf(dataDirectory_ + "/history.csv");
    if (!sf || !af || !hf) throw std::runtime_error("Unable to write data files.");
    sf << "id,category,occupied,registration\n";
    for (const auto& s : slots_) sf << s.id() << ',' << s.category() << ',' << s.occupied() << ',' << s.registration() << '\n';
    af << "registration,owner,type,slotId,entryTime\n";
    for (const auto& a : active_) af << cleanField(a.vehicle->registration()) << ',' << cleanField(a.vehicle->owner()) << ','
        << a.vehicle->type() << ',' << a.slotId << ',' << static_cast<long long>(a.entryTime) << '\n';
    hf << "registration,type,slotId,durationMinutes,fee,entryTime,exitTime\n";
    for (const auto& r : history_) hf << cleanField(r.registration) << ',' << r.vehicleType << ',' << r.slotId << ','
        << r.durationMinutes << ',' << std::fixed << std::setprecision(2) << r.fee << ','
        << static_cast<long long>(r.entryTime) << ',' << static_cast<long long>(r.exitTime) << '\n';
}
void ParkingManager::loadAll() {
    if (fs::exists(dataDirectory_ + "/slots.csv")) {
        std::ifstream in(dataDirectory_ + "/slots.csv"); std::string line; std::getline(in,line);
        while (std::getline(in,line)) {
            auto f=split(line); if(f.size()<4) continue;
            try {
                int id=std::stoi(f[0]); ParkingSlot slot(id,f[1]);
                if(f[2]=="1") slot.park(f[3]);
                slots_.push_back(slot); nextSlotId_=std::max(nextSlotId_,id+1);
            } catch (...) { /* Skip malformed row safely. */ }
        }
    }
    if (fs::exists(dataDirectory_ + "/active.csv")) {
        std::ifstream in(dataDirectory_ + "/active.csv"); std::string line; std::getline(in,line);
        while (std::getline(in,line)) {
            auto f=split(line); if(f.size()<5) continue;
            try {
                auto v=makeVehicle(f[2],f[0],f[1]); int slot=std::stoi(f[3]); auto entry=static_cast<std::time_t>(std::stoll(f[4]));
                if (findSlot(slot) && findSlot(slot)->occupied() && findSlot(slot)->registration()==f[0])
                    active_.push_back({std::move(v),slot,entry});
            } catch (...) { /* Skip malformed row safely. */ }
        }
    }
    if (fs::exists(dataDirectory_ + "/history.csv")) {
        std::ifstream in(dataDirectory_ + "/history.csv"); std::string line; std::getline(in,line);
        while (std::getline(in,line)) {
            auto f=split(line); if(f.size()<7) continue;
            try { history_.push_back({f[0],f[1],std::stoi(f[2]),std::stoll(f[3]),std::stod(f[4]),
                    static_cast<std::time_t>(std::stoll(f[5])),static_cast<std::time_t>(std::stoll(f[6]))}); }
            catch (...) { /* Skip malformed row safely. */ }
        }
    }
}
