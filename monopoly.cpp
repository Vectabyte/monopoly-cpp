#include <iostream>
#include <limits>
#include <random>
#include <string>
#include <vector>
#include <array>
#include <algorithm>

// Color groups for properties
enum ColorGroup {
    NONE,

    // Standard colors - disabled due to similarity in terminal
    /*
    BLACK,
    RED,
    GREEN,
    YELLOW,
    BLUE,
    MAGENTA,
    CYAN,
    WHITE,
    */

    // Bright colors
    BRIGHT_BLACK,
    BRIGHT_RED,
    BRIGHT_GREEN,
    BRIGHT_YELLOW,
    BRIGHT_BLUE,
    BRIGHT_MAGENTA,
    BRIGHT_CYAN,
    BRIGHT_WHITE,
};

// Symbols for players
const std::vector<std::string> availableSmybols = {
        "♠","♣","♥","♦","●","○","■","□",
        "▲","▼","◆","◇","★","☆","✪","✦","✧","✚","✖",
        "⚀","⚁","⚂","⚃","⚄","⚅",
        "♜","♞","♝","♛","♚"
};

// Map display names to fully expanded ColorGroup
const std::vector<std::pair<std::string, ColorGroup>> availableColors = {
    // Standard variants - disabled due to similarity in terminal
    /*
    {"Black",        BLACK},
    {"Red",          RED},
    {"Green",        GREEN},
    {"Yellow",       YELLOW},
    {"Blue",         BLUE},
    {"Magenta",      MAGENTA},
    {"Cyan",         CYAN},
    {"White",        WHITE},
    */

    // Bright variants
    {"Bright Black", BRIGHT_BLACK},
    {"Bright Red",   BRIGHT_RED},
    {"Bright Green", BRIGHT_GREEN},
    {"Bright Yellow",BRIGHT_YELLOW},
    {"Bright Blue",  BRIGHT_BLUE},
    {"Bright Magenta",BRIGHT_MAGENTA},
    {"Bright Cyan",  BRIGHT_CYAN},
    {"Bright White", BRIGHT_WHITE}
};

// ANSI escape codes for each color
const std::vector<std::string> colorCodes = {
    "\033[30m", // Black
    "\033[31m", // Red
    "\033[32m", // Green
    "\033[33m", // Yellow
    "\033[34m", // Blue
    "\033[35m", // Magenta
    "\033[36m", // Cyan
    "\033[37m", // White

    // Bright colors
    "\033[90m", // Bright Black
    "\033[91m", // Bright Red
    "\033[92m", // Bright Green
    "\033[93m", // Bright Yellow
    "\033[94m", // Bright Blue
    "\033[95m", // Bright Magenta
    "\033[96m", // Bright Cyan
    "\033[97m"  // Bright White
};


const std::string RESET_COLOR = "\033[0m";

#include "player.hpp"
#include "tile.hpp"

void clearInputBuffer() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void clearTerminal() {
    fputs("\x1b[1;1H\x1b[2J\x1b[3J", stdout);
    fflush(stdout);
}

int rollDice(){
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1,6);
    
    return dis(gen);
}

bool checkPasch(int x, int y){
    if(x != y){
        return false;
    }else{
        return true;
    }
}

void movePlayer(int x, int y, player &p){
    int s = x+y;
    int temp = p.currentPosition+s; 
    if(temp < 40){
        p.currentPosition = temp;
    }else {
        p.currentPosition = temp - 40;
    }
}

