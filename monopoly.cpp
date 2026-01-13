#include <chrono>
#include <iostream>
#include <limits>
#include <ostream>
#include <random>
#include <string>
#include <vector>
#include <array>
#include <algorithm>

// Color groups for properties
enum ColorGroup {
    NONE,

    // colors
    BROWN,
    BLUE,
    PINK,
    ORANGE,
    RED,
    YELLOW,
    GREEN,
    DARK_BLUE,
    GRAY,
    PURPLE
};

// Symbols for players
const std::vector<std::string> availableSmybols = {
        "♠","♣","♥","♦","●","○","■","□",
        "▲","▼","◆","◇","★","☆","✪","✦","✧","✚","✖",
        "♜","♞","♝","♛","♚"
};

// Map display names to fully expanded ColorGroup
const std::vector<std::pair<std::string, ColorGroup>> availableColors = {
    {"Default", NONE},

    // colors
    {"Brown",       BROWN},
    {"Blue",        BLUE},
    {"Pink",        PINK},
    {"Orange",      ORANGE},
    {"Red",         RED},
    {"Yellow",      YELLOW},
    {"Green",       GREEN},
    {"Dark Blue",   DARK_BLUE},
    {"Gray",        GRAY},
    {"Purple",      PURPLE}
};

// ANSI escape codes for each color
const std::vector<std::pair<std::string, ColorGroup>> colorCodes = {
    {"\033[0m", NONE},// None - Reset

    // colors
    {"\033[38;2;102;51;0m", BROWN},
    {"\033[38;2;0;204;255m", BLUE},
    {"\033[38;2;255;0;255m", PINK},
    {"\033[38;2;255;102;0m", ORANGE},
    {"\033[38;2;255;0;0m", RED},
    {"\033[38;2;255;255;0m", YELLOW},
    {"\033[38;2;0;204;0m", GREEN},
    {"\033[38;2;77;77;255m", DARK_BLUE},
    {"\033[38;2;77;77;77m", GRAY},
    {"\033[38;2;102;0;255m", PURPLE}
};


#include "player.hpp"
#include "tile.hpp"

//Global Variables
std::vector<tile> gameBoard;
std::vector<player> players;
const std::string RESET_COLOR = "\033[0m";
int freeParkingFunds = 0;

void clearInputBuffer() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void clearTerminal() {
    fputs("\x1b[1;1H\x1b[2J\x1b[3J", stdout);
    fflush(stdout);
}

int calculateUtilityRent(int utilitiesOwned, int diceRoll)
{
    if (utilitiesOwned == 1)
        return diceRoll * 4;
    if (utilitiesOwned == 2)
        return diceRoll * 10;
    return 0;
}

bool ownsMonopoly(const std::vector<tile>& board,
                  ColorGroup color,
                  int playerId)
{
    for (const auto& t : board) {
        if (t.color == color && (t.ownerId == -1)) {
            return false;
        }
    }
    return true;
}

int calculateRailroadRent(int railroadsOwned)
{
    switch (railroadsOwned) {
        case 1: return 25;
        case 2: return 50;
        case 3: return 100;
        case 4: return 200;
        default: return 0;
    }
}

int calculatePropertyRent(const tile& t, bool monopoly)
{
    if (t.upgradeStage == 0)
        return monopoly ? t.price0 * 2 : t.price0;

    switch (t.upgradeStage) {
        case 1: return t.price1;
        case 2: return t.price2;
        case 3: return t.price3;
        case 4: return t.price4;
        case 5: return t.price5; // hotel
        default: return 0;
    }
}


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

