#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent):QMainWindow(parent),ui(new Ui::MainWindow)
{
    //QTextCodec::setCodecForTr (QTextCodec::codecForName ("Windows-1250"));
    QTextCodec::setCodecForLocale(QTextCodec::codecForName ("Windows-1250"));
    ui->setupUi(this);
    setWindowTitle(tr("VShield analyzer"));
    setMinimumSize(300, 300);
    resize(1000, 800);
    //showFullScreen();

    // give the axes some labels:
    //ui->customPlot->xAxis->setLabel("Czas");
    ui->customPlot->yAxis->setLabel("Ciśnienie [MPa]");

    ui->customPlot->setAutoAddPlottableToLegend(true);

    ui->customPlot->legend->setVisible(true);
    ui->customPlot->legend->setFont(QFont("Helvetica", 9));
    ui->customPlot->yAxis2->setVisible(true);
    ui->customPlot->yAxis2->setLabel("Pozycja [m]");

    // configure bottom axis to show date and time instead of number:
    ui->customPlot->xAxis->setTickLabelType(QCPAxis::ltDateTime);
    ui->customPlot->xAxis->setDateTimeFormat("dd-MM-yyyy\nHH:mm");
    // set a more compact font size for bottom and left axis tick labels:
    ui->customPlot->xAxis->setTickLabelFont(QFont(QFont().family(), 8));
    //ui->customPlot->yAxis->setTickLabelFont(QFont(QFont().family(), 8));

    // Allow user to drag axis ranges with mouse, zoom with mouse wheel and select graphs by clicking:
    //ui->customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);

    //qAction_one = new QAction(tr("&Nowy"));
    // qAction_one->setStatusTip("Nowy plik");
    //connect(qAction_one, &QAction::triggered, this,  &MainWindow::nowy_plik);

    qAction_openVShield = new QAction(tr("&Otwórz plik VShield"));
    qAction_openVShield->setStatusTip("Otwórz istniejący plik");
    connect(qAction_openVShield, &QAction::triggered, this, &MainWindow::openVShieldFile);

    qAction_analyze_vshield = new QAction(tr("&Analizuj plik"));
    qAction_analyze_vshield->setStatusTip("Tymczasowa funkcja do analizy VShield");
    connect(qAction_analyze_vshield, &QAction::triggered, this, &MainWindow::vShieldAnalyze);

    qAction_close = new QAction(tr("&Zamknij"));
    qAction_close->setStatusTip("Zakończ program");
    connect(qAction_close, &QAction::triggered, this, &MainWindow::close);

    qAction_select_existing_db = new QAction(tr("&Otwórz bazę danych"));
    QString *sel_db_statusTip = new QString;
    sel_db_statusTip->append("Otwiera istniejącą bazę danych");
    if(!db_file.isEmpty()){
        sel_db_statusTip->append(" (obecnie otwarta: ");
        sel_db_statusTip->append(db_file);
    }
    qAction_select_existing_db->setStatusTip(*sel_db_statusTip);
    connect(qAction_select_existing_db, &QAction::triggered, this, &MainWindow::openDatabase);

    qAction_create_db = new QAction(tr("&Stwórz bazę danych"));
    QString *new_db_ststusTip = new QString;
    new_db_ststusTip->append("Tworzy nową bazę danych");
    if(!db_file.isEmpty()){
        new_db_ststusTip->append(" (obecnie otwarta: ");
        new_db_ststusTip->append(db_file);
    }
    qAction_create_db->setStatusTip(*new_db_ststusTip);
    connect(qAction_create_db, &QAction::triggered, this, &MainWindow::newDatabase);

    fileMenu = menuBar()->addMenu(tr("&Plik"));
    fileMenu->addAction(qAction_select_existing_db);
    fileMenu->addAction(qAction_create_db);
    fileMenu->addSeparator();
    fileMenu->addAction(qAction_openVShield);
    fileMenu->addAction(qAction_analyze_vshield);
    fileMenu->addSeparator();
    fileMenu->addAction(qAction_close);
    connect(fileMenu, SIGNAL(aboutToShow()),this, SLOT(setActiveActions()));

    populate_shield_list(db);
    connect(ui->shieldList, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(shieldClicked(QListWidgetItem*)));

    vShieldReader = new VShieldReader();
    //QCoreApplication.addLibraryPath(".");
}

