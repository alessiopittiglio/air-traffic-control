project(AirTrafficControl)

# ottengo tutti i sorgenti
file(GLOB my_sources *.h *.cpp *.qrc *.ui)

# attivo funzionalita' specifiche di CMake per Qt
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# attiva C++11
add_definitions(-std=gnu++0x)

# aggiungo eseguibile
add_executable(AirTrafficControl ${my_sources})

# verifico l'esistenza delle librerie Qt necessarie
find_package(Qt5Widgets REQUIRED)

# collego le librerie Qt con l'eseguibile
target_link_libraries(AirTrafficControl Qt5::Widgets)