cmake_minimum_required(VERSION 3.16)
project(AddressBook VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets Core Gui)

include_directories(src/models src/core src/ui)

set(SOURCES
    main.cpp
    src/models/contact.cpp
    src/models/contact.h
    src/core/databasemanager.cpp
    src/core/databasemanager.h
    src/ui/mainwindow.cpp
    src/ui/mainwindow.h
    src/ui/contactform.cpp
    src/ui/contactform.h
    src/ui/stylesheet.cpp
    src/ui/stylesheet.h
)

add_executable(AddressBook ${SOURCES})
target_link_libraries(AddressBook PRIVATE Qt6::Widgets Qt6::Core Qt6::Gui)