void arrest(player &p){
    p.jailed = true;
    p.currentPosition = 10;
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
        if (t.color == color && (!t.owned)) {
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
        {0,false,false,0,0,0,0,0,0,0,0,0,0,NONE, "GO","GO"},

        // 1 Mediterranean Ave
        {1,false,true,60,30,50,0,2,10,30,90,160,250, BRIGHT_BLACK, "Mediterranean Ave","MED AVE"},

        // 2 Community Chest
        {2,false,false,0,0,0,0,0,0,0,0,0,0,NONE, "Community Chest","CC"},

        // 3 Baltic Ave
        {3,false,true,60,30,50,0,4,20,60,180,320,450, BRIGHT_BLACK, "Baltic Ave","BALTIC"},

        // 4 Income Tax
        {4,false,false,0,0,0,0,0,0,0,0,0,0,NONE, "Income Tax","INCOME"},

        // 5 Reading Railroad
        {5,false,true,200,100,0,0,25,50,100,200,0,0, BRIGHT_RED, "Reading Railroad","READ RR"},

        // 6 Oriental Ave
        {6,false,true,100,50,50,0,6,30,90,270,400,550, BRIGHT_CYAN, "Oriental Ave","ORIENT"},

        // 7 Chance
        {7,false,false,0,0,0,0,0,0,0,0,0,0,NONE, "Chance","CHANCE"},

        // 8 Vermont Ave
        {8,false,true,100,50,50,0,6,30,90,270,400,550, BRIGHT_CYAN, "Vermont Ave","VERMONT"},

        // 9 Connecticut Ave
        {9,false,true,120,60,50,0,8,40,100,300,450,600, BRIGHT_CYAN, "Connecticut Ave","CONNET"},

        // 10 Jail
        {10,false,false,0,0,0,0,0,0,0,0,0,0,NONE, "Jail","JAIL"},

        // 11 St. Charles Place
        {11,false,true,140,70,100,0,10,50,150,450,625,750, BRIGHT_MAGENTA, "St. Charles Place","ST CHAR"},

        // 12 Electric Company
        {12,false,true,150,75,0,0,0,0,0,0,0,0, BRIGHT_CYAN, "Electric Company","ELECTRIC"},

        // 13 States Ave
        {13,false,true,140,70,100,0,10,50,150,450,625,750, BRIGHT_MAGENTA, "States Ave","STATES"},

        // 14 Virginia Ave
        {14,false,true,160,80,100,0,12,60,180,500,700,900, BRIGHT_MAGENTA, "Virginia Ave","VIRGINIA"},

        // 15 Pennsylvania Railroad
        {15,false,true,200,100,0,0,25,50,100,200,0,0, BRIGHT_RED, "Pennsylvania Railroad","PA RR"},

        // 16 St. James Place
        {16,false,true,180,90,100,0,14,70,200,550,750,950, BRIGHT_YELLOW, "St. James Place","STJAMES"},

        // 17 Community Chest
        {17,false,false,0,0,0,0,0,0,0,0,0,0,NONE, "Community Chest","CC"},

        // 18 Tennessee Ave
        {18,false,true,180,90,100,0,14,70,200,550,750,950, BRIGHT_YELLOW, "Tennessee Ave","TENNES"},

        // 19 New York Ave
        {19,false,true,200,100,100,0,16,80,220,600,800,1000, BRIGHT_YELLOW, "New York Ave","NEWYORK"},

        // 20 Free Parking
        {20,false,false,0,0,0,0,0,0,0,0,0,0,NONE, "Free Parking","FP"},

        // 21 Kentucky Ave
        {21,false,true,220,110,150,0,18,90,250,700,875,1050, BRIGHT_RED, "Kentucky Ave","KENTUCKY"},

        // 22 Chance
        {22,false,false,0,0,0,0,0,0,0,0,0,0,NONE, "Chance","CHANCE"},

        // 23 Indiana Ave
        {23,false,true,220,110,150,0,18,90,250,700,875,1050, BRIGHT_RED, "Indiana Ave","INDIANA"},

        // 24 Illinois Ave
        {24,false,true,240,120,150,0,20,100,300,750,925,1100, BRIGHT_RED, "Illinois Ave","ILLINOIS"},

        // 25 B&O Railroad
        {25,false,true,200,100,0,0,25,50,100,200,0,0, BRIGHT_RED, "B&O Railroad","B&O RR"},

        // 26 Atlantic Ave
        {26,false,true,260,130,150,0,22,110,330,800,975,1150, BRIGHT_YELLOW, "Atlantic Ave","ATLANTIC"},

        // 27 Ventnor Ave
        {27,false,true,260,130,150,0,22,110,330,800,975,1150, BRIGHT_YELLOW, "Ventnor Ave","VENTNOR"},

        // 28 Water Works
        {28,false,true,150,75,0,0,0,0,0,0,0,0, BRIGHT_CYAN, "Water Works","WATER"},

        // 29 Marvin Gardens
        {29,false,true,280,140,150,0,24,120,360,850,1025,1200, BRIGHT_YELLOW, "Marvin Gardens","MARVIN"},

        // 30 Go To Jail
        {30,false,false,0,0,0,0,0,0,0,0,0,0,NONE, "Go To Jail","GOTOJAIL"},

        // 31 Pacific Ave
        {31,false,true,300,150,200,0,26,130,390,900,1100,1275, BRIGHT_GREEN, "Pacific Ave","PACIFIC"},

        // 32 North Carolina Ave
        {32,false,true,300,150,200,0,26,130,390,900,1100,1275, BRIGHT_GREEN, "North Carolina Ave","N CAROL"},

        // 33 Community Chest
        {33,false,false,0,0,0,0,0,0,0,0,0,0,NONE, "Community Chest","CC"},

        // 34 Pennsylvania Ave
        {34,false,true,320,160,200,0,28,150,450,1000,1200,1400, BRIGHT_GREEN, "Pennsylvania Ave","PENN AV"},

        // 35 Short Line Railroad
        {35,false,true,200,100,0,0,25,50,100,200,0,0, BRIGHT_RED, "Short Line Railroad","SHORT RR"},

        // 36 Chance
        {36,false,false,0,0,0,0,0,0,0,0,0,0,NONE, "Chance","CHANCE"},

        // 37 Park Place
        {37,false,true,350,175,200,0,35,175,500,1100,1300,1500, BRIGHT_BLUE, "Park Place","PARK PLC"},

        // 38 Luxury Tax
        {38,false,false,0,0,0,0,0,0,0,0,0,0,NONE, "Luxury Tax","LUXURY"},

        // 39 Boardwalk
        {39,false,true,400,200,200,0,50,200,600,1400,1700,2000, BRIGHT_BLUE, "Boardwalk","BOARD W"}
    };

    return tiles;
}

