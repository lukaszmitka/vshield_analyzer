#include "vshieldreader.h"

VShieldReader::VShieldReader()
{

}

VShieldReader::VShieldReader(std::string filename){
    std::vector<unsigned char> file_content;
    std::vector<unsigned char> vect_pattern_head_end;
    char buf[1];

    vshield_file_name = filename;
    vshield_file.open(vshield_file_name, std::ios::binary);
    vshield_file.seekg(0, vshield_file.end);
    int tmp_len = vshield_file.tellg();
    std::cout << "File length: " << tmp_len << std::endl;
    vshield_file.seekg(0, vshield_file.beg);

    // search for </PmcRParams>
    char pattern_head_end[] = {'<', '/', 'P', 'm', 'c', 'R', 'P', 'a', 'r', 'a', 'm', 's', '>'};
    int pattern_hed_end_length = sizeof(pattern_head_end) / sizeof(*pattern_head_end);

    vect_pattern_head_end.clear();
    for (int i = 0; i < pattern_hed_end_length; i++) {
        vect_pattern_head_end.push_back(pattern_head_end[i]);
    }
    for (int i = 0; i < pattern_hed_end_length; i++) {
        vshield_file.read(&buf[0], 1);
        file_content.push_back((unsigned char) buf[0]);
    }
    while (vect_pattern_head_end != file_content && !vshield_file.eof()) {
        vshield_file.read(&buf[0], 1);
        file_content.erase(file_content.begin());
        file_content.push_back((unsigned char) buf[0]);
    }
    if(vshield_file.eof()){
        vshield_file_ok = false;
    } else {
        metadata_end = (int)vshield_file.tellg();
        std::cout << "Found phrase </PmcRParams> at position: " << metadata_end - vect_pattern_head_end.size() << std::endl;
        file_length = tmp_len;
        shield_header = define_shield_header();
        int filename_len = (int) vshield_file_name.length();
        std::string filename_date = vshield_file_name.substr(filename_len-21, 8);
        //std::cout << "Filename date: " << filename_date << std::endl;
        std::string s_year = filename_date.substr(0,4);
        std::string s_month = filename_date.substr(4,2);
        std::string s_day = filename_date.substr(6,2);
        //std::cout << "Filename date: " << s_year << s_month << s_day << std::endl;
        //std::string::size_type sz;   // alias of size_t

        int year = std::atoi (s_year.c_str());
        int month = std::atoi (s_month.c_str());
        int day = std::atoi (s_day.c_str());

        file_date.setDate(QDate(year, month, day));
        //std::cout << "QDate: " << file_date.toString().toLocal8Bit().data() << std::endl;
        vshield_file_ok = true;
    }
}

bool VShieldReader::get_vshield_file_state(){
    return vshield_file_ok;
}

