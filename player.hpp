#pragma once
#include <string>
#include <vector>

typedef struct player{
        std::string name;
        bool jailed;
        int money;
        int position;
        int playerId;
        std::vector<int> ownedStreets;
    } player;