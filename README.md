# monopoly-cpp
The Repository for the C++ Project. Here we implement monopoly in a CLI Application.
For the correct display of the gameboard it is recommended, to use a monospace font.

## Deviations from the base game

The game was implemented largely as originally designed.
A few small rulechanges where applied with limited impact on the gameplay.
- Cards referencing "move to nearest railroad" do not require you to pay double the rent price, calculation is based on normal rent calculation
- Cards referencing "move to nearest utility" do not require you to pay 10x rolled die as rent price, calculation is based on normal rent calculation
- when bankrupt, there is a menu to try to raise cash. Selling streets in this part is not implemented.
- when bankrupted by the bank, the streets are returned to the bank and not immediatly auctioned of.
- when bankrupted by a player, the player is not forced to immediatly pay 10% interest on the properties.

## Compile Instructions

The project was build and tested on linux with the following compile command:

g++ monopoly.cpp -o monopoly --std=c++11

Furthermore CMake is available and tested with linux and the following compile commands:

cmake -S . -B build
cmake --build build