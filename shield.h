#ifndef SHIELD_H
#define SHIELD_H

#include <string>

class Shield
{
public:
    Shield();
    Shield(int id, int coal_line, int pressure_1, int pressure_2);
    int get_id(){return shieldNumber;}
    int get_coal_line(){return coalLine;}
    int get_pressure_1(){return pressure1;}
    int get_pressure_2(){return pressure2;}
    std::string string_desc();
private:
    int shieldNumber; // ID
    int coalLine;
    int pressure1;
    int pressure2;
};

#endif // SHIELD_H
