#include "CheckoutSystem.h"

#include <algorithm>
#include <chrono>
#include <cctype>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>

namespace {
std::string lowerCopy(std::string value) {
    std::transform(value.begin(), value.end(), value.begin(),
        [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
    return value;
}
}

CheckoutSystem::CheckoutSystem(std::string dataFile)
    : dataFile_(std::move(dataFile)) {
    ensureSeedData();
    load();
}

void CheckoutSystem::run() {
    while (true) {
        showMenu();
        const int choice = promptInt("Select an option: ");

        switch (choice) {
        case 1: listEquipment(); break;
        case 2: addEquipment(); break;
        case 3: checkoutEquipment(); break;
        case 4: returnEquipment(); break;
        case 5: searchEquipment(); break;
        case 6: showCheckedOut(); break;
        case 7: showSummary(); break;
        case 0:
            save();
            std::cout << "\nData saved. Goodbye.\n";
            return;
        default:
            std::cout << "\nInvalid option. Please try again.\n";
        }
    }
}

void CheckoutSystem::ensureSeedData() {
    const std::filesystem::path path(dataFile_);
    if (path.has_parent_path()) {
        std::filesystem::create_directories(path.parent_path());
    }
    if (std::filesystem::exists(path)) return;

    std::ofstream out(path);
    out << "id,name,category,checked_out,checked_out_to,checkout_date\n";
    out << "1001,Laptop Dell Latitude,IT,0,,\n";
    out << "1002,Motorola Radio,Communications,0,,\n";
    out << "1003,Tool Kit,Maintenance,0,,\n";
}

void CheckoutSystem::load() {
    equipment_.clear();
    std::ifstream in(dataFile_);
    if (!in) {
        std::cerr << "Warning: could not open " << dataFile_ << "\n";
        return;
    }

    std::string line;
    bool firstLine = true;
    while (std::getline(in, line)) {
        if (firstLine) { firstLine = false; continue; }
        if (trim(line).empty()) continue;

        const auto fields = parseCsvLine(line);
        if (fields.size() < 6) continue;

        try {
            Equipment item;
            item.id = std::stoi(fields[0]);
            item.name = fields[1];
            item.category = fields[2];
            item.checkedOut = fields[3] == "1";
            item.checkedOutTo = fields[4];
            item.checkoutDate = fields[5];
            equipment_.push_back(std::move(item));
        } catch (...) {
        }
    }
}

void CheckoutSystem::save() const {
    std::ofstream out(dataFile_, std::ios::trunc);
    if (!out) {
        std::cerr << "Error: could not save equipment data.\n";
        return;
    }

    out << "id,name,category,checked_out,checked_out_to,checkout_date\n";
    for (const auto& item : equipment_) {
        out << item.id << ','
            << csvEscape(item.name) << ','
            << csvEscape(item.category) << ','
            << (item.checkedOut ? 1 : 0) << ','
            << csvEscape(item.checkedOutTo) << ','
            << csvEscape(item.checkoutDate) << '\n';
    }
}

void CheckoutSystem::showMenu() const {
    std::cout
        << "\n========================================\n"
        << "       EQUIPMENT CHECKOUT SYSTEM\n"
        << "========================================\n"
        << "1. View all equipment\n"
        << "2. Add equipment\n"
        << "3. Check out equipment\n"
        << "4. Return equipment\n"
        << "5. Search equipment\n"
        << "6. View checked-out equipment\n"
        << "7. Inventory summary\n"
        << "0. Save and exit\n"
        << "========================================\n";
}

void CheckoutSystem::listEquipment() const {
    std::cout << "\n" << std::left
              << std::setw(8) << "ID"
              << std::setw(30) << "Name"
              << std::setw(20) << "Category"
              << std::setw(14) << "Status"
              << "Assigned To\n"
              << std::string(90, '-') << '\n';

    if (equipment_.empty()) {
        std::cout << "No equipment records found.\n";
        return;
    }

    for (const auto& item : equipment_) {
        std::cout << std::left
                  << std::setw(8) << item.id
                  << std::setw(30) << item.name.substr(0, 28)
                  << std::setw(20) << item.category.substr(0, 18)
                  << std::setw(14) << (item.checkedOut ? "CHECKED OUT" : "AVAILABLE")
                  << (item.checkedOut ? item.checkedOutTo : "-")
                  << '\n';
    }
}

void CheckoutSystem::addEquipment() {
    std::cout << "\n--- Add Equipment ---\n";
    const int id = promptInt("Equipment ID: ");
    if (findById(id)) {
        std::cout << "An item with that ID already exists.\n";
        return;
    }

    const std::string name = trim(promptLine("Equipment name: "));
    const std::string category = trim(promptLine("Category: "));
    if (name.empty()) {
        std::cout << "Equipment name cannot be empty.\n";
        return;
    }

    equipment_.push_back({id, name, category.empty() ? "Uncategorized" : category, false, "", ""});
    save();
    std::cout << "Equipment added successfully.\n";
}

void CheckoutSystem::checkoutEquipment() {
    std::cout << "\n--- Check Out Equipment ---\n";
    const int id = promptInt("Equipment ID: ");
    Equipment* item = findById(id);

    if (!item) {
        std::cout << "Equipment not found.\n";
        return;
    }
    if (item->checkedOut) {
        std::cout << "That item is already checked out to " << item->checkedOutTo << ".\n";
        return;
    }

    const std::string user = trim(promptLine("Check out to: "));
    if (user.empty()) {
        std::cout << "A user/name is required.\n";
        return;
    }

    item->checkedOut = true;
    item->checkedOutTo = user;
    item->checkoutDate = today();
    save();

    std::cout << item->name << " checked out to " << user
              << " on " << item->checkoutDate << ".\n";
}

void CheckoutSystem::returnEquipment() {
    std::cout << "\n--- Return Equipment ---\n";
    const int id = promptInt("Equipment ID: ");
    Equipment* item = findById(id);

    if (!item) {
        std::cout << "Equipment not found.\n";
        return;
    }
    if (!item->checkedOut) {
        std::cout << "That item is already available.\n";
        return;
    }

    const std::string previousUser = item->checkedOutTo;
    item->checkedOut = false;
    item->checkedOutTo.clear();
    item->checkoutDate.clear();
    save();

    std::cout << item->name << " returned successfully from " << previousUser << ".\n";
}

void CheckoutSystem::searchEquipment() const {
    const std::string query = lowerCopy(trim(promptLine("\nSearch by ID, name, category, or assigned user: ")));
    if (query.empty()) {
        std::cout << "Search text cannot be empty.\n";
        return;
    }

    bool found = false;
    std::cout << '\n';
    for (const auto& item : equipment_) {
        const std::string haystack = lowerCopy(
            std::to_string(item.id) + " " + item.name + " " + item.category + " " + item.checkedOutTo);

        if (haystack.find(query) != std::string::npos) {
            found = true;
            std::cout << '[' << item.id << "] " << item.name
                      << " | " << item.category
                      << " | " << (item.checkedOut ? "Checked out to " + item.checkedOutTo : "Available")
                      << '\n';
        }
    }
    if (!found) std::cout << "No matching equipment found.\n";
}

void CheckoutSystem::showCheckedOut() const {
    std::cout << "\n--- Checked-Out Equipment ---\n";
    bool any = false;
    for (const auto& item : equipment_) {
        if (!item.checkedOut) continue;
        any = true;
        std::cout << '[' << item.id << "] " << item.name
                  << " -> " << item.checkedOutTo
                  << " (" << item.checkoutDate << ")\n";
    }
    if (!any) std::cout << "No equipment is currently checked out.\n";
}

void CheckoutSystem::showSummary() const {
    const auto checkedOut = static_cast<int>(std::count_if(
        equipment_.begin(), equipment_.end(),
        [](const Equipment& item) { return item.checkedOut; }));
    const int total = static_cast<int>(equipment_.size());

    std::cout << "\n--- Inventory Summary ---\n"
              << "Total equipment: " << total << '\n'
              << "Available:       " << total - checkedOut << '\n'
              << "Checked out:     " << checkedOut << '\n';
}

Equipment* CheckoutSystem::findById(int id) {
    auto it = std::find_if(equipment_.begin(), equipment_.end(),
        [id](const Equipment& item) { return item.id == id; });
    return it == equipment_.end() ? nullptr : &(*it);
}

const Equipment* CheckoutSystem::findById(int id) const {
    auto it = std::find_if(equipment_.begin(), equipment_.end(),
        [id](const Equipment& item) { return item.id == id; });
    return it == equipment_.end() ? nullptr : &(*it);
}

std::string CheckoutSystem::promptLine(const std::string& prompt) {
    std::cout << prompt;
    std::string value;
    std::getline(std::cin, value);
    return value;
}

int CheckoutSystem::promptInt(const std::string& prompt) {
    while (true) {
        const std::string raw = trim(promptLine(prompt));
        try {
            std::size_t used = 0;
            const int value = std::stoi(raw, &used);
            if (used == raw.size()) return value;
        } catch (...) {
        }
        std::cout << "Please enter a valid whole number.\n";
    }
}

std::string CheckoutSystem::today() {
    const auto now = std::chrono::system_clock::now();
    const std::time_t current = std::chrono::system_clock::to_time_t(now);
    std::tm local{};
#ifdef _WIN32
    localtime_s(&local, &current);
#else
    localtime_r(&current, &local);
#endif
    std::ostringstream out;
    out << std::put_time(&local, "%Y-%m-%d");
    return out.str();
}

std::string CheckoutSystem::trim(std::string value) {
    auto notSpace = [](unsigned char c) { return !std::isspace(c); };
    value.erase(value.begin(), std::find_if(value.begin(), value.end(), notSpace));
    value.erase(std::find_if(value.rbegin(), value.rend(), notSpace).base(), value.end());
    return value;
}

std::string CheckoutSystem::csvEscape(const std::string& value) {
    if (value.find_first_of(",\"\n\r") == std::string::npos) return value;

    std::string escaped = "\"";
    for (char c : value) {
        if (c == '\"') escaped += "\"\"";
        else escaped += c;
    }
    escaped += '\"';
    return escaped;
}

std::vector<std::string> CheckoutSystem::parseCsvLine(const std::string& line) {
    std::vector<std::string> fields;
    std::string current;
    bool quoted = false;

    for (std::size_t i = 0; i < line.size(); ++i) {
        const char c = line[i];
        if (c == '\"') {
            if (quoted && i + 1 < line.size() && line[i + 1] == '\"') {
                current += '\"';
                ++i;
            } else {
                quoted = !quoted;
            }
        } else if (c == ',' && !quoted) {
            fields.push_back(current);
            current.clear();
        } else {
            current += c;
        }
    }
    fields.push_back(current);
    return fields;
}