void displayGameBoard(std::vector<player>& players, std::vector<tile>& gameBoard){
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
│            │            │            │            │            │            │            │            │            │            │            │
│ [20]   │ [21]   │ [22]   │ [23]   │ [24]   │ [25]   │ [26]   │ [27]   │ [28]   │ [29]   │ [30]   │
│            │            │            │            │            │            │            │            │            │            │            │
├────────────┼────────────┴────────────┴────────────┴────────────┴────────────┴────────────┴────────────┴────────────┴────────────┼────────────┤
│ 19 NEWYORK │                                                                                                                    │ 31 PACIFIC │
│            │                                                                                                                    │            │
│ [19]   │                                                                                                                    │ [31]   │
│            │                                                                                                                    │            │
├────────────┤                                                                                                                    ├────────────┤
│ 18 TENNES  │                                                                                                                    │ 32 N CAROL │
│            │                                                                                                                    │            │
│ [18]   │                                                                                                                    │ [32]   │
│            │                                                                                                                    │            │
├────────────┤                                                                                                                    ├────────────┤
│ 17 CC      │                                                                                                                    │ 33 CC      │
│            │                                                                                                                    │            │
│ [17]   │                                                                                                                    │ [33]   │
│            │                                                                                                                    │            │
├────────────┤                                                                                                                    ├────────────┤
│ 16 STJAMES │                                                                                                                    │ 34 PENN AV │
│            │                                                                                                                    │            │
│ [16]   │                                                                                                                    │ [34]   │
│            │                                                                                                                    │            │
├────────────┤                                                                                                                    ├────────────┤
│ 15 PA RR   │                                                                                                                    │ 35 SHORT RR│
│            │                                                                                                                    │            │
│ [15]   │                                                                                                                    │ [35]   │
│            │                                                                                                                    │            │
├────────────┤                                                                                                                    ├────────────┤
│ 14 VIRGINIA│                                                                                                                    │ 36 CHANCE  │
│            │                                                                                                                    │            │
│ [14]   │                                                                                                                    │ [36]   │
│            │                                                                                                                    │            │
├────────────┤                                                                                                                    ├────────────┤
│ 13 STATES  │                                                                                                                    │ 37 PARKPLC │
│            │                                                                                                                    │            │
│ [13]   │                                                                                                                    │ [37]   │
│            │                                                                                                                    │            │
├────────────┤                                                                                                                    ├────────────┤
│ 12 ELECTRIC│                                                                                                                    │ 38 LUXURY  │
│            │                                                                                                                    │            │
│ [12]   │                                                                                                                    │ [38]   │
│            │                                                                                                                    │            │
├────────────┤                                                                                                                    ├────────────┤
│ 11 ST CHAR │                                                                                                                    │ 39 Board W │
│            │                                                                                                                    │            │
│ [11]   │                                                                                                                    │ [39]   │
│            │                                                                                                                    │            │
├───┬────────┼────────────┬────────────┬────────────┬────────────┬────────────┬────────────┬────────────┬────────────┬────────────┼────────────┤
│ F │ 10 JAIL│ 9  CONNET  │ 8 VERMONT  │ 7 CHANCE   │ 6 ORIENTAL │ 5 READ RR  │ 4 INCOME   │ 3 BALTIC   │ 2 CC       │ 1 MED AVE  │ 0 GO       │
│ R │[40]│            │            │            │            │            │            │            │            │            │            │
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
        std::string replacement = colorCodes[p.color] + p.symbol + RESET_COLOR;
        size_t pos = board.find(p.symbol);
        if (pos != std::string::npos) {
            board.replace(pos, p.symbol.length(), replacement);
        }
    }

    // Replace tile short names with colored versions
    for (const tile& t : gameBoard) {
        std::string replacement = " " + colorCodes[t.color] + t.shortName + RESET_COLOR;
        std::cout << "Replacing tile " << t.tileIndex << " short name '" << t.shortName << "' with '" << replacement << "'\n";
        size_t pos = board.find(" " + t.shortName );
        if (pos != std::string::npos) {
            board.replace(pos, t.shortName.length()+1, replacement);
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
//Main function for the GameLoop, with a selection for the possible actions in Monopoly
bool action(int &sel, player &p, int &count, bool &ok){
    std::cout<<"What to do? \n"
    <<"1 = Roll the dices \n"
    <<"2 = Hypothesize cards \n"
    <<"3 = Build houses \n"
    <<"4 = Trade with player \n"
    <<"0 = End your turn \n"
    <<"10 = Quit the whole game early"
    <<std::endl;
    std::cin>>sel;
    switch (sel) {
        default:{
            sel = -1;
            return false;
        }case 0:{
            return ok;
        }case 1:{
            count++;
            int x = rollDice();
            int y = rollDice();
            if(count == 3 && checkPasch(x,y)){
                arrest(p);
                sel = 0;
                return true;
            }
            movePlayer(x,y,p);
            #
            if(!checkPasch(x, y)){
                ok = true;
            }else{
                ok = false;
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
        }case 10:{
            #
            return true;
        }

    }
}

int main(){
    // Initialize Gameboard
    std::vector<tile> gameBoard = initizialeGameBoard();

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
    std::vector<player> players;
    for (int i = 0; i < playersCount; i++){
        player p;
        std::cout<<"Please enter the name of player "<< i+1 <<":"<<std::endl;
        std::cin>>p.name;
        p.jailed = false;
        p.money = 1500;
        p.currentPosition = 0;
        p.playerId = i;
        p.ownedStreets = {};

        // Symbol Selection
        std::cout << "Choose your symbol:\n";

        // Display available symbols
        for (size_t j = 0; j < availableSmybols.size(); ++j) {
            if (std::find(usedSymbols.begin(), usedSymbols.end(), availableSmybols[j]) == usedSymbols.end())
                std::cout << j << ": " << availableSmybols[j] << "  ";
        }
        std::cout << "\nEnter the number of your choice: ";

        // Get valid choice
        int choice = -1;
        while (true) {
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
        for (size_t j = 0; j < availableColors.size(); ++j) {
            if (std::find(usedColors.begin(), usedColors.end(), availableColors[j].second) == usedColors.end()) {
                std::cout << j << ": " 
                          << colorCodes[j] << p.symbol << RESET_COLOR 
                          << " (" << availableColors[j].first << ")  ";
            }
        }
        std::cout << "\nEnter the number of your choice: ";

        // Get valid choice
        int colorChoice = -1;
        while (true) {
            std::cin >> colorChoice;
            if (colorChoice >= 0 && colorChoice < (int)availableColors.size() &&
                std::find(usedColors.begin(), usedColors.end(), availableColors[colorChoice].second) == usedColors.end()) {
                break;
            }
            std::cout << "Invalid choice, please pick another: ";
        }

        // Assign and mark as used
        ColorGroup playerColor = availableColors[colorChoice].second; // .second to get ColorGroup from pair (mapping)
        p.color = playerColor;
        usedColors.push_back(playerColor);
        // usage of colorCodes vector to get ANSI code, then reset after symbol
        std::cout << "You chose color: " << colorCodes[colorChoice] << p.symbol << RESET_COLOR << " (" << availableColors[colorChoice].first << ")\n";

        // Add player to the list
        players.push_back(p);
    }

    //Randomize Player Order
    random_shuffle(players.begin(), players.end());

    //Display Gameboard
    displayGameBoard(players, gameBoard);

    //start of Gameloop :)
    int sel;
    do{
        for(player &currentPlayer : players){
            bool control = false;
            bool rolled = false;
            int count = 0;
            do{
                control = action(sel,currentPlayer,count,rolled);
            }while(sel && control || sel != 10);
        }
    }while(sel != 10);

    return 0;
}