bool MainWindow::check_shields_table(std::vector <Shield> current_state){
    QString queryStr("SELECT * FROM shields");
    query.exec(queryStr);
    QString shield_list("Shields found in db: ");
    int shield_no = 0;
    while(query.next()){
        QString shield_id = query.value(0).toString();
        //std::cout << "foreign_keys: " << status.toLocal8Bit().constData() << std::endl;
        shield_list.append(shield_id);
        shield_list.append(", ");
        shield_no++;
    }
    if(shield_no>0){
        shield_list.append(" Shield count: ");
        shield_list.append(std::to_string(shield_no).c_str());
        std::cout << shield_list.toLocal8Bit().data() << std::endl;
    } else {
        std::cout << "Database semms to be just created, insterting new list of shields" << std::endl;
        for (std::vector <Shield>::iterator cur_st = current_state.begin(); cur_st !=current_state.end(); cur_st++){
            int id = (*cur_st).get_id();
            //std::cout << "Insert new shield: " << id << std::endl;
            QString queryInsert("INSERT INTO shields VALUES (");
            queryInsert.append(std::to_string(id).c_str());
            queryInsert.append(");");
            if(query.exec(queryInsert)){
                std::cout << "Inserted new shield: " << id << " with query: " << queryInsert.toLocal8Bit().data() << std::endl;
            }
        }
    }
    return true;
}

bool MainWindow::populate_shield_list(QSqlDatabase database){
    if(!database.isOpen()){
        std::cout << "Database not opened"<< std::endl;
        return false;
    }
    query = QSqlQuery(database);
    query.exec("SELECT * FROM shields;");
    while(query.next()){
        //if(query.value(0).toInt()==0){
        std::cout << "Shield number " << query.value(0).toInt() << std::endl;
        QString str("Shield ");
        str.append(query.value(0).toString());
        QListWidgetItem *tmp_item = new QListWidgetItem(str, 0, 1);
        tmp_item->setData(Qt::UserRole, QVariant(query.value(0).toInt()));
        checkboxShieldList.push_back(*tmp_item);
        ui->shieldList->addItem(tmp_item);
    }

    std::cout << "add item to list" << std::endl;
    QString str("shield2");
    //item1 = new QListWidgetItem(str, ui->shieldList, 1);
    //ui->shieldList->addItem(item1);
    //ui->shieldList->addItem("shield1");
    //QListWidgetItem *item = ui->shieldList->takeItem(ui->shieldList->currentRow());
    //item->clone();
    //QListWidgetItem item5 = *item;
    //item5.setText("shield4");
    //item->setText("shield3");
    //ui->shieldList->addItem(item);
    //ui->shieldList->addItem();
    return true;
}

/** Slot for UI utton action. Update plots accordingly to selected shields.
 * @brief MainWindow::shieldClicked
 * @param item Clicked shield.
 */
