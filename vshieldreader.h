#ifndef VSHIELDREADER_H
#define VSHIELDREADER_H

#include <iostream>
#include <fstream>
#include <vector>
#include "shield.h"
#include "facestate.h"
#include <QDateTime>
#include <QObject>

#define TIMESTAMP_INTERVAL 30 // seconds

class VShieldReader : public QObject
{
    Q_OBJECT
public:
    VShieldReader();
    VShieldReader(std::string filename);
    ~VShieldReader();
    int find_pattern_in_file(std::ifstream &file, std::vector<unsigned char> &pattern);
    int run_main();
    std::vector <FaceState> extract_data();
    std::string get_vshield_file_name();
    bool get_vshield_file_state();
    int get_file_length(){return file_length;}
signals:
    void fileProgress(double progress);
private:
    // TODO delete default value
    std::string vshield_file_name = "C:/VShield_10_77_Build_6125/20100915.LogVShield10"; //default value
    std::ifstream vshield_file;
    bool vshield_file_ok = false;
    bool init_read(QDate date, int *position, FaceState *faceState);
    bool go_to_next_timestamp(FaceState *timestamp, int *position);
    std::vector<unsigned char>  shield_header;
    int file_length = 0;
    int metadata_end = 0;
    int pos_in_file = 0;
    QDateTime file_date;
    std::vector<unsigned char>  define_shield_header();
    bool read_next_entry(Shield *shield, std::ifstream &file, std::vector<unsigned char> &pattern, int *position, int *timestamp);
    //std::vector<unsigned char> vect_pattern_shield_beg;
};

#endif // VSHIELDREADER_H
