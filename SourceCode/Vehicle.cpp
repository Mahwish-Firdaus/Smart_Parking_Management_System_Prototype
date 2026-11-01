#include "Vehicle.h"
#include <algorithm>
#include <cctype>
#include <stdexcept>

Vehicle::Vehicle(std::string registration, std::string owner)
    : registration_(std::move(registration)), owner_(std::move(owner)) {
    if (registration_.empty()) throw std::invalid_argument("Registration cannot be empty.");
    if (owner_.empty()) throw std::invalid_argument("Owner name cannot be empty.");
}
const std::string& Vehicle::registration() const { return registration_; }
const std::string& Vehicle::owner() const { return owner_; }

std::string Car::type() const { return "Car"; }
std::string Car::requiredSlotType() const { return "Standard"; }
double Car::hourlyRate() const { return 30.0; }
std::string Motorcycle::type() const { return "Motorcycle"; }
std::string Motorcycle::requiredSlotType() const { return "TwoWheeler"; }
double Motorcycle::hourlyRate() const { return 15.0; }
std::string Truck::type() const { return "Truck"; }
std::string Truck::requiredSlotType() const { return "Heavy"; }
double Truck::hourlyRate() const { return 60.0; }

std::unique_ptr<Vehicle> makeVehicle(const std::string& rawType,
                                     const std::string& registration,
                                     const std::string& owner) {
    std::string type = rawType;
    std::transform(type.begin(), type.end(), type.begin(),
                   [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
    if (type == "car") return std::make_unique<Car>(registration, owner);
    if (type == "motorcycle" || type == "bike") return std::make_unique<Motorcycle>(registration, owner);
    if (type == "truck") return std::make_unique<Truck>(registration, owner);
    throw std::invalid_argument("Unknown vehicle type. Use Car, Motorcycle, or Truck.");
}