int VShieldReader::find_pattern_in_file(std::ifstream &file, std::vector<unsigned char> &pattern){
    std::cout << "Start reading file!" << std::endl;
    int occurences = 0;
    int uncounted = 0;
    std::vector<unsigned char> file_content;
    //std::vector<int> positions;
    char buf[1];
    unsigned int shield_number;
    char timestamp_buf[4];
    long timestamp_val[4];
    char shield_buf[90];
    unsigned char scuType = 0;
    unsigned char count = 0;
    long system_bits = 0;
    long error_bits = 0;
    long coal_line_1, support_pos_2, conveyor_pos_3, cut_line_4, target_line_5, sensor_2, sensor_3;
    unsigned int states[7];
    long last_timestamp = 0;
    long timestamp;
    int shield_count[142] = {0};
    int pattern_length = (int)pattern.size();
    file.seekg(0, file.end);
    std::cout << "File length: " << file.tellg() << std::endl;
    file.seekg(0, file.beg);
    //file.read(&init_buf[0], pattern_length);

    // search for </PmcRParams>
    char patter_head_end[] = {'<', '/', 'P', 'm', 'c', 'R', 'P', 'a', 'r', 'a', 'm', 's', '>'};
    int pattern_hed_end_length = sizeof(patter_head_end) / sizeof(*patter_head_end);
    std::vector<unsigned char> vect_pattern_head_end;
    vect_pattern_head_end.clear();
    for (int i = 0; i < pattern_hed_end_length; i++) {
        vect_pattern_head_end.push_back(patter_head_end[i]);
    }
    for (int i = 0; i < pattern_hed_end_length; i++) {
        file.read(&buf[0], 1);
        file_content.push_back((unsigned char) buf[0]);
    }
    while (vect_pattern_head_end != file_content) {
        file.read(&buf[0], 1);
        file_content.erase(file_content.begin());
        file_content.push_back((unsigned char) buf[0]);
    }
    std::cout << "Found phrase </PmcRParams> at position: " << (int)file.tellg() - vect_pattern_head_end.size() << std::endl;

    // searching file for shield definitions
    file_content.clear();
    //char shield_head[23];
    for (int i = 0; i < pattern_length; i++) {
        file.read(&buf[0], 1);
        file_content.push_back((unsigned char) buf[0]);
    }

    //int pos = 0;
    //int prev_pos = 0;

    std::ofstream logfile;
    logfile.open("log_2010_09_15.csv");

    if (logfile.good()) {

        logfile << "### File header" << std::endl <<
                   "Data structure: timestamp, shield number, Sensor 2, Sensor 3" <<
                   std::endl << "### File header end" << std::endl;


        //TODO remove "occurences<1000" after tests
        while (file.eof() == 0 && occurences<1000) {
            file.read(&buf[0], 1);
            file_content.erase(file_content.begin());
            file_content.push_back((unsigned char) buf[0]);
            if (pattern == file_content) {
                file.read(&buf[0], 1);
                shield_number = (unsigned char) buf[0];
                shield_count[shield_number]++;

                file.read(&buf[0], 1);
                file.read(&timestamp_buf[0], 4);
                timestamp_val[0] = (long) (unsigned char) timestamp_buf[0];
                timestamp_val[1] = (long) (unsigned char) timestamp_buf[1];
                timestamp_val[2] = (long) (unsigned char) timestamp_buf[2];
                timestamp_val[3] = (long) (unsigned char) timestamp_buf[3];

                timestamp =
                        (timestamp_val[3] * 0x1000000) + (timestamp_val[2] * 0x10000) + (timestamp_val[1] * 0x100) +
                        timestamp_val[0];

                if (last_timestamp > 64800000) {
                    if (timestamp < 21600000) {
                        timestamp = timestamp + 86400000;
                    }
                } else {
                    last_timestamp = timestamp;
                    //timestamp = 0;
                    //std::cout << "Timestamp inconsistency, old: " << last_timestamp << ", new: " << timestamp <<
                    //std::endl;
                }
                file.read(&shield_buf[0], 90);
                scuType = (unsigned char) shield_buf[0];
                count = (unsigned char) shield_buf[1];
                system_bits = ((long) (unsigned char) shield_buf[5]) * 0x1000000 +
                        ((long) (unsigned char) shield_buf[4]) * 0x10000 +
                        ((long) (unsigned char) shield_buf[3]) * 0x100 + (long) (unsigned char) shield_buf[2];
                error_bits = ((long) (unsigned char) shield_buf[9]) * 0x1000000 +
                        ((long) (unsigned char) shield_buf[8]) * 0x10000 +
                        ((long) (unsigned char) shield_buf[7]) * 0x100 + (long) (unsigned char) shield_buf[6];
                // shield_buf[10] - undefined
                coal_line_1 = ((long) (unsigned char) shield_buf[13]) * 0x10000 +
                        ((long) (unsigned char) shield_buf[12]) * 0x100 + (long) (unsigned char) shield_buf[11];
                states[0] = shield_buf[14];
                support_pos_2 = ((long) (unsigned char) shield_buf[17]) * 0x10000 +
                        ((long) (unsigned char) shield_buf[16]) * 0x100 + (long) (unsigned char) shield_buf[15];
                states[1] = shield_buf[18];
                conveyor_pos_3 = ((long) (unsigned char) shield_buf[21]) * 0x10000 +
                        ((long) (unsigned char) shield_buf[20]) * 0x100 +
                        (long) (unsigned char) shield_buf[19];
                states[2] = shield_buf[22];
                cut_line_4 = ((long) (unsigned char) shield_buf[25]) * 0x10000 +
                        ((long) (unsigned char) shield_buf[24]) * 0x100 +
                        (long) (unsigned char) shield_buf[23];
                states[3] = shield_buf[26];
                target_line_5 = ((long) (unsigned char) shield_buf[29]) * 0x10000 +
                        ((long) (unsigned char) shield_buf[28]) * 0x100 +
                        (long) (unsigned char) shield_buf[27];
                states[4] = shield_buf[30];
                sensor_2 = //((long) (unsigned char) shield_buf[33]) * 0x10000 +
                        ((long) (unsigned char) shield_buf[32]) * 0x100 +
                        (long) (unsigned char) shield_buf[31];
                states[5] = shield_buf[33];
                sensor_3 = //((long) (unsigned char) shield_buf[34]) * 0x10000 +
                        ((long) (unsigned char) shield_buf[35]) * 0x100 +
                        (long) (unsigned char) shield_buf[34];
                states[6] = shield_buf[36];
                logfile << timestamp << ", " << shield_number << ", " << sensor_2 << ", " << sensor_3 << std::endl;

                occurences++;
            } else {
                //std::cout << "vectors are not equal" << std::endl;
            }
        }

    } else {
        std::cout << "File error!" << std::endl;
    }
    logfile.close();

    std::cout << "Found " << occurences << " occurences of pattern." << std::endl;
    std::cout << "Found shields: " << std::endl;
    for (int i = 1; i < 142; i++) {
        std::cout << shield_count[i] << ", ";
    }
    std::cout << std::endl;
    std::cout << "Found " << uncounted << " uncounted patterns." << std::endl;
    std::cout << "End reading file!" << std::endl;
    return 0;
}