void MainWindow::shieldClicked(QListWidgetItem* item){
    int shield_id = item->data(Qt::UserRole).toInt();
    std::cout << "Clicked " << item->text().toLocal8Bit().data() << " with data " << shield_id << std::endl;
    if(ui->customPlot->graphCount()!=0){
        int current = ui->customPlot->graph(0)->property("ID").toInt();
        //std::cout << "Last used shield: " << current << std::endl;
        if(current == shield_id){
            return;
        } else {
            //int found = ui->shieldList->item(current-1)->data(Qt::UserRole).toInt();
            //std::cout << "Deselect last shield, at pos "<< std::to_string(current-1) << " found " << std::to_string(found) << std::endl;
            ui->shieldList->item(current-1)->setBackgroundColor(Qt::white);
            ui->shieldList->item(current-1)->setForeground(Qt::black);
        }
    }
    if(!db.isOpen()){
        std::cout << "Database not opened"<< std::endl;
        return;
    }
    std::cout << "Working on: " << db.databaseName().toLocal8Bit().data() << std::endl;
    item->setForeground(Qt::green);
    item->setBackgroundColor(Qt::lightGray);
    query = QSqlQuery(db);
    QString query_str("SELECT * FROM states WHERE shield=");
    query_str.append(QString(std::to_string(shield_id).c_str()));
    query_str.append(";");
    std::cout << "Query to exec: " << query_str.toLocal8Bit().data() << std::endl;
    if(query.exec(query_str)){
        std::cout << "Query executed succesfully"<< std::endl;
    } else {
        std::cout << "Query execution error"<< std::endl;
        return;
    }

    QVector<double> time, press2, press3, ramstroke, coalLine, suppPos, convPos;
    int max_val=0, tmp_val;
    int occ=0;
    while(query.next()){
        /*if(query.first()){
            int begin = query.value(0).toInt();
            std::cout << "Begin at time: " << begin << std::endl;
        }
        if(query.last()){
            int end = query.value(0).toInt();
            std::cout << "End at time: " << end << std::endl;
            //std::cout << "Occurences: " << occ << std::endl;
        }*/
        //std::cout << "Shield time: " << query.value(0).toString().toLocal8Bit().data() << ", int: " << query.value(0).toInt() << ", long: " << query.value(0).toLongLong() << std::endl;
        time.push_back(query.value(0).toLongLong()/1000);
        tmp_val = query.value(2).toInt();
        if(tmp_val>max_val){
            max_val=tmp_val;
        }
        press2.push_back(tmp_val);
        tmp_val = query.value(3).toInt();
        if(tmp_val>max_val){
            max_val=tmp_val;
        }
        press3.push_back(tmp_val);
        ramstroke.push_back(query.value(4).toInt());
        // division to change integer value to meters
        coalLine.push_back(query.value(5).toInt()/1000);
        suppPos.push_back(query.value(6).toInt()/1000);
        convPos.push_back(query.value(7).toInt()/1000);
        occ++;
    }
    std::cout << "Occurences: " << occ << std::endl;
    std::cout << "Time length: " << time.length() << std::endl;
    // zoom and interaction disabled, adjust plot axis at every data reload
    /*
     if(ui->customPlot->graphCount()==0){
        std::cout << "Begin time: " << time[0] << ", end time: " << time[time.size()-1] <<  std::endl;
        ui->customPlot->xAxis->setRange(time[0]-3600, time[time.size()-1]+3600);
        ui->customPlot->yAxis->setRange(0, 450);
    }
    */

    ui->customPlot->yAxis->setRange(-50, max_val+50);
    ui->customPlot->xAxis->setRange(time[0]-3600, time[time.size()-1]+3600);
    ui->customPlot->clearGraphs();

    QPen pen;
    int i;
    ui->customPlot->addGraph();
    i = ui->customPlot->graphCount()-1;
    ui->customPlot->graph(i)->setData(time, press2);
    ui->customPlot->graph(i)->setProperty("ID",QVariant(shield_id));
    ui->customPlot->graph(i)->setName("Ciśnienie 2");
    pen.setColor(QColor(255, 0, 0));
    ui->customPlot->graph()->setPen(pen);
    //ui->customPlot->graph(i)->setLineStyle((QCPGraph::LineStyle)(i+1));
    //customPlot->graph()->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 5));
    //ui->customPlot->graph(ui->customPlot->graphCount()-1)->addToLegend();

    //customPlot->addGraph(customPlot->xAxis2, customPlot->yAxis2);
    //customPlot->graph(2)->setPen(QPen(Qt::blue));
    //customPlot->graph(2)->setName("High frequency sine");

    ui->customPlot->addGraph();
    i = ui->customPlot->graphCount()-1;
    ui->customPlot->graph(i)->setData(time, press3);
    ui->customPlot->graph(i)->setProperty("ID",QVariant(shield_id));
    ui->customPlot->graph(i)->setName("Ciśnienie 3");
    pen.setColor(QColor(0, 255, 0));
    ui->customPlot->graph()->setPen(pen);

    /*ui->customPlot->addGraph();
    i = ui->customPlot->graphCount()-1;
    ui->customPlot->graph(i)->setData(time, ramstroke);
    ui->customPlot->graph(i)->setProperty("ID",QVariant(shield_id));
    ui->customPlot->graph(i)->setName("Droga przesuwnika");
    pen.setColor(QColor(0, 0, 255));
    ui->customPlot->graph()->setPen(pen);*/

    ui->customPlot->addGraph(ui->customPlot->xAxis, ui->customPlot->yAxis2);
    i = ui->customPlot->graphCount()-1;
    ui->customPlot->graph(i)->setData(time, suppPos);
    ui->customPlot->graph(i)->setProperty("ID",QVariant(shield_id));
    ui->customPlot->graph(i)->setName("Pozycja obudowy");
    pen.setColor(QColor(255, 0, 255));
    ui->customPlot->graph()->setPen(pen);

    ui->customPlot->yAxis2->setRange(suppPos[0]-10, suppPos[suppPos.length()-1]+10);

    /*ui->customPlot->addGraph();
    i = ui->customPlot->graphCount()-1;
    ui->customPlot->graph(i)->setData(time, convPos);
    ui->customPlot->graph(i)->setProperty("ID",QVariant(shield_id));
    ui->customPlot->graph(i)->setName("Pozycja przenośnika");
    pen.setColor(QColor(0, 255, 255));
    ui->customPlot->graph()->setPen(pen);*/

    /*ui->customPlot->addGraph();
    i = ui->customPlot->graphCount()-1;
    ui->customPlot->graph(i)->setData(time, coalLine);
    ui->customPlot->graph(i)->setProperty("ID",QVariant(shield_id));
    ui->customPlot->graph(i)->setName("Linia węgla");
    pen.setColor(QColor(255, 255, 0));
    ui->customPlot->graph()->setPen(pen);*/

   /* ui->customPlot->addGraph();
    i = ui->customPlot->graphCount()-1;
    ui->customPlot->graph(i)->setData(time, coalLine);
    ui->customPlot->graph(i)->setProperty("ID",QVariant(shield_id));
    ui->customPlot->graph(i)->setName("Linia węgla");
    pen.setColor(QColor(qSin((i+1)*1+1.2)*80+80, qSin((i+1)*0.3+0)*80+80, qSin((i+1)*0.3+1.5)*80+80));
    ui->customPlot->graph()->setPen(pen);

    ui->customPlot->addGraph();
    i = ui->customPlot->graphCount()-1;
    ui->customPlot->graph(i)->setData(time, suppPos);
    ui->customPlot->graph(i)->setProperty("ID",QVariant(shield_id));
    ui->customPlot->graph(i)->setName("Pozycja obudowy");
    pen.setColor(QColor(qSin((i+1)*1+1.2)*80+80, qSin((i+1)*0.3+0)*80+80, qSin((i+1)*0.3+1.5)*80+80));
    ui->customPlot->graph()->setPen(pen);

    ui->customPlot->addGraph();
    i = ui->customPlot->graphCount()-1;
    ui->customPlot->graph(i)->setData(time, convPos);
    ui->customPlot->graph(i)->setProperty("ID",QVariant(shield_id));
    ui->customPlot->graph(i)->setName("Pozycja przenośnika");
    pen.setColor(QColor(qSin((i+1)*1+1.2)*80+80, qSin((i+1)*0.3+0)*80+80, qSin((i+1)*0.3+1.5)*80+80));
    ui->customPlot->graph()->setPen(pen);
*/
    ui->customPlot->replot();

}

