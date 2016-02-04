
#include "GrainBased.h"

std::vector<pdsp::GrainTable> pdsp::GrainBased::tables = std::vector<pdsp::GrainTable>();


pdsp::GrainTable*  pdsp::GrainBased::getTable(Window_t type, int length){
    
    GrainTable* toReturn = nullptr;
    
    for (GrainTable &table : tables) {
        if(table.type==type && table.length==length){
            toReturn = &table;
        }
    }
    
    if (toReturn==nullptr) {
        tables.push_back(GrainTable());
        tables[tables.size()-1].setTable(type, length);
        toReturn = &tables[tables.size()-1];
    }
    
    return toReturn;
    
}

pdsp::GrainTable*  pdsp::GrainBased::getTable(int index){
    if (index<0) {
        index = 0;
    }else if(index>=tables.size()){
        index = tables.size()-1;
    }
    
    if (tables.empty()) {
        return nullptr;
    }else{
        return &tables[index];
    }
}

const int pdsp::GrainBased::getAvailableTablesNumber () const{
    return tables.size();
}

void pdsp::GrainBased::addTable(Window_t type, int length){
    bool found = false;
    for (GrainTable &table : tables) {
        if(table.type==type && table.length==length){
            found = true;
        }
    }
    if (! found) {
        tables.push_back(GrainTable());
        tables[tables.size()-1].setTable(type, length);
    }
}