int VShieldReader::run_main(){
    std::cout << "Analyzing VShield!" << std::endl;

    if(vshield_file_ok==true){
        std::vector<unsigned char>  vect_pattern_shield_beg;
        char pattern_shield_beg[] = {(char)0x66, (char)0x01, (char)0x40, (char)0xf6, (char)0x01, (char)0xf6, (char)0x01}; // beginning of the shield
        std::cout << &pattern_shield_beg[0]<<std::endl;
        int patter_shield_beg_length = sizeof(pattern_shield_beg) / sizeof(*pattern_shield_beg);
        for (int i = 0; i < patter_shield_beg_length; i++) {
            vect_pattern_shield_beg.push_back(pattern_shield_beg[i]);
        }
        if (vshield_file.good()) {
            std::cout << "File opened!" << std::endl;
            // TODO
            find_pattern_in_file(vshield_file, vect_pattern_shield_beg);
        } else {
            std::cout << "File error!" << std::endl;
        }
        vshield_file.close();
        return 0;
    } else {
        return -1;
    }
}

bool VShieldReader::go_to_next_timestamp(FaceState *facestate, int *position){
    Shield tmp_sh;
    int32_t tmp_ts = 0;
    int shields_read = 0;
    //bool dest_reached = false;
    int32_t dest_timestamp = file_date.msecsTo(facestate->get_timestamp().addSecs(TIMESTAMP_INTERVAL));
    while(dest_timestamp > tmp_ts){
        //std::cout << "Read next entry:";
        if(read_next_entry(&tmp_sh, vshield_file, shield_header, position, &tmp_ts)){
            if(dest_timestamp>40000000 && tmp_ts <1000000){
                tmp_ts+=86400000;
            }
            //std::cout << " OK, ";
            //std::cout<<tmp_sh.string_desc()<< ", ts = " << tmp_ts << std::endl;
            facestate->update_state(tmp_sh);
            shields_read++;
            //std::cout << "Procdessed shields: " << shields_read << std::endl;
        } else {
            std::cout << "End of file" << std::endl;
            tmp_ts = dest_timestamp;
            return false;
        }
        //std::cout<< "Dest = " << dest_timestamp << ", tmp = " << tmp_ts << std::endl;
    }
    facestate->update_state(facestate->get_timestamp().addSecs(TIMESTAMP_INTERVAL));
    //std::cout << "At timestamp " << facestate->get_timestamp().toString().toLocal8Bit().data() << " found " << shields_read << " shields." << std::endl;
    return true;
}

