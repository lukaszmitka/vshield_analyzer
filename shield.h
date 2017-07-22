#ifndef SHIELD_H
#define SHIELD_H

#include <string>

class Shield
{
public:
    Shield();
    Shield(int id, int pressure_1, int pressure_2, int ramstroke, int coal_line, int support_pos, int conveyor_pos);
    int get_id(){return shieldNumber;}
    int get_pressure_1(){return pressure1;}
    int get_pressure_2(){return pressure2;}
    int get_ramstroke(){return ramStroke;}
    int get_coal_line(){return coalLine;}
    int get_support_pos(){return supportPos;}
    int get_conveyour_pos(){return conveyorPos;}
    std::string string_desc();
private:
    int shieldNumber; // ID
    int pressure1;
    int pressure2;
    int ramStroke; // droga przesuwnika
    int coalLine;
    int supportPos; //pozycja obudowy
    int conveyorPos; // pozycja przenosnika
};

#endif // SHIELD_H
