#pragma once
#include <string>

enum ColorGroup {
    NONE,
    BROWN, LIGHT_BLUE, PINK, ORANGE,
    RED, YELLOW, GREEN, DARK_BLUE,
    RAILROAD, UTILITY
};

typedef struct tile {
    bool owned;
    bool buyable;

    int buyPrice;
    int mortgage;
    int housePrice;
    int upgradeStage;

    int price0, price1, price2, price3, price4, price5;

    ColorGroup color;
    std::string tileName;
    std::string shortName;
} tile;