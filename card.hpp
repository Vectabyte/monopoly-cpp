#pragma once
#include <string>
#include <unordered_map>
#include <vector>
#include <algorithm>

typedef struct card{
        std::string action;
        std::string text;
        std::unordered_map<std::string, std::string> value;
    } card;

std::vector<card> initializeChanceCards() {
    // Initialize Gameboard with all tiles
    std::vector<card> chanceCards = {
        {"advance", "Advance to Boardwalk", {}},
        {"advance", "Take a trip to Reading Railroad. If you pass Go, collect $200", {{"amount","200"}}},
        {"advance", "Advance to Go (Collect $200)", {{"amount","200"}}},
        {"advance", "Advance to Illinois Avenue. If you pass Go, collect $200", {{"amount","200"}}},
        {"advance", "Advance to St. Charles Place. If you pass Go, collect $200", {{"amount","200"}}},
        {"moveNearest", "Advance to the nearest Railroad. If unowned, you may buy it from the Bank. If owned, pay twice the rental", {{"destination","railroad"}}},
        {"moveNearest", "Advance to the nearest Railroad. If unowned, you may buy it from the Bank. If owned, pay twice the rental", {{"destination","railroad"}}},
        {"moveNearest", "Advance token to nearest Utility. If unowned, you may buy it from the Bank. If owned, throw dice and pay owner a total ten times amount thrown", {{"destination","utility"}}},
        {"jailFree", "Get Out of Jail Free", {}},
        {"move", "Go Back 3 Spaces", {{"steps","3"}}},
        {"jail", "Go to Jail. Go directly to Jail, do not pass Go, do not collect $200", {}},
        {"repairTax", "Make general repairs on all your property: For each house pay $25; For each hotel pay $100", {{"perHouse","25"},{"perHotel","100"}}},
        {"pay", "Speeding fine $15", {{"amount","15"}}},
        {"payEach", "You have been elected Chairman of the Board. Pay each player $50", {{"amount","50"}}},
        {"receive", "Bank pays you dividend of $50", {{"amount","50"}}},
        {"receive", "Your building loan matures. Collect $150", {{"amount","150"}}}
    };

    random_shuffle(chanceCards.begin(), chanceCards.end());
    return chanceCards;
}

std::vector<card> initializeCommunityCards() {
    // Initialize Gameboard with all tiles
    std::vector<card> communityCards = {
        {"advance", "Advance to Go (Collect $400)", {{"amount","400"},{"position","0"}}},
        {"jailFree", "Get Out of Jail Free", {}},
        {"jail", "Go to Jail. Go directly to jail, do not pass Go, do not collect $200", {}},
        {"pay", "Doctor’s fee. Pay $50", {{"amount","50"}}},
        {"pay", "Pay hospital fees of $100", {{"amount","100"}}},
        {"pay", "Pay school fees of $50", {{"amount","50"}}},
        {"repairTax", "You are assessed for street repair: $40 per house; $115 per hotel", {{"perHouse","40"},{"perHotel","115"}}},
        {"receiveFromPlayers", "It is your birthday. Collect $10 from every player", {{"amount","10"}}},
        {"receive", "Bank error in your favor. Collect $200", {{"amount","200"}}},
        {"receive", "From sale of stock you get $50", {{"amount","50"}}},
        {"receive", "Holiday fund matures. Receive $100", {{"amount","100"}}},
        {"receive", "Income tax refund. Collect $20", {{"amount","20"}}},
        {"receive", "Life insurance matures. Collect $100", {{"amount","100"}}},
        {"receive", "Receive $25 consultancy fee", {{"amount","25"}}},
        {"receive", "You have won second prize in a beauty contest. Collect $10", {{"amount","10"}}},
        {"receive", "You inherit $100", {{"amount","100"}}}
    };

    random_shuffle(communityCards.begin(), communityCards.end());
    return communityCards;
}