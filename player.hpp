#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <limits>
#include <ostream>
#include <random>
#include <string>
#include <vector>
#include <array>
#include <algorithm>

typedef struct player{
        std::string name;
        bool jailed;
        int money;
        int currentPosition;
        int playerId;
        std::vector<int> ownedStreets;
        std::string symbol;
        ColorGroup color;
        int jailcounter;
    } player;

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
        p.jailcounter = 0;

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
        while (!(std::cin >> choice) || choice < 0 || choice >= (int)availableSmybols.size() ||
               std::find(usedSymbols.begin(), usedSymbols.end(), availableSmybols[choice]) != usedSymbols.end()) {
            std::cout<<"Invalid choice, please pick another: ";
            clearInputBuffer();
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
        while (!(std::cin >> colorChoice) || colorChoice < 0 || colorChoice >= (int)availableColors.size() ||
               std::find(usedColors.begin(), usedColors.end(), availableColors[colorChoice + 1].second) != usedColors.end()) {
            std::cout<<"Invalid choice, please pick another: ";
            clearInputBuffer();
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