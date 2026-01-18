#pragma once
#include <fstream>
#include <vector>
#include "player.hpp"
#include "tile.hpp"

inline void exportFile(std::vector<tile> *vec){
    std::ofstream csv ("Tiles.csv");

    for(tile t : *vec){
        csv<<t.tileIndex<<";"<<t.ownerId<<";"<<t.buyable<<";"<<t.buyPrice<<";"<<t.isMortgaged<<";"<<t.housePrice<<";"<<t.upgradeStage<<";"<<t.price0<<";"<<t.price1<<";"<<t.price2<<";"<<t.price3<<";"<<t.price4<<";"<<t.price5<<";"<<t.color<<";"<<t.tileName<<";"<<t.shortName<<"\n";
    }
    csv.close();
}

inline void exportFile(std::vector<player> *vec){
    std::ofstream csv ("Players.csv");
    std::ofstream ownedStreetsFile ("OwnedStreets.csv");

    for(player p : *vec){
        csv<<p.playerId<<";"<<p.symbol<<";"<<p.name<<";"<<p.money<<";"<<p.currentPosition<<";"<<p.jailed<<";"<<p.color<<";"<<p.jailCounter<<";"<<p.jailFreeCard<<";"<<p.bankrupt<<"\n";
        for (int street : p.ownedStreets){
            ownedStreetsFile<<p.playerId<<";"<<street<<"\n";
        }
    }
    csv.close();
}

inline void exportFile(int freeParkingMoney, int currentPlayerTurn, std::vector<std::size_t> turnOrder){
    std::ofstream csv ("otherData.csv");

    csv<<freeParkingMoney<<"\n";
    csv<<currentPlayerTurn<<"\n";
    for(std::size_t i : turnOrder){
        csv<<i<<";";
    }
    csv<<"\n";
    csv.close();
}