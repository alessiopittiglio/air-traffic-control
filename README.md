# Air traffic control

An application for the simulation and automatic management of short-range air traffic control. It implements an AI to avoid collisions and to reduce delays.

<p align="center">
  <img src="https://user-images.githubusercontent.com/72530555/147838516-0a692fca-1f32-4d57-a9a1-5b07a9ed5304.gif" />
</p>

## Table of contents
* [General info](#general-info)
* [Advanced information](#advanced-information)
* [Technologies](#technologies)
* [Setup](#setup)

## General info
The airspace is *W*×*H* and is divided into sectors *S*<sub>1</sub>,…,*S*<sub>n</sub> containing *A*<sub>1</sub>,…,*A*<sub>m</sub> airports.

The application asks the user to enter the number *n* of sectors and *m* of airports, after which it generates the map (graph) of the airports (nodes) and routes (arcs) and divides the space into sectors in such a way that a sector contains a number of routes lower than *R*<sub>max</sub> fixed parameter of the problem (for example 5). The map is generated automatically. The simulation starts (and proceeds) by randomly generating planes departing from an airport and heading towards a neighboring airport. The probability of an airplane spawning on a certain route in a certain time interval is regulated by a probability *p* (adjustable by the user) directly proportional to the length of the route. Each airplane has three attributes: the alphanumeric identifier, the altitude (in feet) and the speed (in km/h). For simplicity, the planes move at a constant speed and depart at an altitude of 33,000 feet and a speed of 700 km/h. When they arrive at their destination, they disappear from the map. The objective of the automatic air traffic manager is threefold:

- avoiding collisions in the air by maintaining a vertical distance of at least 1,000 feet between airplanes on the same or intersecting routes;
- avoid airspace congestion by respecting the maximum capacity limit of a sector equal to *C*<sub>max</sub> airplanes (for example 5, which can be set before the simulation starts);
- reduce global delays or minimize the sum of the delays accumulated by the airplane with respect to the established route with initial speed. 

## Advanced information
For the sectorization it was decided to model the sectors using a quadtree. Monte Carlo method was used for traffic management along with a queue.

## Technologies
Project is created with:
- QT 5.15.1
- CMake 3.4 or higher

## Setup
Basic constants (like entity speed control, distance before collision etc.) can be changed in the config.h header file.

1. Clone this repo.
2. Make a build directory in the top level directory: `mkdir build && cd build`
3. Compile: `cmake .. && make`
4. Run it: `./(AirTrafficControl`.