bool VShieldReader::init_read(QDate date, int *position, FaceState *faceState){
    std::vector <Shield> shields;

    Shield tmp_sh;
    int tmp_ts = 0;
    int last_timestamp = 0;
    bool all_shields_found = false;
    while(read_next_entry(&tmp_sh, vshield_file, shield_header, position, &tmp_ts) && !all_shields_found){
        try {
            shields.at(tmp_sh.get_id()-1);
            all_shields_found = true;
        }
        catch (const std::out_of_range& oor) {
            //std::cout << "Push back shield #" << tmp_sh.get_id();
            shields.push_back(tmp_sh);
            last_timestamp = tmp_ts;
            //std::cout << " at pos " << shields.size()-1 << " with ts = " << tmp_ts << std::endl;
        }
    }
    QDateTime ts;
    ts.setDate(date);
    //std::cout << "QDate: " << ts.toString().toLocal8Bit().data() << std::endl;
    ts = ts.addMSecs(last_timestamp);
    //std::cout << "Added seconds: " << last_timestamp << std::endl;
    //std::cout << "QDate: " << ts.toString().toLocal8Bit().data() << std::endl;
    ts = ts.addSecs(TIMESTAMP_INTERVAL -( ts.time().second() % TIMESTAMP_INTERVAL));
    std::cout << "QDate: " << ts.toString().toLocal8Bit().data() << std::endl;
    faceState->update_state(shields, ts);
    *position = *position - 120;
    std::cout << "Found " << shields.size() << " shields." << std::endl;
    return false;
}

std::vector <FaceState> VShieldReader::extract_data(){
    //std::cout << "Extracting data from vshield" << std::endl;
    std::vector <FaceState> facestates;
    pos_in_file = metadata_end;

    if(vshield_file_ok==true){
        QDateTime ts;
        ts.setDate(file_date.date());
        FaceState fs;
        init_read(ts.date(), &pos_in_file, &fs);
        facestates.push_back(fs);
        std::cout << "Read initialization completed" << std::endl;
        while(go_to_next_timestamp(&fs, &pos_in_file)){
            facestates.push_back(fs);
        }
    }
    vshield_file.close();
    vshield_file_ok = false;
    return facestates;
}

std::vector<unsigned char>  VShieldReader::define_shield_header(){
    std::vector<unsigned char>  shield_head;
    char c_shield_head[] = {(char)0x66, (char)0x01, (char)0x40, (char)0xf6, (char)0x01, (char)0xf6, (char)0x01}; // beginning of the shield
    //std::cout << &c_shield_head[0]<<std::endl;
    int head_length = sizeof(c_shield_head) / sizeof(*c_shield_head);
    for (int i = 0; i < head_length; i++) {
        shield_head.push_back(c_shield_head[i]);
    }
    return shield_head;
}

