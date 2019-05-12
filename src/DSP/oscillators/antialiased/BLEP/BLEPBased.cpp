

#include "BLEPBased.h"

std::vector<pdsp::BLEPTable> pdsp::BLEPBased::tables = std::vector<pdsp::BLEPTable>();


pdsp::BLEPTable*  pdsp::BLEPBased::getTable(Window_t type, bool eight, int length){

    BLEPTable* toReturn = nullptr;

    for (BLEPTable &table : tables) {
        if(table.type==type && table.eight==eight &&table.length==length){
            toReturn = &table;
        }
    }

    if (toReturn==nullptr) {
        //Logger::writeToLog("generating blep table");
        tables.push_back(BLEPTable());
        tables[tables.size()-1].setTable(type, eight, length);
        toReturn = &tables[tables.size()-1];
    }

    return toReturn;

}

void pdsp::BLEPBased::setTable(Window_t type, bool eight, int length, bool interpolate){

    blepTable = getTable(type, eight, length);
    interpolateBLEP = interpolate;

}


pdsp::BLEPTable* pdsp::BLEPBased::getTable(int index){
    if (index<0) {
        index = 0;
    }else if(index>=int(tables.size())){
        index = tables.size()-1;
    }

    if (tables.empty()) {
        return nullptr;
    }else{
        return &tables[index];
    }
}

const int pdsp::BLEPBased::getAvailableTablesNumber () const{
    return tables.size();
}

void pdsp::BLEPBased::addTable(Window_t type, bool eight, int length){
    bool found = false;
    for (BLEPTable &table : tables) {
        if(table.type==type && table.eight==eight && table.length==length){
            found = true;
        }
    }
    if (! found) {
        tables.push_back(BLEPTable());
        tables[tables.size()-1].setTable(type, eight, length);
    }
}
