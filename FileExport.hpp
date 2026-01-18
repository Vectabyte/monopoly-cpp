#pragma once
#include <fstream>
#include <vector>
#include "player.hpp"
#include "tile.hpp"

void exportFile(std::vector<tile>& tiles){
    std::ofstream csv ("Tiles.csv");

    for(tile t : tiles){
        csv<<t.tileIndex<<";"<<t.ownerId<<";"<<t.buyable<<";"<<t.buyPrice<<";"<<t.isMortgaged<<";"<<t.housePrice<<";"<<t.upgradeStage<<";"<<t.price0<<";"<<t.price1<<";"<<t.price2<<";"<<t.price3<<";"<<t.price4<<";"<<t.price5<<";"<<t.color<<";"<<t.tileName<<";"<<t.shortName<<"\n";
    }
    csv.close();
}

void exportFile(std::vector<player>& players){
    std::ofstream csv ("Players.csv");
    std::ofstream ownedStreetsFile ("OwnedStreets.csv");

    for(player p : players){
        for (int street : p.ownedStreets){
            ownedStreetsFile<<p.playerId<<";"<<street<<"\n";
        }
    }
    csv.close();
    ownedStreetsFile.close();
}

void exportFile(int freeParkingMoney, int currentPlayerTurn, std::vector<std::size_t>& turnOrder){
    std::ofstream csv ("otherData.csv");

    csv<<freeParkingMoney<<"\n";
    csv<<currentPlayerTurn<<"\n";
    for(std::size_t i : turnOrder){
        csv<<i<<"\n";
    }
    csv.close();
}