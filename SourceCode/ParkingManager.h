#ifndef PARKING_MANAGER_H
#define PARKING_MANAGER_H
#include "ParkingSlot.h"
#include "Vehicle.h"
#include <ctime>
#include <memory>
#include <string>
#include <vector>

struct ActiveParking {
    std::unique_ptr<Vehicle> vehicle;
    int slotId;
    std::time_t entryTime;
};
struct ParkingReceipt {
    std::string registration;
    std::string vehicleType;
    int slotId;
    long long durationMinutes;
    double fee;
    std::time_t entryTime;
    std::time_t exitTime;
};

class ParkingManager {
    std::vector<ParkingSlot> slots_;
    std::vector<ActiveParking> active_;
    std::vector<ParkingReceipt> history_;
    std::string dataDirectory_;
    int nextSlotId_;
    ParkingSlot* findSlot(int id);
    const ParkingSlot* findSlot(int id) const;
    ActiveParking* findActive(const std::string& registration);
    void saveAll() const;
    void loadAll();
public:
    explicit ParkingManager(std::string dataDirectory = "data");
    void initialiseDefaultSlots();
    int parkVehicle(const std::string& type, const std::string& registration,
                    const std::string& owner);
    ParkingReceipt exitVehicle(const std::string& registration,
                               std::time_t exitTime = std::time(nullptr));
    void addSlot(const std::string& category);
    void showSlots() const;
    void showActiveVehicles() const;
    void showOccupancyReport() const;
    void showHistory() const;
    void searchVehicle(const std::string& registration) const;
    std::size_t activeCount() const;
    std::size_t slotCount() const;
};
#endif
