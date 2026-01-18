#pragma once
#include <cstddef>
#include <fstream>
#include <vector>
#include <string>
#include "player.hpp"
#include "tile.hpp"

inline void importFile(std::vector<eintrag> *vec)  {
    std::ifstream csv;
    csv.open("Telefonbuch.csv");
    std::string line;
    const std::string del = ";";

    while (std::getline(csv, line)){
        eintrag s;
        std::size_t pos = 0;
        std::size_t start = 0;

        pos = line.find(del,start);
        s.nachname = line.substr(start, pos-start);
        start = pos + del.length();

        pos = line.find(del,start);
        s.vorname = line.substr(start, pos-start);
        start = pos + del.length();

        s.telefonnummer = std::stoll(line.substr(start));

        vec->push_back(s);
    }
    csv.close();
}