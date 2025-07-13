cmake_minimum_required(VERSION 3.16)

project(AddressBookv1 VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Tell CMake where to find .ui files
set(CMAKE_AUTOUIC_SEARCH_PATHS ${CMAKE_SOURCE_DIR}/Forms)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set(PROJECT_SOURCES
    main.cpp
    mainwindow.cpp
    mainwindow.h
    phonenumberdialog.cpp
    editcontactdialog.cpp
    Forms/mainwindow.ui
)

if(QT_VERSION_MAJOR GREATER_EQUAL 6)
    qt_add_executable(AddressBookv1
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
    qt_finalize_executable(AddressBookv1)
else()
    add_executable(AddressBookv1
        ${PROJECT_SOURCES}
    )
endif()

# Ensure the autogen include dir is available for generated headers like ui_mainwindow.h
target_include_directories(AddressBookv1 PRIVATE
    ${CMAKE_CURRENT_BINARY_DIR}/AddressBookv1_autogen/include
)

target_link_libraries(AddressBookv1 PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
