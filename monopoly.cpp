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

void clearInputBuffer() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void clearTerminal() {
    fputs("\x1b[1;1H\x1b[2J\x1b[3J", stdout);
    fflush(stdout);
}

//Global Variables
const std::string RESET_COLOR = "\033[0m";
#include "player.hpp"
#include "tile.hpp"
#include "card.hpp"
std::vector<tile> gameBoard;
std::vector<player> players;
std::vector<card> communityCards;
std::vector<card> chanceCards;
int freeParkingFunds = 0;
int communityCardCounter = 0;
int chanceCardCounter = 0;
void movePlayer(int s, player &p, bool &ok, std::string message);
void bankruptcy(player &p, int targetID, int amount);

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

        // Place owned / houses / hotel status
        size_t posStatus = board.find(placeholderOwned);
        if (posStatus != std::string::npos) {
            if (t.ownerId == -1) {
                ownedStatus = "     ";
            } else {
                std::string color = colorCodes[players[t.ownerId].color].first;
                if (t.isMortgaged) {
                    ownedStatus = color + "MORTG" + RESET_COLOR;
                } else {
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

// calculate rent for utilities based on rolled dice value and owned utilities
int calculateUtilityRent(tile& currentTile, int diceRoll)
{
    int utilitiesOwned = 0;
    for (const auto& t : gameBoard) {
        if (t.ownerId == currentTile.ownerId && (t.tileIndex == 12 || t.tileIndex == 28)) {
            utilitiesOwned++;
        }
    }
    if (utilitiesOwned == 1)
        return diceRoll * 4;
    if (utilitiesOwned == 2)
        return diceRoll * 10;
    return 0;
}

// calculate rent for railroads based on number of owned railroads
int calculateRailroadRent(tile& currentTile)
{
    int railroadsOwned = 0;
    for (const auto& t : gameBoard) {
        if (t.ownerId == currentTile.ownerId && (t.tileIndex == 5 || t.tileIndex == 15 || t.tileIndex == 25 || t.tileIndex == 35)) {
            railroadsOwned++;
        }
    }
    switch (railroadsOwned) {
        case 1: return 25;
        case 2: return 50;
        case 3: return 100;
        case 4: return 200;
        default: return 0;
    }
}

// check if player owns all properties in the color group
bool ownsMonopoly(tile& currentTile)
{
    for (const auto& t : gameBoard) {
        if (t.color == currentTile.color && t.ownerId != currentTile.ownerId) {
            return false;
        }
    }
    return true;
}

// calculate rent for properties based on upgrade stage and monopoly status
int calculatePropertyRent(tile& t)
{
    if (t.isMortgaged) {
        return 0;
    }
    if (t.upgradeStage == 0)
        return ownsMonopoly(t) ? t.price0 * 2 : t.price0;

    switch (t.upgradeStage) {
        case 1: return t.price1;
        case 2: return t.price2;
        case 3: return t.price3;
        case 4: return t.price4;
        case 5: return t.price5; // hotel
        default: return 0;
    }
}

// roll two six-sided dice
int rollDice(){
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1,6);
    
    return dis(gen);
}

// check if both dice have the same value
bool checkPasch(int &x, int &y){
    if(x != y){
        return false;
    }else{
        return true;
    }
}

// arrest player and move to jail
void arrest(player &p, bool &ok){
    p.jailed = true;
    p.currentPosition = 10; //Jail Position
    ok = true;
    displayGameBoard();
    std::cout<<"You have been arrested! 😡"<<std::endl;
}

// visual representation of dice roll for output in the Terminal
std::string visualDice(int &x){
    switch (x) {
        case 1:{
            return "⚀ 1 ";
        }
        case 2:{
            return "⚁ 2 ";
        }
        case 3:{
            return "⚂ 3 ";
        }
        case 4:{
            return "⚃ 4 ";
        }
        case 5:{
            return "⚄ 5 ";
        }
        case 6:{
            return "⚅ 6 ";
        }
        default:{
            return "Invalid Dice Value";
        }
    }
}

void transferTile(player &from, int targetID, std::vector<int> tiles){

}

// transfer money between players
void transferMoney(player &from, int targetID, int amount){
    // implement bancruptcy logic
    // implement logic to handle transfer from/to bank/free parking?
    if (from.money < amount) {
        bankruptcy(from, targetID, amount);
    }
    if (std::find_if(players.begin(), players.end(),[&from](player& pl) {
        return &pl == &from;
    }) != players.end())
    {
        from.money -= amount;
    }
    if (targetID == -1) {
        freeParkingFunds += amount;
        return;
    }
    players[targetID].money = players[targetID].money + amount;
}

// draw a card from the specified deck and apply its effects
void drawCard(std::string type, player& player, bool& ok) {
    card* currentCard = nullptr;

    // Get current Card, shuffle if every card was used once, tracked per deck
    if (type == "chance") {
        if (chanceCardCounter == chanceCards.size()) {
            chanceCardCounter = 0;
            std::random_shuffle(chanceCards.begin(), chanceCards.end());
        }
        currentCard = &chanceCards[chanceCardCounter++];
    } 
    else if (type == "community") {
        if (communityCardCounter == communityCards.size()) {
            communityCardCounter = 0;
            std::random_shuffle(communityCards.begin(), communityCards.end());
        }
        currentCard = &communityCards[communityCardCounter++];
    }

    // Safety check to see if a card was drawn, crashes program since not handled by design
    if (!currentCard) {
        throw std::runtime_error("No card drawn!");
    }

    // Display drawn card
    std::cout << "You drew a " << type << " card: " << currentCard->text << ".\nPress enter to continue..." << std::endl;
    std::cin.get();
    clearInputBuffer();

    // Apply card effects
    // receive money from bank
    if (currentCard->action == "receive") {
        player.money += std::stoi(currentCard->value.at("amount"));
    } 
    // pay money to bank
    else if (currentCard->action == "pay") {
        transferMoney(player, -1, std::stoi(currentCard->value.at("amount")));
        freeParkingFunds += std::stoi(currentCard->value.at("amount"));
    } 
    // move player to specified position
    else if (currentCard->action == "move") {
        if (currentCard->value.at("position") == "-3") {
            movePlayer(std::stoi(currentCard->value.at("position")), player, ok, "You moved 3 spaces back!");
        } 
        else {
            movePlayer(
                (std::stoi(currentCard->value.at("position")) - player.currentPosition + 40) % 40, 
                player, 
                ok, 
                "You moved to position " + gameBoard[std::stoi(currentCard->value.at("position"))].tileName + "!"
            );
        }
    } 
    // give player a Get Out of Jail Free card
    else if (currentCard->action == "jailFree") {
        player.jailFreeCard += 1;
    } 
    // send player to jail
    else if (currentCard->action == "jail") {
        arrest(player, ok);
    } 
    // receive money from all other players
    else if (currentCard->action == "receiveFromPlayers") {
        int totalAmount = 0;
        for (auto &p : players) {
            if (p.playerId != player.playerId) {
                int amt = std::stoi(currentCard->value.at("amount"));
                totalAmount += amt;
                transferMoney(p, player.playerId, amt);
            }
        }
    } 
    // pay money to all other players
    else if (currentCard->action == "payEach") {
        int totalAmount = 0;
        for (auto &p : players) {
            if (p.playerId != player.playerId) {
                int amt = std::stoi(currentCard->value.at("amount"));
                totalAmount += amt;
            }
        }
        int playerID = player.playerId;
        transferMoney(player, -1, totalAmount);

        // duplicate loop to avoid money transfer before deduction and possible bankruptcy
        for (auto &p : players) {
            if (p.playerId != playerID) {
                int amt = std::stoi(currentCard->value.at("amount"));
                p.money += amt;
            }
        }
    } 
    // move to nearest utility or railroad
    else if (currentCard->action == "moveNearest") {
        if (currentCard->value.at("destination") == "railroad") {
            int distances[] = {5, 15, 25, 35};
            int minDistance = 40;
            for (int d : distances) {
                int distance = (d - player.currentPosition + 40) % 40;
                if (distance < minDistance) minDistance = distance;
            }
            movePlayer(minDistance, player, ok, "You moved to the nearest Railroad!");
        } 
        else if (currentCard->value.at("destination") == "utility") {
            int distances[] = {12, 28};
            int minDistance = 40;
            for (int d : distances) {
                int distance = (d - player.currentPosition + 40) % 40;
                if (distance < minDistance) minDistance = distance;
            }
            movePlayer(minDistance, player, ok, "You moved to the nearest Utility!");
        }
    } 
    // pay for repairs based on owned properties
    else if (currentCard->action == "repairTax") {
        int totalHouses = 0;
        int totalHotels = 0;
        for (const auto& t : gameBoard) {
            if (t.ownerId == player.playerId) {
                if (t.upgradeStage >= 1 && t.upgradeStage <= 4) totalHouses += t.upgradeStage;
                else if (t.upgradeStage == 5) totalHotels += 1;
            }
        }
        int amountDue = (totalHouses * std::stoi(currentCard->value.at("perHouse"))) + 
                        (totalHotels * std::stoi(currentCard->value.at("perHotel")));
        transferMoney(player, -1, amountDue);
        freeParkingFunds += amountDue;
    }
}

// move player by s spaces and handle landing on different tile types
void movePlayer(int s, player &p, bool &ok, std::string message){
    if(p.currentPosition+s > 40){
        p.money += 200;
    }
    p.currentPosition = (p.currentPosition+s)%40;
    displayGameBoard();
    std::cout<<message<<std::endl;
    switch (p.currentPosition){
        case 0:{ //Tiletyp: GO
            p.money += 400;
            break;
        }
        case 2:{ //Tiletyp: Com Chest
            drawCard("community", p, ok);
            break;
        }
        case 4:{ //Tiletyp: Income Tax
            transferMoney(p, -1, 200);
            break;
        }
        case 7:{ //Tiletyp: Chance
            drawCard("chance", p, ok);
            break;
        }
        case 10:{ //Tiletyp: visit Jail
            break;
        }
        case 17:{ //Tiletyp: Com Chest
            drawCard("community", p, ok);
            break;
        }
        case 20:{ //Tiletyp: FreeParking
            p.money += freeParkingFunds;
            freeParkingFunds = 0;
            break;
        }
        case 22:{ //Tiletyp: Chance
            drawCard("chance", p, ok);
            break;
        }
        case 30:{ //Tiletyp: GoToJail
            std::cout << "You have come to the wroong neighbourhood my friend! Now the coppers will get you and take you to Jail!\nPress enter to continue..." << std::endl;
            std::cin.get();
            clearInputBuffer();
            arrest(p,ok);
            break;
        }
        case 33:{ //Tiletyp: Com Chest
            drawCard("community", p, ok);
            break;
        }
        case 36:{ //Tiletyp: Chance
            drawCard("chance", p, ok);
            break;
        }
        case 38:{ //Tiletyp: Luxury Tax
            transferMoney(p, -1, 100);
            break;
        }
        default:{ //Tiletyp: Streets, Trainstations, Facilities
            tile& currentfield = gameBoard[p.currentPosition];
            if( currentfield.ownerId == -1){ // unowned property
                if(p.money >= currentfield.buyPrice){
                    bool correct = false;
                    while(!correct){
                        std::cout<<colorCodes[p.color].first << p.symbol << " " << p.name << RESET_COLOR 
                        << " Turn! Do you want to buy " << currentfield.tileName << " for " << currentfield.buyPrice << " ?"<<std::endl;
                        std::cout<<"------------------\n"
                        <<"| 1: YES | "
                        <<"0: NO |\n"
                        <<"------------------\n"
                        <<std::endl;
                        int sel;
                        std::cin>>sel;
                        switch (sel) {
                            case 1:{
                                transferMoney(p, -1, currentfield.buyPrice);
                                currentfield.ownerId = p.playerId;
                                p.ownedStreets.push_back(currentfield.tileIndex);
                                displayGameBoard();
                                correct = true;
                                std::cout<<"Bravo you have succesfully puchased "<< currentfield.tileName <<"! 🥳" <<std::endl;
                                break;
                            }
                            case 0:{
                                displayGameBoard();
                                std::cout<<"Why tho? 🤨 "<<std::endl;
                                correct = true;
                                break;
                            }
                            default:{
                                displayGameBoard();
                                std::cout<<"No valid input! 😡"<<std::endl;
                            }
                        }
                    }
                }else{
                    std::cout<<"Not enough Money. 😢"<<std::endl;
                }
            }else if (players[currentfield.ownerId].playerId == p.playerId) { // own property
                std::cout<< "Lucky, you landed on your own tile 🍀" <<std::endl;
            }else{ // pay rent
                int rentPayment;
                if (currentfield.tileIndex == 12 || currentfield.tileIndex == 28) { // Utility
                    rentPayment = calculateUtilityRent(currentfield, s);
                } else if (currentfield.tileIndex == 5 || currentfield.tileIndex == 15 || currentfield.tileIndex == 25 || currentfield.tileIndex == 35) { // Railroad
                    rentPayment = calculateRailroadRent(currentfield);

                } else { // Property
                    rentPayment = calculatePropertyRent(currentfield);
                }
                // transfer the money and check for bankruptcy
                transferMoney(p, currentfield.ownerId, rentPayment);
                std::cout<<"You had to pay " << rentPayment << " to " << players[currentfield.ownerId].name << " 💵" <<std::endl;
            }
            break;
        }
    }
}

// Main function for the GameLoop when the player is in Jail, with a selection for the possible actions in Monopoly
bool jailedaction(int &sel, player &p, int &diceRolls, bool &ok){
    if (p.jailFreeCard > 0) {
        std::cout<<colorCodes[p.color].first + p.symbol + " " + p.name + RESET_COLOR + ", it's your turn!\n"<<"You have " <<p.money<<"$ in your account.\n"
        <<"What do you want to do? \n"
        <<"----------------------------------------------------------------------------------------------------\n"
        <<"| 1 = Roll the dices | "
        <<"2 = Buy you out (50$) | "
        <<"3 = Play a get out of jail card | "
        <<"0 = End your turn | "
        <<"77 = Quit the whole game early |\n"
        <<"----------------------------------------------------------------------------------------------------"
        <<std::endl;
    } else {
        std::cout<<colorCodes[p.color].first + p.symbol + " " + p.name + RESET_COLOR + ", it's your turn!\n"<<"You have " <<p.money<<"$ in your account.\n"
        <<"What do you want to do? \n"
        <<"-------------------------------------------------------------------\n"
        <<"| 1 = Roll the dices | "
        <<"2 = Buy you out (50$) | "
        <<"0 = End your turn | "
        <<"77 = Quit the whole game early |\n"
        <<"-------------------------------------------------------------------"
        <<std::endl;
    }
    std::cin>>sel;
    switch (sel) {
        case 0:{
            displayGameBoard();
            if(!ok){
                std::cout<<"You haven't rolled enough dice! 😡"<<std::endl;
            }
            return ok;
        }
        case 1:{
            if(!ok){
                int x = rollDice();
                int y = rollDice();
                ok = true;
                if(p.jailCounter < 3){
                    p.jailCounter++;
                    if(checkPasch(x, y)){
                        p.jailed = false;
                        p.jailCounter = 0;
                        diceRolls++;
                        movePlayer(x+y,p,ok,visualDice(x)+"+ "+visualDice(y));
                    }else{
                        displayGameBoard();
                        visualDice(x);
                        visualDice(y);
                        std::cout<<"No doubles no FREEDOM! 🦅"<<std::endl;
                    }
                }else{
                    p.jailed = false;
                    p.jailCounter = 0;
                    transferMoney(p,-1, 50);
                    diceRolls++;
                    movePlayer(x+y, p,ok,visualDice(x)+"+ "+visualDice(y));
                    std::cout<< "FREEDOM is not FREE! 🦅" <<std::endl;
                }
            }else{
                displayGameBoard();
                std::cout<<"You have already rolled enough dice! 😡"<<std::endl;
            }
            return false;
        }
        case 2:{
            int x = rollDice();
            int y = rollDice();
            ok = true;
            p.jailed = false;
            p.jailCounter = 0;
            transferMoney(p, -1, 50);
            diceRolls++;
            movePlayer(x+y,p,ok,visualDice(x)+"+ "+visualDice(y));
            std::cout<< "FREEDOM is not FREE! 🦅" <<std::endl;
            return false;
        }
        case 3:{
            if (p.jailFreeCard <= 0) {
                sel = -1;
                displayGameBoard();
                std::cout<<"No valid input! 😡"<<std::endl;
                clearInputBuffer();
                return false;
            }
            p.jailFreeCard--;
            int x = rollDice();
            int y = rollDice();
            ok = true;
            p.jailed = false;
            p.jailCounter = 0;
            diceRolls++;
            movePlayer(x+y,p,ok,visualDice(x)+"+ "+visualDice(y));
            std::cout<< "FREEDOM is FREE? 😫" <<std::endl;
            return false;
        }
        case 77:{
            return true;
        }
        default:{
            sel = -1;
            displayGameBoard();
            std::cout<<"No valid input! 😡"<<std::endl;
            clearInputBuffer();
            return false;
        }
    }
}

bool financial_menue(player &p){
    int sel;
    displayGameBoard();
    std::vector<tile> filteredTileListPlayer;
    std::cout<<colorCodes[p.color].first << p.symbol << " " << p.name << RESET_COLOR << ", welcome to the financial menue\n"<<"You have " <<p.money<<"$ in your account.\n"
    <<"What do you want to do? \n"
    <<"--------------------------------------------------------------\n"
    <<"| 1 = mortgage your cards | "
    <<"2 = unmortgage your cards | "
    <<"0 = go back |\n"
    <<"--------------------------------------------------------------"
    <<std::endl;
    std::cin>>sel;
    switch (sel) {
        case 1:{
            if(!p.ownedStreets.empty()){
                for(int i : p.ownedStreets){
                    if(!gameBoard[i].isMortgaged && gameBoard[i].upgradeStage == 0){
                        filteredTileListPlayer.push_back(gameBoard[i]);
                    }
                }
                do{
                    displayGameBoard();
                    if(!filteredTileListPlayer.empty()){
                        std::cout<<colorCodes[p.color].first << p.symbol << " " << p.name << RESET_COLOR << "here are the cards you can mortgage:" <<std::endl;
                        int i = 0;
                        for(tile t : filteredTileListPlayer){
                            std::cout<< i << " | " << t.tileName <<std::endl;
                            i++;
                        }
                        std::cout<< "99 | Finished in this menue" <<std::endl;
                        std::cout<<"Wich do you choose?"<<std::endl;
                        std::cin>>sel;
                        if(sel !=99){
                            p.money += (0.5*filteredTileListPlayer[sel].buyPrice);
                            gameBoard[filteredTileListPlayer[sel].tileIndex].isMortgaged = true;
                            filteredTileListPlayer.erase(filteredTileListPlayer.begin() + sel);
                        }
                    }else{
                        std::cout<<"All your cards are mortgaged! 🏚"<<std::endl;
                        break;
                    }
                }while(sel != 99);
                if(sel ==99){
                    displayGameBoard();
                }
            }else{
                displayGameBoard();
                std::cout<<"You are homeless! 🏚"<<std::endl;
            }
            break;
        }
        case 2:{
            if(p.ownedStreets.size()){
                for(int i : p.ownedStreets){
                    if(gameBoard[i].isMortgaged){
                        filteredTileListPlayer.push_back(gameBoard[i]);
                    }
                }
                do{
                    displayGameBoard();
                    if(!filteredTileListPlayer.empty()){
                        std::cout<<colorCodes[p.color].first << p.symbol << " " << p.name << RESET_COLOR << "here are the cards you can unmortgage:" <<std::endl;
                        int i = 0;
                        for(tile t : filteredTileListPlayer){
                            std::cout<< "| " << i << " | " << t.tileName << " |" <<std::endl;
                            i++;
                        }
                        std::cout<< "99 | Finished in this menue" <<std::endl;
                        std::cout<<"Wich do you choose?"<<std::endl;
                        std::cin>>sel;
                        if(sel !=99){
                            transferMoney(p, -1, (0.55*filteredTileListPlayer[sel].buyPrice));
                            gameBoard[filteredTileListPlayer[sel].tileIndex].isMortgaged = false;
                            filteredTileListPlayer.erase(filteredTileListPlayer.begin() + sel);
                        }
                    }else{
                        std::cout<<"All your cards are unmortgaged! 🏠"<<std::endl;
                        break;
                    }
                }while(sel != 99);
                if(sel ==99){
                    displayGameBoard();
                }
            }else{
                displayGameBoard();
                std::cout<<"You are homeless! 🏚"<<std::endl;
            }
            break;
        }
        case 0:{
            displayGameBoard();
            std::cout<<"i want to be MONKEY! 🐒"<<std::endl;
            break;
        }
        default:{
            displayGameBoard();
            std::cout<<"No valid input! 😡"<<std::endl;
            break;
        }
    }
    return false;
}

bool building_menue(player &p){
    int sel;
    displayGameBoard();
    std::vector<int> filteredTileListPlayer;
    std::cout<<colorCodes[p.color].first << p.symbol << " " << p.name << RESET_COLOR << ", welcome to the building menue\n"<<"You have " <<p.money<<"$ in your account.\n"
    <<"What do you want to do? \n"
    <<"--------------------------------------------------------------\n"
    <<"| 1 = buy houses | "
    <<"2 = sell houses | "
    <<"0 = go back |\n"
    <<"--------------------------------------------------------------"
    <<std::endl;
    std::cin>>sel;
    switch (sel) {
        case 1:{
            if(!p.ownedStreets.empty()){
                for(int i : p.ownedStreets){
                    if(ownsMonopoly(gameBoard[i])){
                        if(gameBoard[i].upgradeStage != 5){
                            filteredTileListPlayer.push_back(gameBoard[i].tileIndex);
                        }
                    }
                }
                if(!filteredTileListPlayer.empty()){
                    std::sort(filteredTileListPlayer.begin(),filteredTileListPlayer.end());
                    do{
                        displayGameBoard();
                        if(!filteredTileListPlayer.empty()){
                            std::cout<<colorCodes[p.color].first << p.symbol << " " << p.name << RESET_COLOR << "here are the cards you can upgrade:" <<std::endl;
                            int i = 0;
                            for(int t : filteredTileListPlayer){
                                std::cout<<colorCodes[gameBoard[t].color].first << "| " << i << " |" <<RESET_COLOR << " " << gameBoard[t].tileName << " |" <<std::endl;
                                i++;
                            }
                            std::cout<< "99 | Finished in this menue" <<std::endl;
                            std::cout<<"Wich do you choose?"<<std::endl;
                            std::cin>>sel;
                            if(sel !=99 && sel < filteredTileListPlayer.size()){
                                transferMoney(p, -1, gameBoard[filteredTileListPlayer[sel]].housePrice);
                                gameBoard[filteredTileListPlayer[sel]].upgradeStage++;
                                if(gameBoard[filteredTileListPlayer[sel]].upgradeStage == 5){
                                    filteredTileListPlayer.erase(filteredTileListPlayer.begin() + sel);
                                }
                            }
                        }else{
                            std::cout<<"All your cards max upgraded! 🏠"<<std::endl;
                            break;
                        }
                    }while(sel != 99);
                    if(sel ==99){
                        displayGameBoard();
                    }
                }else{
                    displayGameBoard();
                    std::cout<<"No monopolys! 🏚"<<std::endl;
                }
            }else{
                displayGameBoard();
                std::cout<<"You are homeless! 🏚"<<std::endl;
            }
            break;
        }case 2:{
            if(!p.ownedStreets.empty()){
                for(int i : p.ownedStreets){
                    if(ownsMonopoly(gameBoard[i])){
                        if(gameBoard[i].upgradeStage != 0){
                            filteredTileListPlayer.push_back(gameBoard[i].tileIndex);
                        }
                    }
                }
                if(!filteredTileListPlayer.empty()){
                    std::sort(filteredTileListPlayer.begin(),filteredTileListPlayer.end());
                    do{
                        displayGameBoard();
                        if(!filteredTileListPlayer.empty()){
                            std::cout<<colorCodes[p.color].first << p.symbol << " " << p.name << RESET_COLOR << "here are the cards you can upgrade:" <<std::endl;
                            int i = 0;
                            for(int t : filteredTileListPlayer){
                                std::cout<<colorCodes[gameBoard[t].color].first << "| " << i << " |" <<RESET_COLOR << " " << gameBoard[t].tileName << " |" <<std::endl;
                                i++;
                            }
                            std::cout<< "99 | Finished in this menue" <<std::endl;
                            std::cout<<"Wich do you choose?"<<std::endl;
                            std::cin>>sel;
                            if(sel !=99 && sel < filteredTileListPlayer.size()){
                                p.money += (0.5*gameBoard[filteredTileListPlayer[sel]].housePrice);
                                gameBoard[filteredTileListPlayer[sel]].upgradeStage--;
                                if(gameBoard[filteredTileListPlayer[sel]].upgradeStage == 0){
                                    filteredTileListPlayer.erase(filteredTileListPlayer.begin() + sel);
                                }
                            }
                        }else{
                            std::cout<<"All your cards are max downgraded! 🏚"<<std::endl;
                            break;
                        }
                    }while(sel != 99);
                    if(sel ==99){
                        displayGameBoard();
                    }
                }else{
                    displayGameBoard();
                    std::cout<<"No monopolys! 🏚"<<std::endl;
                }
            }else{
                displayGameBoard();
                std::cout<<"You are homeless! 🏚"<<std::endl;
            }
            break;
        }case 0:{
            displayGameBoard();
            std::cout<<"i want to be MONKEY! 🐒"<<std::endl;
            break;
        }
        default:{
            displayGameBoard();
            std::cout<<"No valid input! 😡"<<std::endl;
            break;
        }
    }
    return false;
}

bool trading_menue(player &p){
    int sel;
    std::vector<int> otherplayers;
    displayGameBoard();
    std::vector<tile> filteredTileListPlayer;
    std::cout<<colorCodes[p.color].first << p.symbol << " " << p.name << RESET_COLOR << ", welcome to the traiding menue\n"
    <<"Whit whom you want to trade?\n"
    <<"--------------------------------------------------------------"<<std::endl;
    int i = 0;
    for(player pl : players){
        if(pl.playerId != p.playerId){
            otherplayers.push_back(pl.playerId);
            std::cout<<"| " << i << " | " << pl.name << " |\n";
        }
    }
    std::cout<<"| 99 | go back |"
    <<"\n--------------------------------------------------------------"
    <<std::endl;
    std::cin>>sel;
    if(sel < otherplayers.size()){
        int amount1;
        int amount2;
        std::vector<int> tiles1;
        std::vector<int>tiles2;

        displayGameBoard();
        std::cout<<colorCodes[p.color].first << p.symbol << " " << p.name << RESET_COLOR <<"You have " <<p.money<<"$ in your account.\n"
        <<"How Much money do you want to give " << players[otherplayers[sel]].name << "?" <<std::endl;
        std::cin>>amount1;

        if(!p.ownedStreets.empty()){
            int tilesel;
            do{
                displayGameBoard();
                std::cout<<colorCodes[p.color].first << p.symbol << " " << p.name << RESET_COLOR << "here are the cards you can trade:" <<std::endl;
                int i = 0;
                for(int t : p.ownedStreets){
                    std::cout<<colorCodes[gameBoard[t].color].first << "| " << i << " |" <<RESET_COLOR << " " << gameBoard[t].tileName << " |" <<std::endl;
                    i++;
                }
                std::cout<< "99 | Finished in this menue" <<std::endl;
                std::cout<<"Wich do you choose?"<<std::endl;
                std::cin>>tilesel;
                if(sel !=99 && sel < p.ownedStreets.size()){
                    tiles1.push_back(p.ownedStreets[tilesel]);
                }
            }while(tilesel != 99);
        }else{
            displayGameBoard();
            std::cout<<"You have no cards to give\n"<<"press enter to continue..."<<std::endl;
            std::cin.get();
            clearInputBuffer();
        }

        displayGameBoard();
        std::cout<<colorCodes[players[otherplayers[sel]].color].first << p.symbol << " " << p.name << RESET_COLOR <<"has " <<p.money<<"$ in their account.\n"
        <<"How Much money do you want from " << players[otherplayers[sel]].name << "?" <<std::endl;
        std::cin>>amount2;

        displayGameBoard();
        if(!players[otherplayers[sel]].ownedStreets.empty()){
            int tilesel;
            do{
                displayGameBoard();
                std::cout<<colorCodes[p.color].first << p.symbol << " " << p.name << RESET_COLOR << "here are the cards you can trade:" <<std::endl;
                int i = 0;
                for(int t : players[otherplayers[sel]].ownedStreets){
                    std::cout<<colorCodes[gameBoard[t].color].first << "| " << i << " |" <<RESET_COLOR << " " << gameBoard[t].tileName << " |" <<std::endl;
                    i++;
                }
                std::cout<< "99 | Finished in this menue" <<std::endl;
                std::cout<<"Wich do you choose?"<<std::endl;
                std::cin>>tilesel;
                if(sel !=99 && sel < players[otherplayers[sel]].ownedStreets.size()){
                    tiles2.push_back(players[otherplayers[sel]].ownedStreets[tilesel]);
                }
            }while(tilesel != 99);
        }else{
            std::cout<<players[otherplayers[sel]].name << " has no cards to give\n"<<"press enter to continue..."<<std::endl;
            std::cin.get();
            clearInputBuffer();
        }

        int desicion;
        do{
            displayGameBoard();
            std::cout<<colorCodes[players[otherplayers[sel]].color].first << players[otherplayers[sel]].symbol << " " << players[otherplayers[sel]].name << RESET_COLOR << "do you accept the trade?"
            <<"---------------------------------\n"
            <<"| 1 = Yes | "
            <<"0 = No |\n"
            <<"---------------------------------"
            <<std::endl;
            std::cin>>desicion;
        }while (!desicion || desicion == 1);
        if(!desicion){
            displayGameBoard();
            std::cout<<"They didn't want your offer! 😡"<<std::endl;
        }else{
            transferMoney(p, otherplayers[sel], amount1);
            transferMoney(players[otherplayers[sel]], p.playerId, amount2);

            transferTile(p, otherplayers[sel], tiles1);
            transferTile(players[otherplayers[sel]], p.playerId, tiles2);
            displayGameBoard();
            std::cout<<"The offer was accepted! 🥳"<<std::endl;
        }
    }else if (sel == 99) {
        displayGameBoard();
        std::cout<<"i want to be MONKEY! 🐒"<<std::endl;
    }else{
        displayGameBoard();
        std::cout << "Invalid input! 😡" << std::endl;
    }
    return false;
}

void bankruptcy(player &p, int targetID, int amount){
    std::cout << colorCodes[p.color].first << p.symbol << " " << p.name << RESET_COLOR << ", you are facing BANKRUPTCY! 😵‍💫\nPress enter to continue dealing with it..." << std::endl;
    std::cin.get();
    clearInputBuffer();
    int sel = 0;
    while (sel != 99) {
        displayGameBoard();
        std::cout << "You owe " << amount << "$.\n"
                  << "You have " << p.money << "$.\n"
                  << "Choose an action to raise funds:\n"
                  << "1: Mortgage properties\n"
                  << "2: Sell houses\n"
                  << "99: Declare bankruptcy\n"
                  << "Your choice: ";
        std::cin >> sel;
        switch (sel) {
            case 1:
                financial_menue(p);
                break;
            case 2:
                building_menue(p);
                break;
            case 99:
                break;
            default:
                std::cout << "Invalid input! 😡" << std::endl;
                clearInputBuffer();
                continue;
        }
        if (p.money >= amount) {
            std::cout << "You have raised enough funds to pay your debts! This was a close one! Press enter to continue..." << std::endl;
            p.money -= amount;
            std::cin.get();
            clearInputBuffer();
            return;
        } else {
            std::cout << "You still need more funds to pay your debts." << std::endl;
        }
    }
    if (amount > p.money) {
        std::cout << "You do not have enough money to pay your debts." << std::endl;
        if (targetID != -1) {
            player& targetPlayer = players[targetID];
            std::cout << "You owe " << colorCodes[targetPlayer.color].first << targetPlayer.symbol << " " << targetPlayer.name << RESET_COLOR << " money." << std::endl;
            // Transfer all properties to the target player
            for (int tileIndex : p.ownedStreets) {
                gameBoard[tileIndex].ownerId = targetPlayer.playerId;
                gameBoard[tileIndex].isMortgaged = false;
                targetPlayer.jailFreeCard += p.jailFreeCard;
                targetPlayer.ownedStreets.push_back(tileIndex);
            }
        } else {
            std::cout << "You owe the bank money." << std::endl;
            // Transfer all properties to the bank
            for (int tileIndex : p.ownedStreets) {
                gameBoard[tileIndex].ownerId = -1;
                gameBoard[tileIndex].isMortgaged = false;
                gameBoard[tileIndex].upgradeStage = 0;
            }
        }
        p.ownedStreets.clear();
        p.money = 0;
        std::cout << colorCodes[p.color].first << p.symbol << " " << p.name << RESET_COLOR << ", you have been removed from the game. 😞" << std::endl;
        players.erase(std::remove_if(players.begin(), players.end(),
            [&p](const player& pl) { return pl.playerId == p.playerId; }), players.end());
    } else {
        std::cout << "You have successfully paid off your debts!\nPress enter to continue..." << std::endl;
        std::cin.get();
        clearInputBuffer();
    }
}

// Main function for the GameLoop, with a selection for the possible actions in Monopoly
bool normalaction(int &sel, player &p, int &diceRolls, bool &ok){
    std::cout<<colorCodes[p.color].first + p.symbol + " " + p.name + RESET_COLOR + ", it's your turn!\n"<<"You have " <<p.money<<"$ in your account.\n"
    <<"What do you want to do? \n"
    <<"--------------------------------------------------------------------------------------------------------------------------------------------\n"
    <<"| 1 = Roll the dices | "
    <<"2 = Financial Menue | "
    <<"3 = Building Menue | "
    <<"4 = Trade with player |"
    <<"0 = End your turn | "
    <<"77 = Quit the whole game early |\n"
    <<"--------------------------------------------------------------------------------------------------------------------------------------------"
    <<std::endl;
    std::cin>>sel;
    switch (sel) { //Main action
        case 0:{ //
            displayGameBoard();
            if(!ok){
                std::cout<<"You haven't rolled enough dice! 😡"<<std::endl;
            }
            return ok;
        }case 1:{
            if(!diceRolls || !ok){
                diceRolls++;
                int x = rollDice();
                int y = rollDice();
                movePlayer(x+y,p,ok,visualDice(x)+"+ "+visualDice(y));
                if(diceRolls == 3 && checkPasch(x,y)){
                    std::cout << "You have rolled too many doubles, now you have to go to jail!\nPress enter to continue..." << std::endl;
                    std::cin.get();
                    clearInputBuffer();
                    arrest(p, ok);
                    sel = 0;
                    return true;
                }
                if(!checkPasch(x, y)){
                    ok = true;
                }else{
                    std::cout<<"You rolled doubles! You can roll again!"<<std::endl;
                    ok = false;
                }
            }else{
                displayGameBoard();
                std::cout<<"You have already rolled enough dice! 😡"<<std::endl;
            }
            return false;
        }case 2:{
            return financial_menue(p);
        }
        case 3:{
            return building_menue(p);
        }
        case 4:{
            return trading_menue(p);
        }
        case 77:{
            return true;
        }
        default:{
            sel = -1;
            displayGameBoard();
            std::cout<<"No valid input! 😡"<<std::endl;
            clearInputBuffer();
            return false;
        }
    }
}

// Main function to initialize and run the Monopoly game loop
int main(){
    // Initialize Gameboard
    gameBoard = initializeGameBoard();
    chanceCards = initializeChanceCards();
    communityCards = initializeCommunityCards();
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
            int diceRolls = 0;
            do{
                if(!currentPlayer.jailed){
                    control = normalaction(sel,currentPlayer,diceRolls,rolled);
                    if(sel == 77){
                        break;
                    }else if(!control){
                        sel = -1;
                    }
                }else{
                    control = jailedaction(sel,currentPlayer,diceRolls,rolled);
                    if(sel == 77){
                        break;
                    }else if(!control){
                        sel = -1;
                    }
                }
            }while(sel);
            if(sel == 77){
                break;
            }
        }
    }while(sel != 77 && players.size() > 1);

    if (sel != 77 && players.size() == 1) {
        std::cout << colorCodes[players[0].color].first << players[0].symbol << " " << players[0].name << RESET_COLOR << " is the WINNER! 🏆🎉" << std::endl;
    } else {
        std::cout << "Game ended early." << std::endl;
    }
    return 0;
}