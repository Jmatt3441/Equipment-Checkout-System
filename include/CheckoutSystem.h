#pragma once

#include "Equipment.h"

#include <string>
#include <vector>

class CheckoutSystem {
public:
    explicit CheckoutSystem(std::string dataFile = "data/equipment.csv");

    void run();

private:
    std::string dataFile_;
    std::vector<Equipment> equipment_;

    void load();
    void save() const;
    void ensureSeedData();

    void showMenu() const;
    void listEquipment() const;
    void addEquipment();
    void checkoutEquipment();
    void returnEquipment();
    void searchEquipment() const;
    void showCheckedOut() const;
    void showSummary() const;

    Equipment* findById(int id);
    const Equipment* findById(int id) const;

    static std::string promptLine(const std::string& prompt);
    static int promptInt(const std::string& prompt);
    static std::string today();
    static std::string trim(std::string value);
    static std::string csvEscape(const std::string& value);
    static std::vector<std::string> parseCsvLine(const std::string& line);
};
