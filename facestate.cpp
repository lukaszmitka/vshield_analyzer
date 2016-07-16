#include "facestate.h"

FaceState::FaceState(){}

FaceState::FaceState(QDateTime timestamp, std::vector<Shield> state){
    dateTime = timestamp;
    current_state = state;
}

void FaceState::update_state(std::vector<Shield> state){
    current_state = state;
}

void FaceState::update_state(std::vector<Shield> state, QDateTime timestamp){
    dateTime = timestamp;
    current_state = state;
}

std::string FaceState::get_status(){
    std::string status;
    status.append("Timestamp: ");
    status.append(dateTime.toString().toLocal8Bit().data());
    status.append(", shields: ");
    status.append(std::to_string(current_state.size()));
    return status;
}

void FaceState::update_state(QDateTime timestamp){

    dateTime = timestamp;
}


void FaceState::update_state(Shield shield){
    //std::cout << "Updating face with shield " << shield.get_id() << "..."<< std::endl;
    int shield_id = shield.get_id();
    //std::cout << shield.string_desc() << std::endl;
    current_state.at(shield_id-1) = shield;
    //std::cout << " Done. ";
}
