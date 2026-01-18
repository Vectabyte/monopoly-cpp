#pragma once
#include <cstddef>
#include <fstream>
#include <vector>
#include <string>
#include "player.hpp"
#include "tile.hpp"

inline void importFile(std::vector<tile>& tiles)  {
    std::ifstream csv;
    csv.open("Tiles.csv");
    std::string line;
    const std::string del = ";";

    while (std::getline(csv, line)){
        tile s;
        std::size_t pos = 0;
        std::size_t start = 0;
   
        pos = line.find(del,start);
        s.tileIndex = std::stoi(line.substr(start, pos-start));
        start = pos + del.length();

        pos = line.find(del,start);
        s.ownerId = std::stoi(line.substr(start, pos-start));
        start = pos + del.length();

        pos = line.find(del,start);
        s.buyable = line.substr(start, pos-start) == "true";
        start = pos + del.length();

        pos = line.find(del,start);
        s.buyPrice = std::stoi(line.substr(start, pos-start));
        start = pos + del.length();

        pos = line.find(del,start);
        s.isMortgaged = line.substr(start, pos-start) == "true";
        start = pos + del.length();

        pos = line.find(del,start);
        s.housePrice = std::stoi(line.substr(start, pos-start));
        start = pos + del.length();

        pos = line.find(del,start);
        s.upgradeStage = std::stoi(line.substr(start, pos-start));
        start = pos + del.length();

        pos = line.find(del,start);
        s.price0 = std::stoi(line.substr(start, pos-start));
        start = pos + del.length();

        pos = line.find(del,start);
        s.price1 = std::stoi(line.substr(start, pos-start));
        start = pos + del.length();

        pos = line.find(del,start);
        s.price2 = std::stoi(line.substr(start, pos-start));
        start = pos + del.length();

        pos = line.find(del,start);
        s.price3 = std::stoi(line.substr(start, pos-start));
        start = pos + del.length();

        pos = line.find(del,start);
        s.price4 = std::stoi(line.substr(start, pos-start));
        start = pos + del.length();

        pos = line.find(del,start);
        s.price5 = std::stoi(line.substr(start, pos-start));
        start = pos + del.length();

        pos = line.find(del,start);
        std::string colorStr = line.substr(start, pos-start);
        s.color = stringColorGroupMatcher(colorStr);
        start = pos + del.length();

        pos = line.find(del,start);
        s.tileName = line.substr(start, pos-start);
        start = pos + del.length();

        s.shortName = line.substr(start);

        tiles.push_back(s);
    }
    csv.close();
}

inline void importFile(std::vector<player>& players)  {
    std::ifstream csv;
    csv.open("Players.csv");
    std::ifstream ownedStreetsFile;
    ownedStreetsFile.open("OwnedStreets.csv");
    std::string line;
    const std::string del = ";";

    while (std::getline(csv, line)){
        player s;
        std::size_t pos = 0;
        std::size_t start = 0;
   
        pos = line.find(del,start);
        s.playerId = std::stoi(line.substr(start, pos-start));
        start = pos + del.length();

        pos = line.find(del,start);
        s.symbol = line.substr(start, pos-start);
        start = pos + del.length();

        pos = line.find(del,start);
        s.name = line.substr(start, pos-start);
        start = pos + del.length();

        pos = line.find(del,start);
        s.money = std::stoi(line.substr(start, pos-start));
        start = pos + del.length();

        pos = line.find(del,start);
        s.currentPosition = std::stoi(line.substr(start, pos-start));
        start = pos + del.length();

        pos = line.find(del,start);
        s.jailed = line.substr(start, pos-start) == "true";
        start = pos + del.length();

        pos = line.find(del,start);
        std::string colorStr = line.substr(start, pos-start);
        s.color = stringColorGroupMatcher(colorStr);
        start = pos + del.length();

        pos = line.find(del,start);
        s.jailCounter = std::stoi(line.substr(start, pos-start));
        start = pos + del.length();

        pos = line.find(del,start);
        s.jailFreeCard = std::stoi(line.substr(start, pos-start));
        start = pos + del.length();

        s.bankrupt = line.substr(start, pos-start) == "true";

        players.push_back(s);
    }
    csv.close();

    while (std::getline(ownedStreetsFile, line)){
        int playerId;
        int tileId;
        std::size_t pos = 0;
        std::size_t start = 0;
   
        pos = line.find(del,start);
        playerId = std::stoi(line.substr(start, pos-start));
        start = pos + del.length();

        tileId = std::stoi(line.substr(start, pos-start));
        
        for (auto& player : players) { 
            if (player.playerId == playerId) {
                player.ownedStreets.push_back(tileId);
                break;
            }
        }
    }
    ownedStreetsFile.close();
}

inline void importFile(int freeParkingMoney, int currentPlayerTurn, std::vector<std::size_t>& turnOrder)  {
    std::ifstream csv;
    csv.open("otherData.csv");
    std::string line;
    const std::string del = ";";

    if (std::getline(csv, line)) {
        freeParkingMoney = std::stoi(line);
    }

    if (std::getline(csv, line)) {
        currentPlayerTurn = std::stoi(line);
    }

    while (std::getline(csv, line)){
        int s;
        std::size_t pos = 0;
        std::size_t start = 0;
   
        pos = line.find(del,start);
        s = std::stoi(line.substr(start, pos-start));
        start = pos + del.length();

        turnOrder.push_back(s);
    }
    csv.close();
}