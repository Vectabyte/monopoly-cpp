#pragma once
#include <string>

typedef struct tile {
    int tileIndex;
    int ownerId;
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

std::vector<tile> initizialeGameBoard() {
    // Initialize Gameboard with all tiles
    std::vector<tile> tiles = {

        // 0 GO
        {0,-1,false,0,0,0,0,0,0,0,0,0,0,NONE, "GO","GO"},

        // 1 Mediterranean Ave
        {1,-1,true,60,30,50,0,2,10,30,90,160,250, BROWN, "Mediterranean Ave","MED AVE"},

        // 2 Community Chest
        {2,-1,false,0,0,0,0,0,0,0,0,0,0,NONE, "Community Chest","CC"},

        // 3 Baltic Ave
        {3,-1,true,60,30,50,0,4,20,60,180,320,450, BROWN, "Baltic Ave","BALTIC"},

        // 4 Income Tax
        {4,-1,false,0,0,0,0,0,0,0,0,0,0,NONE, "Income Tax","INCOME"},

        // 5 Reading Railroad
        {5,-1,true,200,100,0,0,25,50,100,200,0,0, GRAY, "Reading Railroad","READ RR"},

        // 6 Oriental Ave
        {6,-1,true,100,50,50,0,6,30,90,270,400,550, BLUE, "Oriental Ave","ORIENTAL"},

        // 7 Chance
        {7,-1,false,0,0,0,0,0,0,0,0,0,0,NONE, "Chance","CHANCE"},

        // 8 Vermont Ave
        {8,-1,true,100,50,50,0,6,30,90,270,400,550, BLUE, "Vermont Ave","VERMONT"},

        // 9 Connecticut Ave
        {9,-1,true,120,60,50,0,8,40,100,300,450,600, BLUE, "Connecticut Ave","CONNET"},

        // 10 Jail
        {10,-1,false,0,0,0,0,0,0,0,0,0,0,NONE, "Jail","JAIL"},

        // 11 St. Charles Place
        {11,-1,true,140,70,100,0,10,50,150,450,625,750, PINK, "St. Charles Place","ST CHAR"},

        // 12 Electric Company
        {12,-1,true,150,75,0,0,0,0,0,0,0,0,PURPLE, "Electric Company","ELECTRIC"},

        // 13 States Ave
        {13,-1,true,140,70,100,0,10,50,150,450,625,750, PINK, "States Ave","STATES"},

        // 14 Virginia Ave
        {14,-1,true,160,80,100,0,12,60,180,500,700,900, PINK, "Virginia Ave","VIRGINIA"},

        // 15 Pennsylvania Railroad
        {15,-1,true,200,100,0,0,25,50,100,200,0,0, GRAY, "Pennsylvania Railroad","PA RR"},

        // 16 St. James Place
        {16,-1,true,180,90,100,0,14,70,200,550,750,950, ORANGE, "St. James Place","STJAMES"},

        // 17 Community Chest
        {17,-1,false,0,0,0,0,0,0,0,0,0,0,NONE, "Community Chest","CC"},

        // 18 Tennessee Ave
        {18,-1,true,180,90,100,0,14,70,200,550,750,950, ORANGE, "Tennessee Ave","TENNES"},

        // 19 New York Ave
        {19,-1,true,200,100,100,0,16,80,220,600,800,1000, ORANGE, "New York Ave","NEWYORK"},

        // 20 Free Parking
        {20,-1,false,0,0,0,0,0,0,0,0,0,0,NONE, "Free Parking","FP"},

        // 21 Kentucky Ave
        {21,-1,true,220,110,150,0,18,90,250,700,875,1050, RED, "Kentucky Ave","KENTUCKY"},

        // 22 Chance
        {22,-1,false,0,0,0,0,0,0,0,0,0,0,NONE, "Chance","CHANCE"},

        // 23 Indiana Ave
        {23,-1,true,220,110,150,0,18,90,250,700,875,1050, RED, "Indiana Ave","INDIANA"},

        // 24 Illinois Ave
        {24,-1,true,240,120,150,0,20,100,300,750,925,1100, RED, "Illinois Ave","ILLINOIS"},

        // 25 B&O Railroad
        {25,-1,true,200,100,0,0,25,50,100,200,0,0, GRAY, "B&O Railroad","B&O RR"},

        // 26 Atlantic Ave
        {26,-1,true,260,130,150,0,22,110,330,800,975,1150, YELLOW, "Atlantic Ave","ATLANTIC"},

        // 27 Ventnor Ave
        {27,-1,true,260,130,150,0,22,110,330,800,975,1150, YELLOW, "Ventnor Ave","VENTNOR"},

        // 28 Water Works
        {28,-1,true,150,75,0,0,0,0,0,0,0,0,PURPLE, "Water Works","WATER"},

        // 29 Marvin Gardens
        {29,-1,true,280,140,150,0,24,120,360,850,1025,1200, YELLOW, "Marvin Gardens","MARVIN"},

        // 30 Go To Jail
        {30,-1,false,0,0,0,0,0,0,0,0,0,0,NONE, "Go To Jail","GOTOJAIL"},

        // 31 Pacific Ave
        {31,-1,true,300,150,200,0,26,130,390,900,1100,1275, GREEN, "Pacific Ave","PACIFIC"},

        // 32 North Carolina Ave
        {32,-1,true,300,150,200,0,26,130,390,900,1100,1275, GREEN, "North Carolina Ave","N CAROL"},

        // 33 Community Chest
        {33,-1,false,0,0,0,0,0,0,0,0,0,0,NONE, "Community Chest","CC"},

        // 34 Pennsylvania Ave
        {34,-1,true,320,160,200,0,28,150,450,1000,1200,1400, GREEN, "Pennsylvania Ave","PENN AV"},

        // 35 Short Line Railroad
        {35,-1,true,200,100,0,0,25,50,100,200,0,0, GRAY, "Short Line Railroad","SHORT RR"},

        // 36 Chance
        {36,-1,false,0,0,0,0,0,0,0,0,0,0,NONE, "Chance","CHANCE"},

        // 37 Park Place
        {37,-1,true,350,175,200,0,35,175,500,1100,1300,1500, DARK_BLUE, "Park Place","PARK PLC"},

        // 38 Luxury Tax
        {38,-1,false,0,0,0,0,0,0,0,0,0,0,NONE, "Luxury Tax","LUXURY"},

        // 39 Boardwalk
        {39,-1,true,400,200,200,0,50,200,600,1400,1700,2000, DARK_BLUE, "Boardwalk","BOARD W"}
    };

    return tiles;
}