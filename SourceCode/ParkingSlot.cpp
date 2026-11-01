#include "ParkingSlot.h"
#include <stdexcept>
#include <utility>
ParkingSlot::ParkingSlot(int id, std::string category)
    : id_(id), category_(std::move(category)), occupied_(false), registration_("") {
    if (id <= 0) throw std::invalid_argument("Slot ID must be positive.");
}
int ParkingSlot::id() const { return id_; }
const std::string& ParkingSlot::category() const { return category_; }
bool ParkingSlot::occupied() const { return occupied_; }
const std::string& ParkingSlot::registration() const { return registration_; }
bool ParkingSlot::isCompatible(const std::string& requiredCategory) const {
    return category_ == requiredCategory;
}
void ParkingSlot::park(const std::string& registration) {
    if (occupied_) throw std::logic_error("Slot is already occupied.");
    occupied_ = true; registration_ = registration;
}
void ParkingSlot::release() {
    if (!occupied_) throw std::logic_error("Slot is already empty.");
    occupied_ = false; registration_.clear();
}
