#include "CheckoutSystem.h"

#include <exception>
#include <iostream>

int main() {
    try {
        CheckoutSystem system;
        system.run();
    } catch (const std::exception& ex) {
        std::cerr << "Fatal error: " << ex.what() << '\n';
        return 1;
    }

    return 0;
}
