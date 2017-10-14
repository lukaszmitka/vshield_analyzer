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
#define VSHIELD_VERSION_7 "Bogdanka V7"
#define VSHIELD_VERSION_11 "Bogdanka V11"
#define VSHIELD_VERSION_12 "Bogdanka V12 B4562"
#define VSHIELD_VERSION_2_V6 "Bogdanka2 V6 B6073"
#define VSHIELD_VERSION_7_DATA_LEN 0x66
#define VSHIELD_VERSION_11_DATA_LEN 0x66
#define VSHIELD_VERSION_12_DATA_LEN 0x6f
#define VSHIELD_VERSION_2_V6_DATA_LEN 0x6e

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

    enum Version {
        e_version_7,
        e_version_11,
        e_version_12,
        e_version_2_v6
    };

    int file_version;

    unsigned char version_7[11] = {'B', 'o', 'g', 'd', 'a', 'n', 'k', 'a', ' ', 'V', '7'};
    unsigned char version_11[12] = {'B', 'o', 'g', 'd', 'a', 'n', 'k', 'a', ' ', 'V', '1', '1'};
    unsigned char version_12[18] = {'B', 'o', 'g', 'd', 'a', 'n', 'k', 'a', ' ', 'V', '1', '2', ' ', 'B', '4', '5', '6', '2'};
    unsigned char version_2_V6[18] = {'B', 'o', 'g', 'd', 'a', 'n', 'k', 'a', '2', ' ', 'V', '6', ' ', 'B', '6', '0', '7', '3'};

    std::vector<unsigned char> vect_version_7;
    std::vector<unsigned char> vect_version_11;
    std::vector<unsigned char> vect_version_12;
    std::vector<unsigned char> vect_version_2_V6;


};

#endif // VSHIELDREADER_H
