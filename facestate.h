#ifndef FACESTATE_H
#define FACESTATE_H

#include <QDateTime>
#include <vector>
#include <shield.h>
#include <iostream>

class FaceState
{
public:
    FaceState();
    FaceState(QDateTime timestamp, std::vector <Shield> state);
    QDateTime get_timestamp(){return dateTime;}
    std::vector <Shield> get_state(){return current_state;}
    std::string get_status();
    void update_state(std::vector <Shield> state);
    void update_state(Shield shield);
    void update_state(QDateTime timestamp);
    void update_state(std::vector <Shield> state, QDateTime timestamp);
private:
    QDateTime dateTime;
    std::vector <Shield> current_state;
};

#endif // FACESTATE_H
