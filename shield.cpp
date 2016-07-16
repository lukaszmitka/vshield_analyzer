#include "shield.h"

Shield::Shield(){

}

Shield::Shield(int id, int coal_line, int pressure_1, int pressure_2)
{
    shieldNumber = id;
    coalLine = coal_line;
    pressure1 = pressure_1;
    pressure2 = pressure_2;
}

std::string Shield::string_desc(){
    std::string params("Shield params{ id = ");
    params.append( std::to_string(shieldNumber));
    params.append(", coal line = ");
    params.append(std::to_string(coalLine));
    params.append(", pressure_1 = ");
    params.append(std::to_string(pressure1));
    params.append(", pressure 2 = ");
    params.append(std::to_string(pressure2));
    return params;
}
