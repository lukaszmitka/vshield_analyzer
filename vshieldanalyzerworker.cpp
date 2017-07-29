#include "vshieldanalyzerworker.h"

VShieldAnalyzerWorker::VShieldAnalyzerWorker(QSqlQuery query, QString directory, QStringList files){
    filesToProcess = files;
    filesDirectory = directory;
    num_of_files_to_process = filesToProcess.size();
    insertQuery = query;
}

void VShieldAnalyzerWorker::process(){
    for (unsigned int i = 0; i < num_of_files_to_process; ++i){
        file = filesDirectory + QDir::separator() + filesToProcess.at(i);
        current_file_id = i;
        std::string filename(file.toLocal8Bit());
        std::cout << "File to process: " << filename << std::endl;
        vShieldReader = new VShieldReader(filename);
        connect(vShieldReader, SIGNAL(fileProgress(double)), this, SLOT(fileProgress(double)));

        std::vector <FaceState> facestates;
        facestates = vShieldReader->extract_data();
        int states_no = facestates.size();
        if(states_no==0){
            //QString qStr("Błąd pliku VShield");
            //statusBar()->showMessage(qStr,0);
        } else{
            std::cout << "Entries found: " << states_no << std::endl;
            int insert_counter = 0;
            int fc_counter = 0;
            check_shields_table(facestates.at(0).get_state(), true);
            int insert_begin = 0;
            for (std::vector<FaceState>::iterator fs = facestates.begin() ; fs != facestates.end(); ++fs){

                //std::cout << (*fs).get_status() << std::endl;
                std::vector <Shield> current_state = (*fs).get_state();
                QDateTime current_timestamp = (*fs).get_timestamp();
                int64_t timestamp_epoch = current_timestamp.toMSecsSinceEpoch();
                QString queryCheckTS("SELECT * FROM timestamps WHERE timestamp='");
                queryCheckTS.append(std::to_string(timestamp_epoch).c_str());
                queryCheckTS.append("';");
                insertQuery.exec(queryCheckTS);
                if(insertQuery.next()){
                    std::cout << "Timestamp found in database, skip updating" << std::endl;
                } else {
                    if(insert_begin == 0){
                        insertQuery.exec("BEGIN");
                    }
                    //std::cout << "Inserting face state " << insert_counter << " of " << states_no << " with timestamp " << timestamp_epoch << std::endl;
                    insert_counter++;
                    QString queryInsertTS( "INSERT INTO timestamps VALUES (" );
                    queryInsertTS.append(std::to_string(timestamp_epoch).c_str());
                    queryInsertTS.append( ");" );
                    if(insertQuery.exec(queryInsertTS)){

                        for (std::vector <Shield>::iterator cur_st = current_state.begin(); cur_st !=current_state.end(); cur_st++){
                            int id = (*cur_st).get_id();
                            int pressure_1 = (*cur_st).get_pressure_1();
                            int pressure_2 = (*cur_st).get_pressure_2();
                            int ramStroke = (*cur_st).get_ramstroke();
                            int coal_line = (*cur_st).get_coal_line();
                            int suport_pos = (*cur_st).get_support_pos();
                            int conv_pos = (*cur_st).get_conveyour_pos();
                            // fields in table states:  time, shield, press2 press3, coalline
                            QString queryInsertShield("INSERT INTO states VALUES (");
                            queryInsertShield.append(std::to_string(timestamp_epoch).c_str());
                            queryInsertShield.append(", ");
                            queryInsertShield.append(std::to_string(id).c_str());
                            queryInsertShield.append(", ");
                            queryInsertShield.append(std::to_string(pressure_1).c_str());
                            queryInsertShield.append(", ");
                            queryInsertShield.append(std::to_string(pressure_2).c_str());
                            queryInsertShield.append(", ");
                            queryInsertShield.append(std::to_string(ramStroke).c_str());
                            queryInsertShield.append(", ");
                            queryInsertShield.append(std::to_string(coal_line).c_str());
                            queryInsertShield.append(", ");
                            queryInsertShield.append(std::to_string(suport_pos).c_str());
                            queryInsertShield.append(", ");
                            queryInsertShield.append(std::to_string(conv_pos).c_str());
                            queryInsertShield.append(");");
                            if(insertQuery.exec(queryInsertShield)){
                                //std::cout << "Inserted new shield: " << id << " with query: " << queryInsertShield.toLocal8Bit().data() << std::endl;
                            }
                        }
                        insert_begin++;
                        if(insert_begin == 20){
                            insertQuery.exec("COMMIT");
                            insert_begin = 0;
                        }
                    } else {
                        std::cout << "Unable to add new timestamp int table" << std::endl;
                    }
                }

                major_progress = (100 * current_file_id)/num_of_files_to_process;
                minor_progress = (50/num_of_files_to_process) + ((50/num_of_files_to_process)* fc_counter)/states_no;
                emit setValue(major_progress + minor_progress);
                fc_counter++;
            }
            if(insert_begin!=0){
                insertQuery.exec("COMMIT");
            }
        }
    }
    emit setValue(100);
    emit setShieldNo(number_of_shields);
    emit finished();
}

bool VShieldAnalyzerWorker::check_shields_table(std::vector <Shield> current_state, bool init_table){
    QString queryStr("SELECT * FROM shields");
    insertQuery.exec(queryStr);
    QString shield_list("Shields found in db: ");
    int shield_no = 0;
    while(insertQuery.next()){
        QString shield_id = insertQuery.value(0).toString();
        //std::cout << "foreign_keys: " << status.toLocal8Bit().constData() << std::endl;
        shield_list.append(shield_id);
        shield_list.append(", ");
        shield_no++;
    }
    if(shield_no>0){
        shield_list.append(" Shield count: ");
        shield_list.append(std::to_string(shield_no).c_str());
        std::cout << shield_list.toLocal8Bit().data() << std::endl;
        number_of_shields = shield_no;
    } else {
        if(init_table){
            shield_no=0;
            std::cout << "Database seems to be just created, insterting new list of shields" << std::endl;
            for (std::vector <Shield>::iterator cur_st = current_state.begin(); cur_st !=current_state.end(); cur_st++){
                int id = (*cur_st).get_id();
                //std::cout << "Insert new shield: " << id << std::endl;
                QString queryInsert("INSERT INTO shields VALUES (");
                queryInsert.append(std::to_string(id).c_str());
                queryInsert.append(");");
                if(insertQuery.exec(queryInsert)){
                    std::cout << "Inserted new shield: " << id << " with query: " << queryInsert.toLocal8Bit().data() << std::endl;
                    shield_no++;
                }
            }
            number_of_shields = shield_no;
        }
    }
    return true;
}

void VShieldAnalyzerWorker::fileProgress(double progress){
    //std::cout << "File progress: " << progress << std::endl;
    major_progress = (100 * current_file_id)/num_of_files_to_process;
    minor_progress = (50/num_of_files_to_process) * progress;
    emit setValue(major_progress + minor_progress);
}
