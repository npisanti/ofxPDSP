


#include "BLEPTable.h"



pdsp::BLEPTable::BLEPTable()
{
    buffer = nullptr;
    length = 0;
    length_f = 0.0f;
    points = 2;
    eight = false;
    type = Rectangular;

}

pdsp::BLEPTable::BLEPTable(const BLEPTable& other)
{
    if(other.buffer!=nullptr){
        setTable(other.type, other.eight, other.length);
    }
}


pdsp::BLEPTable& pdsp::BLEPTable::operator= (const BLEPTable& other)
{
    if(other.buffer!=nullptr){
        setTable(other.type, other.eight, other.length);
    }else{
        if(buffer!=nullptr){
            length=0;
            ofx_deallocate_aligned(buffer);
        }
    }
    return *this;
}


pdsp::BLEPTable::~BLEPTable()
{
    if(buffer!=nullptr){
        ofx_deallocate_aligned(buffer);
    }
}


void pdsp::BLEPTable::setTable(Window_t type, bool eight, int length){
    //Logger::writeToLog("setting blep table");

    this->type = type;
    this->eight = eight;
    this->length = length;
    this->length_f = static_cast<float>(length);
    if(eight){
        points = 8.0f;
    }else{
        points = 2.0f;
    }
    if (eight) {
        buffer = blepTable8(type, length);
    }else{
        buffer = blepTable2(type, length);
    }



}






bool pdsp::BLEPTable::empty(){
    return (buffer==nullptr);
}
