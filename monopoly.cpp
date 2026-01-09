#include <iostream>
#include <limits>
#include <random>
#include <string>
#include <vector>

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


void initizialeGameBoard(tile* gameBoard) {
    /*
    gameBoard[0].tileName = "GO";
    gameBoard[0].shortName = "GO";
    gameBoard[0].buyable = false;

    gameBoard[1].tileName = "Mediterranean Ave";
    gameBoard[1].shortName = "MED AVE";
    gameBoard[1].buyable = true;

    gameBoard[2].tileName = "Community Chest";
    gameBoard[2].shortName = "CC";
    gameBoard[2].buyable = false;

    gameBoard[3].tileName = "Baltic Ave";
    gameBoard[3].shortName = "BALTIC";
    gameBoard[3].buyable = true;

    gameBoard[4].tileName = "Income Tax";
    gameBoard[4].shortName = "INCOME";
    gameBoard[4].buyable = false;

    gameBoard[5].tileName = "Reading Railroad";
    gameBoard[5].shortName = "READ RR";
    gameBoard[5].buyable = true;

    gameBoard[6].tileName = "Oriental Ave";
    gameBoard[6].shortName = "ORIENT";
    gameBoard[6].buyable = true;

    gameBoard[7].tileName = "Chance";
    gameBoard[7].shortName = "CHANCE";
    gameBoard[7].buyable = false;

    gameBoard[8].tileName = "Vermont Ave";
    gameBoard[8].shortName = "VERMONT";
    gameBoard[8].buyable = true;

    gameBoard[9].tileName = "Connecticut Ave";
    gameBoard[9].shortName = "CONNET";
    gameBoard[9].buyable = true;

    gameBoard[10].tileName = "Jail";
    gameBoard[10].shortName = "JAIL";
    gameBoard[10].buyable = false;

    gameBoard[11].tileName = "St. Charles Place";
    gameBoard[11].shortName = "ST CHAR";
    gameBoard[11].buyable = true;

    gameBoard[12].tileName = "Electric Company";
    gameBoard[12].shortName = "ELECTRIC";
    gameBoard[12].buyable = true;

    gameBoard[13].tileName = "States Ave";
    gameBoard[13].shortName = "STATES";
    gameBoard[13].buyable = true;

    gameBoard[14].tileName = "Virginia Ave";
    gameBoard[14].shortName = "VIRGINIA";
    gameBoard[14].buyable = true;

    gameBoard[15].tileName = "Pennsylvania Railroad";
    gameBoard[15].shortName = "PA RR";
    gameBoard[15].buyable = true;

    gameBoard[16].tileName = "St. James Place";
    gameBoard[16].shortName = "ST JAMES";
    gameBoard[16].buyable = true;

    gameBoard[17].tileName = "Community Chest";
    gameBoard[17].shortName = "CC";
    gameBoard[17].buyable = false;

    gameBoard[18].tileName = "Tennessee Ave";
    gameBoard[18].shortName = "TENNES";
    gameBoard[18].buyable = true;

    gameBoard[19].tileName = "New York Ave";
    gameBoard[19].shortName = "NEW YORK";
    gameBoard[19].buyable = true;

    gameBoard[20].tileName = "Free Parking";
    gameBoard[20].shortName = "FP";
    gameBoard[20].buyable = false;

    gameBoard[21].tileName = "Kentucky Ave";
    gameBoard[21].shortName = "KENTUCKY";
    gameBoard[21].buyable = true;

    gameBoard[22].tileName = "Chance";
    gameBoard[22].shortName = "CHANCE";
    gameBoard[22].buyable = false;

    gameBoard[23].tileName = "Indiana Ave";
    gameBoard[23].shortName = "INDIANA";
    gameBoard[23].buyable = true;

    gameBoard[24].tileName = "Illinois Ave";
    gameBoard[24].shortName = "ILLINOIS";
    gameBoard[24].buyable = true;

    gameBoard[25].tileName = "B&O Railroad";
    gameBoard[25].shortName = "B&O RR";
    gameBoard[25].buyable = true;

    gameBoard[26].tileName = "Atlantic Ave";
    gameBoard[26].shortName = "ATLANTIC";
    gameBoard[26].buyable = true;

    gameBoard[27].tileName = "Ventnor Ave";
    gameBoard[27].shortName = "VENTNOR";
    gameBoard[27].buyable = true;

    gameBoard[28].tileName = "Water Works";
    gameBoard[28].shortName = "WATER";
    gameBoard[28].buyable = true;

    gameBoard[29].tileName = "Marvin Gardens";
    gameBoard[29].shortName = "MARVIN";
    gameBoard[29].buyable = true;

    gameBoard[30].tileName = "Go To Jail";
    gameBoard[30].shortName = "GOTOJAIL";
    gameBoard[30].buyable = false;

    gameBoard[31].tileName = "Pacific Ave";
    gameBoard[31].shortName = "PACIFIC";
    gameBoard[31].buyable = true;

    gameBoard[32].tileName = "North Carolina Ave";
    gameBoard[32].shortName = "N CAROL";
    gameBoard[32].buyable = true;

    gameBoard[33].tileName = "Community Chest";
    gameBoard[33].shortName = "CC";
    gameBoard[33].buyable = false;

    gameBoard[34].tileName = "Pennsylvania Ave";
    gameBoard[34].shortName = "PENN AV";
    gameBoard[34].buyable = true;

    gameBoard[35].tileName = "Short Line Railroad";
    gameBoard[35].shortName = "SHORT RR";
    gameBoard[35].buyable = true;

    gameBoard[36].tileName = "Chance";
    gameBoard[36].shortName = "CHANCE";
    gameBoard[36].buyable = false;

    gameBoard[37].tileName = "Park Place";
    gameBoard[37].shortName = "PARK PLC";
    gameBoard[37].buyable = true;

    gameBoard[38].tileName = "Luxury Tax";
    gameBoard[38].shortName = "LUXURY";
    gameBoard[38].buyable = false;

    gameBoard[39].tileName = "Boardwalk";
    gameBoard[39].shortName = "BOARD W";
    gameBoard[39].buyable = true;
    */

}