/** Slot for UI button action. Select file and check if it is compatible with VShield standard.
 * @brief MainWindow::openVShieldFile
 */
void MainWindow::openVShieldFile(){
    QString filename = QFileDialog::getOpenFileName(this, tr("Otwórz plik VShield"),tr(""),tr("Pliki VShield (*.LogVShield10)"));
    vShieldReader = new VShieldReader(filename.toStdString());
    if(vShieldReader->get_vshield_file_state()){
        QString statuBarMessage("Wybrany plik: ");
        statuBarMessage.append(vShieldReader->get_vshield_file_name().c_str());
        statusBar()->showMessage(statuBarMessage,0);
        vshield_selected = true;
    } else {
        vshield_selected = false;
    }
}

/** Slot for UI button action. Extract data from VShield file and save it to database.
 * @brief MainWindow::vShieldAnalyze
 */
void MainWindow::vShieldAnalyze(){
    std::vector <FaceState> facestates;
    facestates = vShieldReader->extract_data();
    if(facestates.size()==0){
        QString qStr("Błąd pliku VShield");
        statusBar()->showMessage(qStr,0);
    } else{
        std::cout << "Entries found: " << facestates.size() << std::endl;
        int insert_counter = 0;
        check_shields_table(facestates.at(0).get_state());
        for (std::vector<FaceState>::iterator fs = facestates.begin() ; fs != facestates.end(); ++fs){
            //std::cout << (*fs).get_status() << std::endl;
            std::vector <Shield> current_state = (*fs).get_state();
            QDateTime current_timestamp = (*fs).get_timestamp();
            int64_t timestamp_epoch = current_timestamp.toMSecsSinceEpoch();
            QString queryCheckTS("SELECT * FROM timestamps WHERE timestamp='");
            queryCheckTS.append(std::to_string(timestamp_epoch).c_str());
            queryCheckTS.append("';");
            query.exec(queryCheckTS);
            if(query.next()){
                std::cout << "Timestamp found in database, skip updating" << std::endl;
            } else {
                std::cout << "Inserting face state " << insert_counter << " of " << facestates.size() << " with timestamp " << timestamp_epoch << std::endl;
                insert_counter++;
                QString queryInsertTS( "INSERT INTO timestamps VALUES (" );
                queryInsertTS.append(std::to_string(timestamp_epoch).c_str());
                queryInsertTS.append( ");" );
                if(query.exec(queryInsertTS)){
                    query.exec("BEGIN");
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
                        if(query.exec(queryInsertShield)){
                            //std::cout << "Inserted new shield: " << id << " with query: " << queryInsertShield.toLocal8Bit().data() << std::endl;
                        }
                    }
                    query.exec("COMMIT");
                } else {
                    std::cout << "Unable to add new timestamp int table" << std::endl;
                }
            }
        }
    }
    vshield_selected = false;
}