std::vector<player> initializePlayers() {
    // Get number of players
    std::cout<<"Please enter a number for players between 2 and 6:"<<std::endl;
    int playersCount;
    while (!(std::cin>>playersCount) || playersCount < 2 || playersCount > 6) {
        std::cout<<"Invalid input. Please enter a number for players between 2 and 6:"<<std::endl;
        clearInputBuffer();
    }

    // Vectors to track used symbols and colors
    std::vector<std::string> usedSymbols;
    std::vector<ColorGroup> usedColors;

    // initialize Players
    std::vector<player> initPlayers;
    for (int i = 0; i < playersCount; i++){
        std::cout << "\n==============================================================================" << std::endl;
        player p;
        std::cout<<"Please enter the name of player "<< i+1 <<":"<<std::endl;
        std::cin>>p.name;
        p.jailed = false;
        p.money = 1500;
        p.currentPosition = 0;
        p.playerId = i;
        p.ownedStreets = {};

        // Symbol Selection
        std::cout << "\nChoose your symbol:\n";

        // Display available symbols
        for (size_t j = 0; j < availableSmybols.size(); ++j) {
            if (std::find(usedSymbols.begin(), usedSymbols.end(), availableSmybols[j]) == usedSymbols.end())
                std::cout << j << ": " << availableSmybols[j] << "  ";
        }
        std::cout << "\nEnter the number of your choice: ";

        // Get valid choice
        int choice = -1;
        while (true) {
            clearInputBuffer();
            std::cin >> choice;
            if (choice >= 0 && choice < (int)availableSmybols.size() &&
                std::find(usedSymbols.begin(), usedSymbols.end(), availableSmybols[choice]) == usedSymbols.end()) {
                break;
            }
            std::cout << "Invalid choice, please pick another: ";
        }

        // Assign and mark as used
        std::string playerSymbol = availableSmybols[choice];
        usedSymbols.push_back(playerSymbol);
        p.symbol = playerSymbol;
        std::cout << "You chose symbol: " << p.symbol << "\n";

        // Color Selection
        std::cout << "\nChoose your color:\n";
        for (size_t j = 1; j < availableColors.size(); ++j) {
            if (std::find(usedColors.begin(), usedColors.end(), availableColors[j].second) == usedColors.end()) {
                std::cout << j - 1 << ": " 
                          << colorCodes[j].first << p.symbol << RESET_COLOR 
                          << " (" << availableColors[j].first << ")  ";
            }
        }
        std::cout << "\nEnter the number of your choice: ";

        // Get valid choice
        int colorChoice = -1;
        while (true) {
            clearInputBuffer();
            std::cin >> colorChoice;
            if (colorChoice >= 0 && colorChoice < ((int)availableColors.size() - 1) &&
                std::find(usedColors.begin(), usedColors.end(), availableColors[colorChoice + 1].second) == usedColors.end()) {
                break;
            }
            std::cout << "Invalid choice, please pick another: ";
        }

        // Assign and mark as used
        ColorGroup playerColor = availableColors[colorChoice + 1].second; // .second to get ColorGroup from pair (mapping)
        p.color = playerColor;
        usedColors.push_back(playerColor);
        // usage of colorCodes vector to get ANSI code, then reset after symbol
        std::cout << "You chose color: " << colorCodes[colorChoice + 1].first << p.symbol << RESET_COLOR << " (" << availableColors[colorChoice + 1].first << ")" << std::endl;
        // Add player to the list
        initPlayers.push_back(p);
    }
    return initPlayers;
}

