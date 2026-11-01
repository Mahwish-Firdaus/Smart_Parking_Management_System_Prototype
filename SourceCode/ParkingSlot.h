#ifndef PARKING_SLOT_H
#define PARKING_SLOT_H
#include <string>
class ParkingSlot {
    int id_;
    std::string category_;
    bool occupied_;
    std::string registration_;
public:
    ParkingSlot(int id, std::string category);
    int id() const;
    const std::string& category() const;
    bool occupied() const;
    const std::string& registration() const;
    bool isCompatible(const std::string& requiredCategory) const;
    void park(const std::string& registration);
    void release();
};
#endif
