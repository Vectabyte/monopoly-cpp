#pragma once
#include <fstream>
#include <vector>
#include "Eintrag.hpp"

inline void exportFile(std::vector<eintrag> *vec){
    std::ofstream csv ("Telefonbuch.csv");

    for(eintrag e : *vec){
        csv<<e.nachname<<";"<<e.vorname<<";"<<e.telefonnummer<<"\n";
    }
    csv.close();
}