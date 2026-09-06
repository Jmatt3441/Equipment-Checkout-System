#pragma once

#include <string>

struct Equipment {
    int id{};
    std::string name;
    std::string category;
    bool checkedOut{false};
    std::string checkedOutTo;
    std::string checkoutDate;
};