/** Slot for UI button action. Create new database or overwrite existing one.
 * @brief MainWindow::newDatabase
 */
void MainWindow::newDatabase(){
    QString database_fileName = QFileDialog::getSaveFileName(this, tr("Utwórz bazę danych"),tr(""),tr("Baza danych SQlite (*.db3)"));
    if(database_fileName.isEmpty()){
        std::cout << "Brak wybranego pliku" << std::endl;
        statusBar()->showMessage("Nie wybrano nowego pliku",0);
        db_selected = false;
        return;
    } else {
        QFile *database_file = new QFile(database_fileName);
        if(database_file->exists()){
            QMessageBox::StandardButton reply;
            QString message = "Wybrano opcję utworzenia bazy danych na już istniejącej. Kontunuacja spowoduje utratę danych zawartych w istniejącej bazie.";
            reply = QMessageBox::question(this, "Usunięcie bazy danych", message, QMessageBox::Yes|QMessageBox::No);
            if (reply == QMessageBox::Yes) {
                database_file->remove();
                open_database(database_fileName, true);
            } else {
                std::cout << "Brak zgody na usunięcie bazy" << std::endl;
                statusBar()->showMessage("Nie wybrano bazy danych",0);
                db_selected = false;
                return;
            }
        } else {
            open_database(database_fileName, true);
        }
    }
}

/** Slot for UI button action. Open existing database file.
 * @brief MainWindow::openDatabase
 */
void MainWindow::openDatabase(){
    QString database_fileName = QFileDialog::getOpenFileName(this, tr("Otwórz bazę danych"),tr(""),tr("Baza danych SQlite (*.db3)"));
    if(database_fileName.isEmpty()){
        std::cout << "Brak wybranego pliku" << std::endl;
        statusBar()->showMessage("Nie wybrano żadnego pliku",0);
        db_selected = false;
        return;
    } else {
        QFile *database_file = new QFile(database_fileName);
        if(database_file->exists()){
            if(open_database(database_fileName)){
                std::cout << "Otwarto bazę danych" << std::endl;
                statusBar()->showMessage("Otwarto bazę danych",0);
                populate_shield_list(db);
            } else {
                std::cout << "Błąd otwierania bazy danych." << std::endl;
                statusBar()->showMessage("Błąd otwierania bazy danych.",0);
            }
        } else {
            std::cout << "Plik nie istnieje" << std::endl;
            statusBar()->showMessage("Plik nie istnieje",0);
        }
    }
}

/** Open selected database file.
 * @brief MainWindow::open_database
 * @param database_name Name of database file to open.
 * @return True if database was successfully opened, false otherwise.
 */
bool MainWindow::open_database(QString database_name){
    return open_database(database_name, false);
}

