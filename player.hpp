#pragma once
#include <string>
#include <vector>

enum ColorGroup {
    NONE,
    BROWN, LIGHT_BLUE, PINK, ORANGE,
    RED, YELLOW, GREEN, DARK_BLUE,
};

typedef struct player{
        std::string name;
        bool jailed;
        int money;
        int currentPosition;
        int playerId;
        std::vector<int> ownedStreets;
        ColorGroup color;
    } player;