void displayGameBoard(){
    clearTerminal();

    //Map players to positions
    //10 = Free
    //40 = Jail
    std::array<std::vector<std::string>, 41> positionMap;
    for (const player& p : players) {
        if (p.jailed) {
            positionMap[40].push_back(p.symbol);
            continue;
        }
        positionMap[p.currentPosition].push_back(p.symbol);
    }
    
    //Raw Gameboard String
    std::string board = R"(┌────────────┬────────────┬────────────┬────────────┬────────────┬────────────┬────────────┬────────────┬────────────┬────────────┬────────────┐
│ 20 FP      │ 21 KENTUCKY│ 22 CHANCE  │ 23 INDIANA │ 24 ILLINOIS│ 25 B&O RR  │ 26 ATLANTIC│ 27 VENTNOR │ 28 WATER   │ 29 MARVIN  │ 30 GOTOJAIL│
│            │ [O21]      │            │ [O23]      │ [O24]      │ [O25]      │ [O26]      │ [O27]      │ [O28]      │ [O29]      │            │
│ [20]   │ [21]   │ [22]   │ [23]   │ [24]   │ [25]   │ [26]   │ [27]   │ [28]   │ [29]   │ [30]   │
│            │            │            │            │            │            │            │            │            │            │            │
├────────────┼────────────┴────────────┴────────────┴────────────┴────────────┴────────────┴────────────┴────────────┴────────────┼────────────┤
│ 19 NEWYORK │                                                                                                                    │ 31 PACIFIC │
│ [O19]      │                                                                                                                    │ [O31]      │
│ [19]   │                                                                                                                    │ [31]   │
│            │                                                                                                                    │            │
├────────────┤                                                                                                                    ├────────────┤
│ 18 TENNES  │                                                                                                                    │ 32 N CAROL │
│ [O18]      │                                                                                                                    │ [O32]      │
│ [18]   │                                                                                                                    │ [32]   │
│            │                                                                                                                    │            │
├────────────┤                                                                                                                    ├────────────┤
│ 17 CC      │                                                                                                                    │ 33 CC      │
│            │                                                                                                                    │            │
│ [17]   │                                                                                                                    │ [33]   │
│            │                                                                                                                    │            │
├────────────┤                                                                                                                    ├────────────┤
│ 16 STJAMES │                                                                                                                    │ 34 PENN AV │
│ [O16]      │                                                                                                                    │ [O34]      │
│ [16]   │                                                                                                                    │ [34]   │
│            │                                                                                                                    │            │
├────────────┤                                                                                                                    ├────────────┤
│ 15 PA RR   │                                                                                                                    │ 35 SHORT RR│
│ [O15]      │                                                                                                                    │ [O35]      │
│ [15]   │                                                                                                                    │ [35]   │
│            │                                                                                                                    │            │
├────────────┤                                                                                                                    ├────────────┤
│ 14 VIRGINIA│                                                                                                                    │ 36 CHANCE  │
│ [O14]      │                                                                                                                    │            │
│ [14]   │                                                                                                                    │ [36]   │
│            │                                                                                                                    │            │
├────────────┤                                                                                                                    ├────────────┤
│ 13 STATES  │                                                                                                                    │ 37 PARK PLC│
│ [O13]      │                                                                                                                    │ [O37]      │
│ [13]   │                                                                                                                    │ [37]   │
│            │                                                                                                                    │            │
├────────────┤                                                                                                                    ├────────────┤
│ 12 ELECTRIC│                                                                                                                    │ 38 LUXURY  │
│ [O12]      │                                                                                                                    │            │
│ [12]   │                                                                                                                    │ [38]   │
│            │                                                                                                                    │            │
├────────────┤                                                                                                                    ├────────────┤
│ 11 ST CHAR │                                                                                                                    │ 39 BOARD W │
│ [O11]      │                                                                                                                    │ [O39]      │
│ [11]   │                                                                                                                    │ [39]   │
│            │                                                                                                                    │            │
├───┬────────┼────────────┬────────────┬────────────┬────────────┬────────────┬────────────┬────────────┬────────────┬────────────┼────────────┤
│ F │ 10 JAIL│ 9  CONNET  │ 8 VERMONT  │ 7 CHANCE   │ 6 ORIENTAL │ 5 READ RR  │ 4 INCOME   │ 3 BALTIC   │ 2 CC       │ 1 MED AVE  │ 0 GO       │
│ R │[40]│ [O09]      │ [O08]      │            │ [O06]      │ [O05]      │            │ [O03]      │            │ [O01]      │            │
│ E └────────┤ [09]   │ [08]   │ [07]   │ [06]   │ [05]   │ [04]   │ [03]   │ [02]   │ [01]   │ [00]   │
│ E [10] │            │            │            │            │            │            │            │            │            │            │
└────────────┴────────────┴────────────┴────────────┴────────────┴────────────┴────────────┴────────────┴────────────┴────────────┴────────────┘)";

    // Replace placeholders with player symbols
    for (int i = 0; i < 41; ++i) {
        // Create placeholder string [XX] format with leading 0's if needed
        std::string placeholder = "[" + std::string(i < 10 ? "0" : "") + std::to_string(i) + "]";

        std::string replacement;

        // Append player symbols at this position, count number of players in field
        int counter = 0;
        for (const auto& symbol : positionMap[i]) {
            counter++;
            replacement += symbol;
        }

        // Fill remaining space with spaces to keep field width consistent
        while (counter < 6) {
            replacement += " ";
            counter++;
        }

        replacement = "[" + replacement + "]";

        // Replace in board string
        size_t pos = board.find(placeholder);
        if (pos != std::string::npos) {
            board.replace(pos, 4, replacement);
        }
    }

    // Replace player symbols with colored versions
    for (const player& p : players) {
        std::string replacement = colorCodes[p.color].first + p.symbol + RESET_COLOR;
        size_t pos = board.find(p.symbol);
        if (pos != std::string::npos) {
            board.replace(pos, p.symbol.length(), replacement);
        }
    }

    // Replace tile short names with colored versions
    for (const tile& t : gameBoard) {
        std::string replacement = " " + colorCodes[t.color].first + t.shortName + RESET_COLOR;
        size_t pos = board.find(" " + t.shortName );
        if (pos != std::string::npos) {
            board.replace(pos, t.shortName.length() + 1, replacement);
        }

        std::string placeholderOwned = "[O" + std::string(t.tileIndex < 10 ? "0" : "") + std::to_string(t.tileIndex) + "]";
        std::string ownedStatus;

        // Place owned / houses
        size_t posStatus = board.find(placeholderOwned);
        if (posStatus != std::string::npos) {
            if (t.ownerId == -1) {
                ownedStatus = "     ";
            } else {
                std::string color = colorCodes[players[t.ownerId].color].first;
                switch (t.upgradeStage){
                    case 1:
                        ownedStatus = color + "⌂    " + RESET_COLOR;
                        break;
                    case 2:
                        ownedStatus = color + "⌂⌂   " + RESET_COLOR;
                        break;
                    case 3:
                        ownedStatus = color + "⌂⌂⌂  " + RESET_COLOR;
                        break;
                    case 4:
                        ownedStatus = color + "⌂⌂⌂⌂ " + RESET_COLOR;
                        break;
                    case 5:
                        ownedStatus = color + "HOTEL" + RESET_COLOR;
                        break;
                    default:
                        ownedStatus = color + "OWNED" + RESET_COLOR;
                        break;
                }
            }
            board.replace(posStatus, 5, ownedStatus);
        }
    }

    std::cout << board << std::endl;

}

