cmake_minimum_required(VERSION 3.16)
project(EquipmentCheckoutSystem VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(EquipmentCheckoutSystem
    src/main.cpp
    src/CheckoutSystem.cpp
)

target_include_directories(EquipmentCheckoutSystem PRIVATE include)

if(MSVC)
    target_compile_options(EquipmentCheckoutSystem PRIVATE /W4 /permissive-)
else()
    target_compile_options(EquipmentCheckoutSystem PRIVATE -Wall -Wextra -Wpedantic)
endif()
