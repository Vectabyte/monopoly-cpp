#pragma once
#include <string>
#include <vector>

typedef struct player{
        std::string name;
        bool jailed;
        int money;
        int currentPosition;
        int playerId;
        std::vector<int> ownedStreets;
        std::string symbol;
        ColorGroup color;
    } player;