/*

0  = GO
1  = Mediterranean Ave
2  = Community Chest
3  = Baltic Ave
4  = Income Tax
5  = Reading Railroad
6  = Oriental Ave
7  = Chance
8  = Vermont Ave
9  = Connecticut Ave
10 = Jail
11 = St. Charles Place
12 = Electric Company
13 = States Ave
14 = Virginia Ave
15 = Pennsylvania Railroad
16 = St. James Place
17 = Community Chest
18 = Tennessee Ave
19 = New York Ave
20 = Free Parking
21 = Kentucky Ave
22 = Chance
23 = Indiana Ave
24 = Illinois Ave
25 = B&O Railroad
26 = Atlantic Ave
27 = Ventnor Ave
28 = Water Works
29 = Marvin Gardens
30 = Go To Jail
31 = Pacific Ave
32 = North Carolina Ave
33 = Community Chest
34 = Pennsylvania Ave
35 = Short Line Railroad
36 = Chance
37 = Park Place
38 = Luxury Tax
39 = Boardwalk

40 % 40 = 0 -> Reset
10 -> Jail or no jail 
*/

int rollDice(){
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1,6);
    
    return dis(gen);
}

bool checkPasch(int &x, int &y){
    if(x != y){
        return false;
    }else{
        return true;
    }
}

void arrest(player &p){
    p.jailed = true;
    displayGameBoard();
    std::cout<<"You have been arrested! 😡"<<std::endl;
}

void visualDice(int &x){
    switch (x) {
        case 1:{
            std::cout<<"⚀ ";
            break;
        }case 2:{
            std::cout<<"⚁ ";
            break;
        }case 3:{
            std::cout<<"⚂ ";
            break;
        }case 4:{
            std::cout<<"⚃ ";
            break;
        }case 5:{
            std::cout<<"⚄ ";
            break;
        }case 6:{
            std::cout<<"⚅ ";
            break;
        }
    }
}

void transferMoney(player &from, player &to, int amount){
    from.money = from.money - amount;
    to.money = to.money + amount;
}