bool VShieldReader::read_next_entry(Shield *shield, std::ifstream &file, std::vector<unsigned char> &pattern, int *position, int32_t *timestamp_update){
    //std::cout << "Reading at pos: " << *position << "...";
    std::vector<unsigned char> data_read;
    char buf[1];
    char timestamp_buf[4];
    long timestamp_val[4];
    char shield_buf[90];

    unsigned int shield_number;
    long timestamp;
    unsigned char scuType = 0;
    unsigned char count = 0;
    long system_bits = 0;
    long error_bits = 0;
    long coal_line_1, support_pos_2, conveyor_pos_3, cut_line_4, target_line_5, sensor_2, sensor_3, sensor_4;
    unsigned int states[8];

    int pattern_length = (int)pattern.size();
    file.seekg(*position, file.beg);

    // read first portion of data
    data_read.clear();
    for (int i = 0; i < pattern_length; i++) {
        file.read(&buf[0], 1);
        data_read.push_back((unsigned char) buf[0]);
    }

    while (file.eof() == 0 && pattern != data_read) {
        file.read(&buf[0], 1);
        data_read.erase(data_read.begin());
        data_read.push_back((unsigned char) buf[0]);
    }

    if (file.eof() != 0)
    {
        // Reached end of file
        return false;
    } else {
        file.read(&buf[0], 1);
        shield_number = (unsigned char) buf[0];
        file.read(&buf[0], 1);
        file.read(&timestamp_buf[0], 4);
        timestamp_val[0] = (long) (unsigned char) timestamp_buf[0];
        timestamp_val[1] = (long) (unsigned char) timestamp_buf[1];
        timestamp_val[2] = (long) (unsigned char) timestamp_buf[2];
        timestamp_val[3] = (long) (unsigned char) timestamp_buf[3];

        timestamp = (timestamp_val[3] * 0x1000000) + (timestamp_val[2] * 0x10000) + (timestamp_val[1] * 0x100) + timestamp_val[0];

        file.read(&shield_buf[0], 90);
        scuType = (unsigned char) shield_buf[0];
        count = (unsigned char) shield_buf[1];
        system_bits = ((long) (unsigned char) shield_buf[5]) * 0x1000000 +
                ((long) (unsigned char) shield_buf[4]) * 0x10000 +
                ((long) (unsigned char) shield_buf[3]) * 0x100 + (long) (unsigned char) shield_buf[2];
        error_bits = ((long) (unsigned char) shield_buf[9]) * 0x1000000 +
                ((long) (unsigned char) shield_buf[8]) * 0x10000 +
                ((long) (unsigned char) shield_buf[7]) * 0x100 + (long) (unsigned char) shield_buf[6];
        // shield_buf[10] - undefined
        coal_line_1 = ((long) (unsigned char) shield_buf[13]) * 0x10000 +
                ((long) (unsigned char) shield_buf[12]) * 0x100 + (long) (unsigned char) shield_buf[11];
        states[0] = shield_buf[14];
        support_pos_2 = ((long) (unsigned char) shield_buf[17]) * 0x10000 +
                ((long) (unsigned char) shield_buf[16]) * 0x100 + (long) (unsigned char) shield_buf[15];
        states[1] = shield_buf[18];
        conveyor_pos_3 = ((long) (unsigned char) shield_buf[21]) * 0x10000 +
                ((long) (unsigned char) shield_buf[20]) * 0x100 +
                (long) (unsigned char) shield_buf[19];
        states[2] = shield_buf[22];
        cut_line_4 = ((long) (unsigned char) shield_buf[25]) * 0x10000 +
                ((long) (unsigned char) shield_buf[24]) * 0x100 +
                (long) (unsigned char) shield_buf[23];
        states[3] = shield_buf[26];
        target_line_5 = ((long) (unsigned char) shield_buf[29]) * 0x10000 +
                ((long) (unsigned char) shield_buf[28]) * 0x100 +
                (long) (unsigned char) shield_buf[27];
        states[4] = shield_buf[30];
        sensor_2 = //((long) (unsigned char) shield_buf[33]) * 0x10000 +
                ((long) (unsigned char) shield_buf[32]) * 0x100 +
                (long) (unsigned char) shield_buf[31];
        states[5] = shield_buf[33];
        sensor_3 = //((long) (unsigned char) shield_buf[34]) * 0x10000 +
                ((long) (unsigned char) shield_buf[35]) * 0x100 +
                (long) (unsigned char) shield_buf[34];
        states[6] = shield_buf[36];
        sensor_4 = ((long) (unsigned char) shield_buf[38]) * 0x100 +
                (long) (unsigned char) shield_buf[37];
        //logfile << timestamp << ", " << shield_number << ", " << sensor_2 << ", " << sensor_3 << std::endl;
        Shield tmp(shield_number, sensor_2, sensor_3, sensor_4, coal_line_1, support_pos_2, conveyor_pos_3);
        *shield = tmp;
        *position = file.tellg();
        *timestamp_update = timestamp;
        //std::cout << " Done." << std::endl;
        return true;
    }
    /*logfile.close();

        std::cout << "Found " << occurences << " occurences of pattern." << std::endl;
        std::cout << "Found shields: " << std::endl;
        for (int i = 1; i < 142; i++) {
            std::cout << shield_count[i] << ", ";
        }
        std::cout << std::endl;
        std::cout << "Found " << uncounted << " uncounted patterns." << std::endl;
        std::cout << "End reading file!" << std::endl;*/
    return 0;
}

std::string VShieldReader::get_vshield_file_name(){
    return vshield_file_name;
}
