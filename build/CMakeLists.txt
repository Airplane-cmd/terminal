cmake_minimum_required(VERSION 3.14)
project(terminal LANGUAGES CXX)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
find_package(OpenCV REQUIRED)
find_package(QT NAMES Qt5 Qt6 COMPONENTS Core REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Widgets Gui Network REQUIRED)
#set_property(SOURCE ../src/src.cpp PROPERTY SKIP_AUTOMOC ON)
add_executable(bin ../src/movementCalibrator.cpp ../src/burnInator.cpp ../src/recControl.cpp ../src/camHolder.cpp ../src/spinLayout.cpp ../src/udpHolder.cpp ../src/algorithms.cpp ../src/telemetry.cpp ../src/mainWindow.cpp ../src/logs.cpp ../src/powerLimit.cpp ../src/usbHolder.cpp ../src/src.cpp)
target_link_libraries(bin usb-1.0 opencv_core opencv_highgui opencv_imgproc opencv_imgcodecs Qt${QT_VERSION_MAJOR}::Core Qt5::Gui Qt5::Widgets Qt5::Network)
