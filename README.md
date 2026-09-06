# Equipment Checkout System

A complete C++17 equipment accountability application for tracking shared tools, devices, radios, computers, keys, and other organizational assets through checkout and return workflows.

## Features

- Add equipment to inventory
- View all equipment and current status
- Check equipment out to a user
- Automatically record checkout dates
- Return checked-out equipment
- Prevent duplicate checkout of unavailable items
- Search by ID, name, category, or assigned user
- View all currently checked-out equipment
- View inventory totals and availability
- Persistent CSV storage between sessions
- Sample inventory included
- Input validation and duplicate-ID protection
- Cross-platform CMake build
- Automated Windows and Linux build checks with GitHub Actions

## Project Structure

```text
Equipment-Checkout-System/
├── include/
│   ├── CheckoutSystem.h
│   └── Equipment.h
├── src/
│   ├── CheckoutSystem.cpp
│   └── main.cpp
├── data/
│   └── equipment.csv
├── .github/workflows/
│   └── build.yml
├── CMakeLists.txt
├── .gitignore
└── README.md
```

## Technologies

**C++17 · STL · OOP · File I/O · CSV Persistence · CMake · GitHub Actions**

The program is compatible with Visual Studio/MSVC and standard GCC/Clang C++17 toolchains.

## Build

From the repository root:

```bash
cmake -S . -B build
cmake --build build --config Release
```

On Windows, the executable will normally be located at:

```text
build/Release/EquipmentCheckoutSystem.exe
```

With a single-configuration generator:

```text
build/EquipmentCheckoutSystem
```

Run the application from the repository root so it can read and update `data/equipment.csv`.

## Menu

```text
========================================
       EQUIPMENT CHECKOUT SYSTEM
========================================
1. View all equipment
2. Add equipment
3. Check out equipment
4. Return equipment
5. Search equipment
6. View checked-out equipment
7. Inventory summary
0. Save and exit
========================================
```

## Why I Built It

This project applies software development to a real inventory and property-accountability workflow. Organizations that share equipment need a repeatable way to identify assets, determine availability, record who has an item, and return it to inventory.

The project demonstrates practical C++ development, object-oriented design, file persistence, data validation, inventory-state management, and business-process modeling.

## Example Inventory

The included sample data starts with:

- Dell Latitude laptop
- Motorola radio
- Maintenance tool kit

Users can add additional assets directly through the application.

## Future Improvements

Planned expansion ideas include a graphical interface, SQLite/PostgreSQL database storage, user authentication, due dates, overdue alerts, barcode/QR scanning, equipment condition tracking, transaction history, audit reports, and web/mobile front ends.

## Developer

**James Matthews**  
Computer Information Technology / Software Development Portfolio Project
