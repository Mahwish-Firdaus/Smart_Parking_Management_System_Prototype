#ifndef VEHICLE_H
#define VEHICLE_H
#include <memory>
#include <string>

class Vehicle {
protected:
    std::string registration_;
    std::string owner_;
public:
    Vehicle(std::string registration, std::string owner);
    virtual ~Vehicle() = default;
    const std::string& registration() const;
    const std::string& owner() const;
    virtual std::string type() const = 0;
    virtual std::string requiredSlotType() const = 0;
    virtual double hourlyRate() const = 0;
};
class Car final : public Vehicle {
public:
    using Vehicle::Vehicle;
    std::string type() const override;
    std::string requiredSlotType() const override;
    double hourlyRate() const override;
};
class Motorcycle final : public Vehicle {
public:
    using Vehicle::Vehicle;
    std::string type() const override;
    std::string requiredSlotType() const override;
    double hourlyRate() const override;
};
class Truck final : public Vehicle {
public:
    using Vehicle::Vehicle;
    std::string type() const override;
    std::string requiredSlotType() const override;
    double hourlyRate() const override;
};
std::unique_ptr<Vehicle> makeVehicle(const std::string& type,
                                     const std::string& registration,
                                     const std::string& owner);
#endif