/** Open or create selected database file.
 * @brief MainWindow::open_database
 * @param database_name Name of database file to open.
 * @param init Define if create new database, will overwrite existing data.
 * @return True if database was successfully opened, false otherwise.
 */
bool MainWindow::open_database(QString database_name, bool init){
    //QSqlDatabase db_tmp;
    std::cout << "Otwieranie bazy danych" << std::endl;
    db = QSqlDatabase::addDatabase("QSQLITE", "SQLITE");
    //std::cout << "Utworzono wskaźnik" << std::endl;
    db_file = database_name;
    db.setDatabaseName(db_file);
    //std::cout << "Określono nazwę bazy danych" << std::endl;
    // default value
    //db.setDatabaseName("C:/VShield_10_77_Build_6125/vShield.db3");
    if(init){
        db.open();
        query = QSqlQuery(db);
        QString queryStr = "PRAGMA application_id = ";
        queryStr.append(std::to_string(DATABASE_APP_ID).c_str());
        queryStr.append(";");
        if(query.exec(queryStr)){
            std::cout << "application id set with query: " << queryStr.toLocal8Bit().data() << std::endl;
        } else {
            std::cout << "unable to set application id" << std::endl;
        }
        if(query.exec("CREATE TABLE timestamps(timestamp INTEGER PRIMARY KEY);")){
            std::cout << "Table timestamps created" << std::endl;
        } else {
            std::cout << "Error 1" << std::endl;
        }
        if(query.exec("CREATE TABLE shields(shield_number INTEGER PRIMARY KEY);")){
            std::cout << "Table shields created" << std::endl;
        } else {
            std::cout << "Error 2" << std::endl;
        }
        if(query.exec("CREATE TABLE states(time INT, shield INT, pressure2 INT, pressure3 INT, ramstroke INT, coal_line INT, support_pos INT, conveyor_pos INT, FOREIGN KEY(time) REFERENCES timestamps(timestamp), FOREIGN KEY(shield) REFERENCES shields(shield_number));")){
            std::cout << "Table states created" << std::endl;
        } else {
            std::cout << "Error 3" << std::endl;
        }
    }
    if(check_db_integrity(db)){
        db_selected = true;
        return true;
    } else {
        db_selected = false;
        return false;
    }
}

/** Check if provided database object was created with this application and properly configured.
 * @brief MainWindow::check_db_integrity
 * @param database Database object to check.
 * @return
 */
bool MainWindow::check_db_integrity(QSqlDatabase database){
    if(!database.isOpen()){
        if(!database.open()){
            std::cout << "ERROR while opening db"<< std::endl;
            return false;
        }
    }
    query = QSqlQuery(database);
    query.exec("PRAGMA foreign_keys;");
    if(query.next()){
        //QString status = query.value(0).toString();
        //std::cout << "foreign_keys: " << status.toLocal8Bit().constData() << std::endl;
        if(query.value(0).toInt()==0){
            if(query.exec("PRAGMA foreign_keys = ON;")){
                //std::cout << "foreign_keys turned ON "<< std::endl;
            } else {
                //std::cout << "ERROR while turning on foreign_keys "<< std::endl;
                return false;
            }
        }
        query.exec("PRAGMA application_id;");
        if(query.next()){
            //QString status = query.value(0).toString();
            //std::cout << "Application_id: " << status.toLocal8Bit().constData() << std::endl;
            if(query.value(0).toInt()==DATABASE_APP_ID){
                return true;
            }
        }
    }
    return false;
}

/** Slot for menu showup action. Sets active or inactive appropriate buttons.
 * @brief MainWindow::setActiveActions
 */
void MainWindow::setActiveActions(){
    //std::cout << "Set active menu items"<<std::endl;
    if(db_selected){
        qAction_openVShield->setEnabled(true);
    } else {
        qAction_openVShield->setEnabled(false);
    }
    if(db_selected && vshield_selected){
        qAction_analyze_vshield->setEnabled(true);
    } else {
        qAction_analyze_vshield->setEnabled(false);
    }
}

enum MainWindow::shield_params{
    press_1 = 1,
    press_2 = 2,
    ramstrk = 3,
    coalLn = 4,
    supp_Pos = 5,
    conv_Pos = 6
};

MainWindow::~MainWindow()
{
    if(db.isOpen()){
        db.close();
    }
    delete ui;
}