void displayGameBoard(tile* gameBoard){

}

/*
┌───────┬──────────────┬──────────────┬──────────────┬──────────────┬──────────────┬──────────────┬──────────────┬──────────────┬─────────┐
│ 20    │ Kentucky Ave │ Chance       │ Indiana Ave  │ Illinois Ave │ B&O RR       │ Atlantic Ave │ Ventnor Ave  │ Water Works  │ 30      │
│ FP    │ (21)         │ (22)         │ (23)         │ (24)         │ (25)         │ (26)         │ (27)         │ (28)         │ GJ      │
│       │ [    ]       │ [    ]       │ [    ]       │ [    ]       │ [    ]       │ [    ]       │ [    ]       │ [    ]       │         │
├───────┼──────────────┴──────────────┴──────────────┴──────────────┴──────────────┴──────────────┴──────────────┴──────────────┼─────────┤
│ New   │                                                                                                                       │ Marvin  │
│ York  │                                                                                                                       │Gardens  │
│ (19)  │                                                                                                                       │ (29)    │
│[    ] │                                                                                                                       │[    ]   │
├───────┤                                                                                                                       ├─────────┤
│ Tenn  │                                                                                                                       │ Chance  │
│ Ave   │                                                                                                                       │ (31)    │
│ (18)  │                                                                                                                       │[    ]   │
│[    ] │                                                                                                                       ├─────────┤
├───────┤                                                                                                                       │ Pacific │
│ CC    │                                                                                                                       │ Ave     │
│ (17)  │                                                                                                                       │ (32)    │
│[    ] │                                                                                                                       │[    ]   │
├───────┤                                                                                                                       ├─────────┤
│ St.   │                                                                                                                       │ N.      │
│ James │                                                                                                                       │ Carolina│
│ (16)  │                                                                                                                       │ (33)    │
│[    ] │                                                                                                                       │[    ]   │
├───────┤                                                                                                                       ├─────────┤
│ Penn  │                                                                                                                       │ CC      │
│ RR    │                                                                                                                       │ (34)    │
│ (15)  │                                                                                                                       │[    ]   │
│[    ] │                                                                                                                       ├─────────┤
├───────┤                                                                                                                       │ Penn    │
│ Vir-  │                                                                                                                       │ Ave     │
│ ginia │                                                                                                                       │ (35)    │
│ (14)  │                                                                                                                       │[    ]   │
│[    ] │                                                                                                                       ├─────────┤
├───────┤                                                                                                                       │ Short   │
│ States│                                                                                                                       │ Line    │
│ Ave   │                                                                                                                       │ RR      │
│ (13)  │                                                                                                                       │ (36)    │
│[    ] │                                                                                                                       │[    ]   │
├───────┤                                                                                                                       ├─────────┤
│ Elec  │                                                                                                                       │ Chance  │
│ Co    │                                                                                                                       │ (37)    │
│ (12)  │                                                                                                                       │[    ]   │
│[    ] │                                                                                                                       ├─────────┤
├───────┤                                                                                                                       │ Park    │
│ Conn  │                                                                                                                       │ Place   │
│ Ave   │                                                                                                                       │ (38)    │
│ (11)  │                                                                                                                       │         │
│[    ] │                                                                                                                       │[    ]   │
├───────┼──────────────┬──────────────┬──────────────┬──────────────┬──────────────┬──────────────┬──────────────┬──────────────┼─────────┤
│ 10    │ Mediterranean│ CC           │ Baltic Ave   │ Income Tax   │ Reading RR   │ Oriental Ave │ Chance       │ Vermont Ave  │ 0       │
│ JAIL  │ (1)          │ (2)          │ (3)          │ (4)          │ (5)          │ (6)          │ (7)          │ (8)          │ GO      │
│       │ [    ]       │ [    ]       │ [    ]       │              │ [    ]       │ [    ]       │ [    ]       │ [    ]       │         │
└───────┴──────────────┴──────────────┴──────────────┴──────────────┴──────────────┴──────────────┴──────────────┴──────────────┴─────────┘


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


┌────────────┬────────────┬────────────┬────────────┬────────────┬────────────┬────────────┬────────────┬────────────┬────────────┬────────────┐
│ 20 FP      │ 21 KENTUCKY│ 22 CHANCE  │ 23 INDIANA │ 24 ILLINOIS│ 25 B&O RR  │ 26 ATLANTIC│ 27 VENTNOR │ 28 WATER   │ 29 MARVIN  │ 30 GOTOJAIL│
│            │            │            │            │            │            │            │            │            │            │            │
│ [      ]   │ [      ]   │ [      ]   │ [      ]   │ [      ]   │ [      ]   │ [      ]   │ [      ]   │ [      ]   │ [      ]   │ [      ]   │
│            │            │            │            │            │            │            │            │            │            │            │
├────────────┼────────────┴────────────┴────────────┴────────────┴────────────┴────────────┴────────────┴────────────┴────────────┼────────────┤
│ 19 NEWYORK │                                                                                                                    │ 31 Pacific │
│            │                                                                                                                    │            │
│ [      ]   │                                                                                                                    │ [      ]   │
│            │                                                                                                                    │            │
├────────────┤                                                                                                                    ├────────────┤
│ 18 TENNES  │                                                                                                                    │ 32 N CAROL │
│            │                                                                                                                    │            │
│ [      ]   │                                                                                                                    │ [      ]   │
│            │                                                                                                                    │            │
├────────────┤                                                                                                                    ├────────────┤
│ 17 CC      │                                                                                                                    │ 33 CC      │
│            │                                                                                                                    │            │
│ [      ]   │                                                                                                                    │ [      ]   │
│            │                                                                                                                    │            │
├────────────┤                                                                                                                    ├────────────┤
│ 16 STJAMES │                                                                                                                    │ 34 PENN AV │
│            │                                                                                                                    │            │
│ [      ]   │                                                                                                                    │ [      ]   │
│            │                                                                                                                    │            │
├────────────┤                                                                                                                    ├────────────┤
│ 15 PA RR   │                                                                                                                    │ 35 SHORT RR│
│            │                                                                                                                    │            │
│ [      ]   │                                                                                                                    │ [      ]   │
│            │                                                                                                                    │            │
├────────────┤                                                                                                                    ├────────────┤
│ 14 VIRGINIA│                                                                                                                    │ 36 CHANCE  │
│            │                                                                                                                    │            │
│ [      ]   │                                                                                                                    │ [      ]   │
│            │                                                                                                                    │            │
├────────────┤                                                                                                                    ├────────────┤
│ 13 STATES  │                                                                                                                    │ 37 PARKPLC │
│            │                                                                                                                    │            │
│ [      ]   │                                                                                                                    │ [      ]   │
│            │                                                                                                                    │            │
├────────────┤                                                                                                                    ├────────────┤
│ 12 ELECTRIC│                                                                                                                    │ 38 Luxury  │
│            │                                                                                                                    │            │
│ [      ]   │                                                                                                                    │ [      ]   │
│            │                                                                                                                    │            │
├────────────┤                                                                                                                    ├────────────┤
│ 11 CONN    │                                                                                                                    │ 39 Board W │
│            │                                                                                                                    │            │
│ [      ]   │                                                                                                                    │ [      ]   │
│            │                                                                                                                    │            │
├───┬────────┼────────────┬────────────┬────────────┬────────────┬────────────┬────────────┬────────────┬────────────┬────────────┼────────────┤
│ F │ 10 JAIL│ 9  Connet  │ 8 VERMONT  │ 7 CHANCE   │ 6 ORIENTAL │ 5 READ RR  │ 4 INCOME   │ 3 BALTIC   │ 2 CC       │ 1 MED AVE  │ 0 GO       │
│ R │[      ]│            │            │            │            │            │            │            │            │            │            │
│ E └────────┤ [      ]   │ [      ]   │ [      ]   │ [      ]   │ [      ]   │ [      ]   │ [      ]   │ [      ]   │ [      ]   │ [      ]   │
│ E [      ] │            │            │            │            │            │            │            │            │            │            │
└────────────┴────────────┴────────────┴────────────┴────────────┴────────────┴────────────┴────────────┴────────────┴────────────┴────────────┘

40 % 40 = 0 -> Reset
10 -> Jail or no jail 

🎉 Fun / Decorative Markers
Symbol	Name	Unicode Code
🤠	Cowboy Hat Face	U+1F920
🧸	Teddy Bear	U+1F9F8
🤖	Robot Face	U+1F916
🐎	Horse	U+1F40E
🎩 Objects Often Used as Monopoly Tokens
Symbol	Name	Unicode Code
🚗	Automobile	U+1F697
🎩	Top Hat	U+1F3A9
🐕	Dog	U+1F415
🐈	Cat	U+1F408
🚢	Ship	U+1F6A2
🪙	Coin	U+1FA99
🧳	Luggage	U+1F9F3
🔑	Key	U+1F511
🧍 People / Player Figures
Symbol	Name	Unicode Code
🧍	Person Standing	U+1F9CD
🧑	Person	U+1F9D1
👤	Bust in Silhouette	U+1F464

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
    //tile *gameBoard = new tile[40];
    //initizialeGameBoard(gameBoard);
    std::vector<tile> gameBoard = {

        // 0 GO
        {false,false,0,0,0,0, 0,0,0,0,0,0, NONE, "GO","GO"},

        // 1 Mediterranean Ave
        {false,true,60,30,50,0, 2,10,30,90,160,250, BROWN, "Mediterranean Ave","MED AVE"},

        // 2 Community Chest
        {false,false,0,0,0,0, 0,0,0,0,0,0, NONE, "Community Chest","CC"},

        // 3 Baltic Ave
        {false,true,60,30,50,0, 4,20,60,180,320,450, BROWN, "Baltic Ave","BALTIC"},

        // 4 Income Tax
        {false,false,0,0,0,0, 0,0,0,0,0,0, NONE, "Income Tax","INCOME"},

        // 5 Reading Railroad
        {false,true,200,100,0,0, 25,50,100,200,0,0, RAILROAD, "Reading Railroad","READ RR"},

        // 6 Oriental Ave
        {false,true,100,50,50,0, 6,30,90,270,400,550, LIGHT_BLUE, "Oriental Ave","ORIENT"},

        // 7 Chance
        {false,false,0,0,0,0, 0,0,0,0,0,0, NONE, "Chance","CHANCE"},

        // 8 Vermont Ave
        {false,true,100,50,50,0, 6,30,90,270,400,550, LIGHT_BLUE, "Vermont Ave","VERMONT"},

        // 9 Connecticut Ave
        {false,true,120,60,50,0, 8,40,100,300,450,600, LIGHT_BLUE, "Connecticut Ave","CONNET"},

        // 10 Jail
        {false,false,0,0,0,0, 0,0,0,0,0,0, NONE, "Jail","JAIL"},

        // 11 St. Charles Place
        {false,true,140,70,100,0, 10,50,150,450,625,750, PINK, "St. Charles Place","ST CHAR"},

        // 12 Electric Company
        {false,true,150,75,0,0, 0,0,0,0,0,0, UTILITY, "Electric Company","ELECTRIC"},

        // 13 States Ave
        {false,true,140,70,100,0, 10,50,150,450,625,750, PINK, "States Ave","STATES"},

        // 14 Virginia Ave
        {false,true,160,80,100,0, 12,60,180,500,700,900, PINK, "Virginia Ave","VIRGINIA"},

        // 15 Pennsylvania Railroad
        {false,true,200,100,0,0, 25,50,100,200,0,0, RAILROAD, "Pennsylvania Railroad","PA RR"},

        // 16 St. James Place
        {false,true,180,90,100,0, 14,70,200,550,750,950, ORANGE, "St. James Place","ST JAMES"},

        // 17 Community Chest
        {false,false,0,0,0,0, 0,0,0,0,0,0, NONE, "Community Chest","CC"},

        // 18 Tennessee Ave
        {false,true,180,90,100,0, 14,70,200,550,750,950, ORANGE, "Tennessee Ave","TENNES"},

        // 19 New York Ave
        {false,true,200,100,100,0, 16,80,220,600,800,1000, ORANGE, "New York Ave","NEW YORK"},

        // 20 Free Parking
        {false,false,0,0,0,0, 0,0,0,0,0,0, NONE, "Free Parking","FP"},

        // 21 Kentucky Ave
        {false,true,220,110,150,0, 18,90,250,700,875,1050, RED, "Kentucky Ave","KENTUCKY"},

        // 22 Chance
        {false,false,0,0,0,0, 0,0,0,0,0,0, NONE, "Chance","CHANCE"},

        // 23 Indiana Ave
        {false,true,220,110,150,0, 18,90,250,700,875,1050, RED, "Indiana Ave","INDIANA"},

        // 24 Illinois Ave
        {false,true,240,120,150,0, 20,100,300,750,925,1100, RED, "Illinois Ave","ILLINOIS"},

        // 25 B&O Railroad
        {false,true,200,100,0,0, 25,50,100,200,0,0, RAILROAD, "B&O Railroad","B&O RR"},

        // 26 Atlantic Ave
        {false,true,260,130,150,0, 22,110,330,800,975,1150, YELLOW, "Atlantic Ave","ATLANTIC"},

        // 27 Ventnor Ave
        {false,true,260,130,150,0, 22,110,330,800,975,1150, YELLOW, "Ventnor Ave","VENTNOR"},

        // 28 Water Works
        {false,true,150,75,0,0, 0,0,0,0,0,0, UTILITY, "Water Works","WATER"},

        // 29 Marvin Gardens
        {false,true,280,140,150,0, 24,120,360,850,1025,1200, YELLOW, "Marvin Gardens","MARVIN"},

        // 30 Go To Jail
        {false,false,0,0,0,0, 0,0,0,0,0,0, NONE, "Go To Jail","GOTOJAIL"},

        // 31 Pacific Ave
        {false,true,300,150,200,0, 26,130,390,900,1100,1275, GREEN, "Pacific Ave","PACIFIC"},

        // 32 North Carolina Ave
        {false,true,300,150,200,0, 26,130,390,900,1100,1275, GREEN, "North Carolina Ave","N CAROL"},

        // 33 Community Chest
        {false,false,0,0,0,0, 0,0,0,0,0,0, NONE, "Community Chest","CC"},

        // 34 Pennsylvania Ave
        {false,true,320,160,200,0, 28,150,450,1000,1200,1400, GREEN, "Pennsylvania Ave","PENN AV"},

        // 35 Short Line Railroad
        {false,true,200,100,0,0, 25,50,100,200,0,0, RAILROAD, "Short Line Railroad","SHORT RR"},

        // 36 Chance
        {false,false,0,0,0,0, 0,0,0,0,0,0, NONE, "Chance","CHANCE"},

        // 37 Park Place
        {false,true,350,175,200,0, 35,175,500,1100,1300,1500, DARK_BLUE, "Park Place","PARK PLC"},

        // 38 Luxury Tax
        {false,false,0,0,0,0, 0,0,0,0,0,0, NONE, "Luxury Tax","LUXURY"},

        // 39 Boardwalk
        {false,true,400,200,200,0, 50,200,600,1400,1700,2000, DARK_BLUE, "Boardwalk","BOARD W"}
    };
    //List of players
    std::vector<player> players;

    //displayGameBoard(gameBoard);

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