void movePlayer(int &x, int &y, player &p){
    int s = x+y;
    if(p.currentPosition+s > 40){
        p.money = p.money + 200;
    }
    p.currentPosition = (p.currentPosition+s)%40;
    displayGameBoard();
    visualDice(x);
    visualDice(y);
    std::cout<<std::endl;
    switch (p.currentPosition){
        case 0:{ //Tiletyp: GO
            p.money = p.money + 400;
            break;
        }case 2:{ //Tiletyp: Com Chest
            #
            break;
        }
        case 4:{ //Tiletyp: Income Tax
            p.money = p.money - 200;
            break;
        }case 7:{ //Tiletyp: Chance
            #
            break;
        }case 10:{ //Tiletyp: visit Jail
            break;
        }
        case 17:{ //Tiletyp: Com Chest
            #
            break;
        }case 20:{ //Tiletyp: FreeParking
            p.money = p.money + freeParkingFunds;
            break;
        }case 22:{ //Tiletyp: Chance
            #
            break;
        }case 30:{ //Tiletyp: GoToJail
            arrest(p);
            break;
        }case 33:{ //Tiletyp: Com Chest
            #
            break;
        }case 36:{ //Tiletyp: Chance
            #
            break;
        }case 38:{ //Tiletyp: Luxury Tax
            p.money = p.money - 100;
            break;
        }default:{ //Tiletyp: Streets, Trainstations, Facilities
            tile currentfield = gameBoard[p.currentPosition];
            if( currentfield.ownerId == -1){
                if(p.money >= currentfield.buyPrice){
                    bool correct = false;
                    while(!correct){
                        std::cout<<colorCodes[p.color].first << p.symbol << " " << p.name << RESET_COLOR 
                        << "Do you want to buy " << currentfield.tileName << " for " << currentfield.buyPrice << " ?"<<std::endl;
                        std::cout<<"------------------\n"
                        <<"| 1: YES | "
                        <<"0: NO |\n"
                        <<"------------------\n"
                        <<std::endl;
                        int sel;
                        std::cin>>sel;
                        switch (sel) {
                            case 1:{
                                p.money = p.money - currentfield.buyPrice;
                                currentfield.ownerId = p.playerId;
                                displayGameBoard();
                                correct = true;
                                std::cout<<"Bravo you have succesfully puchased "<< currentfield.tileName <<"! 🥳" <<std::endl;
                                break;
                            }case 0:{
                                displayGameBoard();
                                std::cout<<"Why tho? 🤨 "<<std::endl;
                                correct = true;
                                break;
                            }default:{
                                displayGameBoard();
                                std::cout<<"No Valid Input! 😡"<<std::endl;
                            }
                        }
                    }
                }else{
                    displayGameBoard();
                    std::cout<<"Not enough Money. 😢"<<std::endl;
                }
            }else{
                displayGameBoard();
                int payload;
                switch (currentfield.upgradeStage){
                    case 1:{
                        payload = currentfield.price1;
                    }case 2:{
                        payload = currentfield.price2;
                    }case 3:{
                        payload = currentfield.price3;
                    }case 4:{
                        payload = currentfield.price4;
                    }case 5:{
                        payload = currentfield.price5;
                    }default:{
                        payload = currentfield.price0;
                    }
                }
                bool bankrupt = false;
                p.money = p.money - payload;
                if(p.money<0){
                    #
                }
                if(!bankrupt){
                    players[currentfield.ownerId].money = players[currentfield.ownerId].money + payload;
                    std::cout<<"You had to pay " << payload << " to " << players[currentfield.ownerId].name <<std::endl;
                }
            }
            break;
        }
    }
}

//Main function for the GameLoop, with a selection for the possible actions in Monopoly
bool action(int &sel, player &p, int &count, bool &ok){
    std::cout<<colorCodes[p.color].first + p.symbol + " " + p.name + RESET_COLOR + ", it's your turn!\n"
    <<"What do you want to do? \n"
    <<"--------------------------------------------------------------------------------------------------------------------------------------------\n"
    <<"| 1 = Roll the dices | "
    <<"2 = Financial Menue | "
    <<"3 = Building Menue | "
    <<"4 = Trade with player |"
    <<"0 = End your turn |"
    <<"77 = Quit the whole game early | "
    <<"Balance: "<<p.money<<"$\n"
    <<"--------------------------------------------------------------------------------------------------------------------------------------------"
    <<std::endl;
    std::cin>>sel;
    switch (sel) {
        case 0:{
            displayGameBoard();
            if(!ok){
                std::cout<<"You havn't rolled enough dice! 😡"<<std::endl;
            }
            return ok;
        }case 1:{
            if(!count || ok == false){
                count++;
                int x = rollDice();
                int y = rollDice();
                movePlayer(x,y,p);
                if(count == 3 && checkPasch(x,y)){
                    arrest(p);
                    sel = 0;
                    return true;
                }
                if(!checkPasch(x, y)){
                    ok = true;
                }else{
                    ok = false;
                }
            }else{
                displayGameBoard();
                std::cout<<"You have already rolled enough dice! 😡"<<std::endl;
            }
            return false;
        }case 2:{
            #
            return false;
        }case 3:{
            #
            return false;
        }case 4:{
            #
            return false;
        }case 77:{
            return true;
        }default:{
            sel = -1;
            displayGameBoard();
            std::cout<<"No Valid Input! 😡"<<std::endl;
            return false;
        }
    }
}

int main(){
    // Initialize Gameboard
    gameBoard = initizialeGameBoard();
    players = initializePlayers();

    //Randomize Player Order
    random_shuffle(players.begin(), players.end());

    //Display Gameboard
    displayGameBoard();

    //start of Gameloop :)
    int sel;
    do{
        for(player &currentPlayer : players){
            bool control = false;
            bool rolled = false;
            int count = 0;
            do{
                control = action(sel,currentPlayer,count,rolled);
                if(sel == 77){
                    break;
                }else if(!control){
                    sel = -1;
                }
            }while(sel);
            if(sel == 77){
                break;
            }
        }
    }while(sel != 77);

    return 0;
}