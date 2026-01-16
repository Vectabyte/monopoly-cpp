# monopoly-cpp
The Repository for the C++ Project. Here we implement monopoly in a CLI Application.

g++ monopoly.cpp -o monopoly.exe --std=c++11

# Deviations from the base game

The game was implemented largely as originally designed.
A few small rulechanges where applied with limited impact on the gameplay.
- Cards referencing "move to nearest railroad" do not require you to pay double the rent price, calculation is based on normal rent calculation
- Cards referencing "move to nearest utility" do not require you to pay 10x rolled die as rent price, calculation is based on normal rent calculation
- when bankrupt, there is a menu to try to raise cash. Selling streets in this part is not implemented.
- when bankrupted by the bank, the streets are returned to the bank and not immediatly auctioned of.
- when bankrupted by a player, the player is not forced to immediatly pay 10% interest on the properties.