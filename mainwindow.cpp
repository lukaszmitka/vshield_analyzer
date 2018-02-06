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

    qAction_openVShield = new QAction(tr("&Otwórz plik VShield"), this);
    //qAction_openVShield = new QAction("Otwórz plik VShield");
    qAction_openVShield->setStatusTip("Otwórz istniejący plik");
    connect(qAction_openVShield, SIGNAL(triggered()), this, SLOT(openVShieldFile()));

    qAction_openVShieldFolder = new QAction(tr("Otwórz folder VShield"), this);
    qAction_openVShieldFolder->setStatusTip("Otwórz folder zawierający pliki VShield");
    connect(qAction_openVShieldFolder, SIGNAL(triggered()), this, SLOT(openVShieldFolder()));

    //connect(myAction,SIGNAL(triggered()), this, SLOT(myFileFunction()));

    qAction_analyze_vshield = new QAction(tr("&Analizuj plik"), this);
    qAction_analyze_vshield->setStatusTip("Tymczasowa funkcja do analizy VShield");
    connect(qAction_analyze_vshield, SIGNAL(triggered()), this, SLOT(vShieldAnalyze()));

    qAction_close = new QAction(tr("&Zamknij"), this);
    qAction_close->setStatusTip("Zakończ program");
    connect(qAction_close, SIGNAL(triggered()), this, SLOT(close()));

    qAction_select_existing_db = new QAction(tr("&Otwórz bazę danych"), this);
    QString *sel_db_statusTip = new QString;
    sel_db_statusTip->append("Otwiera istniejącą bazę danych");
    if(!db_file.isEmpty()){
        sel_db_statusTip->append(" (obecnie otwarta: ");
        sel_db_statusTip->append(db_file);
    }
    qAction_select_existing_db->setStatusTip(*sel_db_statusTip);
    connect(qAction_select_existing_db, SIGNAL(triggered()), this, SLOT(openDatabase()));

    qAction_create_db = new QAction(tr("&Stwórz bazę danych"), this);
    QString *new_db_ststusTip = new QString;
    new_db_ststusTip->append("Tworzy nową bazę danych");
    if(!db_file.isEmpty()){
        new_db_ststusTip->append(" (obecnie otwarta: ");
        new_db_ststusTip->append(db_file);
    }
    qAction_create_db->setStatusTip(*new_db_ststusTip);
    connect(qAction_create_db, SIGNAL(triggered()), this, SLOT(newDatabase()));

    qAction_pressure_limit = new QAction(tr("Określ limit ciśnienia"), this);
    qAction_pressure_limit->setStatusTip("Określa nowe ograniczenia ciśnień dla początku postoju sekcji.");
    connect(qAction_pressure_limit, SIGNAL(triggered()), this, SLOT(dialogGetPressureLimits()));


    qAction_stay_time = new QAction(tr("Określ czas postoju"), this);
    qAction_stay_time->setStatusTip("Określa nowe ograniczenia czasu postoju sekcji.");
    connect(qAction_stay_time, SIGNAL(triggered()), this, SLOT(dialogGetStayTime()));

    qAction_press_index = new QAction(tr("Oblicz wskaźnik przyrostu ciśnienia"), this);
    qAction_press_index->setStatusTip("Oblicza wskaźnik przyrostu ciśnienia dla określonych sekcji");
    connect(qAction_press_index, SIGNAL(triggered()), this, SLOT(determinePressureIndex()));

    qAction_get_compressive_strengths = new QAction(tr("Określ odporność na ściskanie"), this);
    qAction_get_compressive_strengths->setStatusTip("Pozwala określić odporność skał na ściskanie na danym obszarze");
    connect(qAction_get_compressive_strengths, SIGNAL(triggered()), this, SLOT(dialogGetCompressiveStrengths()));

    qAction_export = new QAction(tr("Eksport do pliku csv"), this);
    qAction_export->setStatusTip("Eksportuje obliczone wskaźniki ciśnienia do pliku .csv (Do obróbki w Excelu lub Matlabie");
    connect(qAction_export, SIGNAL(triggered()), this, SLOT(export_to_csv()));

    qAction_clear_pressure_table = new QAction(tr("Wyczyść tabelę wskaźników ciśnienia"), this);
    qAction_clear_pressure_table->setStatusTip("Usuwa wpisy z tabeli wskaźników ciśnienia");
    connect(qAction_clear_pressure_table, SIGNAL(triggered()), this, SLOT(clear_index_table()));

    qAction_set_day_shift_begin_time = new QAction(tr("Określ godzinę rozpoczęcia dnia"));
    qAction_set_day_shift_begin_time->setStatusTip("Określa godzinę według której obliczany jest dzienny postęp ściany i wskaźnik przyrostu ciśnienia.");
    connect(qAction_set_day_shift_begin_time, SIGNAL(triggered()), this, SLOT(dialogSetDayShifBeginTime()));

    qAction_calculate_wall_progress = new QAction(tr("Oblicz średni postęp ściany"));
    qAction_calculate_wall_progress->setStatusTip("Oblicza średni dzienny postęp ściany dla całego przebiegu zapisanego w bazie.");
    connect(qAction_calculate_wall_progress, SIGNAL(triggered()), this, SLOT(calculateWallProgress()));

    qAction_calculate_avg_press_index = new QAction(tr("Oblicz średni wskaźnik przyrostu ciśnienia"));
    qAction_calculate_avg_press_index->setStatusTip("Oblicza średni dzienny wskaźnik przyrostu ciśnienia dla całego przebiegu zapisanego w bazie.");
    connect(qAction_calculate_avg_press_index, SIGNAL(triggered()), this, SLOT(calculateAvgPressIndex()));

    fileMenu = menuBar()->addMenu(tr("&Plik"));
    fileMenu->addAction(qAction_select_existing_db);
    fileMenu->addAction(qAction_create_db);
    fileMenu->addSeparator();
    fileMenu->addAction(qAction_openVShield);
    fileMenu->addAction(qAction_openVShieldFolder);
    //fileMenu->addAction(qAction_analyze_vshield);
    fileMenu->addSeparator();
    fileMenu->addAction(qAction_pressure_limit);
    fileMenu->addAction(qAction_stay_time);
    fileMenu->addAction(qAction_press_index);
    fileMenu->addAction(qAction_calculate_avg_press_index);
    fileMenu->addSeparator();
    fileMenu->addAction(qAction_get_compressive_strengths);
    fileMenu->addAction(qAction_set_day_shift_begin_time);
    fileMenu->addAction(qAction_calculate_wall_progress);
    fileMenu->addSeparator();
    fileMenu->addAction(qAction_export);
    fileMenu->addAction(qAction_clear_pressure_table);
    fileMenu->addSeparator();
    fileMenu->addAction(qAction_close);
    connect(fileMenu, SIGNAL(aboutToShow()),this, SLOT(setActiveActions()));

    populate_shield_list(db);
    connect(ui->shieldList, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(shieldClicked(QListWidgetItem*)));

    vShieldReader = new VShieldReader();
    //QCoreApplication.addLibraryPath(".");
}

void MainWindow::calculateAvgPressIndex(){
    QString select_min_query = "SELECT min(end_time) FROM pressure_index;";
    QString select_max_query = "SELECT max(end_time) FROM pressure_index;";
    QString insert_query;
    int day_begin_time = get_dayBeginTime_From_DB();
    if(day_begin_time==-1){
        std::cout << "Error, day begin time not set" << std::endl;
        return;
    }
    long long min_timestamp = 0;
    long long max_timestamp = 0;
    long long previous_timestamp = 0;
    long long next_timestamp = 0;
    if(query.exec(select_min_query)){
        if(query.next()){
            min_timestamp = query.value(0).toLongLong();
        }
    }
    if(query.exec(select_max_query)){
        if(query.next()){
            max_timestamp = query.value(0).toLongLong();
        }
    }
    if(min_timestamp != 0 && max_timestamp != 0){
        if(min_timestamp < max_timestamp){
            previous_timestamp = min_timestamp;
            while (previous_timestamp<max_timestamp){
                next_timestamp = get_nextDay(previous_timestamp, day_begin_time);
                double avgPressInd = get_averagePressureIndex(previous_timestamp, next_timestamp);
                insert_query.clear();
                insert_query.append("INSERT INTO average_pressure_index VALUES (");
                insert_query.append(QString::number(previous_timestamp));
                insert_query.append(", ");
                insert_query.append(QString::number(next_timestamp));
                insert_query.append(", ");
                insert_query.append(std::to_string(avgPressInd).c_str());
                insert_query.append(");");
                std::cout << "query to execute: " << insert_query.toLocal8Bit().data() << std::endl;
                if(query.exec(insert_query)){
                    std::cout << "Succesfully inserted entry" << std::endl;
                }
                previous_timestamp = next_timestamp+1;
            }
        }
    }
}


/**
 * @brief MainWindow::get_averagePressureIndex Calculate average pressure index in a given time.
 * @param beginTimestamp Beginning of the search period.
 * @param endTimestamp End of the search period.
 * @return Average pressure index.
 */
double MainWindow::get_averagePressureIndex(long long beginTimestamp, long long endTimestamp){
    double avg_press_index = 0;
    double avg_press_index_sum = 0;
    int entries_count=0;
    QString select_query = "SELECT * FROM pressure_index WHERE end_time > '";
    select_query.append(QString::number(beginTimestamp));
    select_query.append("' AND end_time < '");
    select_query.append(QString::number(endTimestamp));
    select_query.append("';");
    std::cout << "Calculating average pressure index with query: " << select_query.toLocal8Bit().data() << std::endl;
    if (query.exec(select_query)){
        while (query.next()){
            avg_press_index_sum += query.value(3).toDouble();
            entries_count++;
        }
        std::cout << "Query found " << entries_count << " entries" << std::endl;
        avg_press_index = avg_press_index_sum/entries_count;
        std::cout << "Average pressure is: " << avg_press_index << std::endl;
    }

    if(entries_count==0){
        avg_press_index=-1;
    }
    return avg_press_index;
}


void MainWindow::calculateWallProgress(){
    QString select_min_query = "SELECT min(time) FROM states;";
    QString select_max_query = "SELECT max(time) FROM states;";
    QString insert_query;

    int day_begin_time = get_dayBeginTime_From_DB();
    if(day_begin_time==-1){
        std::cout << "Error, day begin time not set" << std::endl;
        return;
    }
    long long min_timestamp = 0;
    long long max_timestamp = 0;
    long long previous_timestamp = 0;
    long long next_timestamp = 0;
    if(query.exec(select_min_query)){
        if(query.next()){
            min_timestamp = query.value(0).toLongLong();
        }
    }
    if(query.exec(select_max_query)){
        if(query.next()){
            max_timestamp = query.value(0).toLongLong();
        }
    }

    if(min_timestamp != 0 && max_timestamp != 0){
        if(min_timestamp < max_timestamp){
            previous_timestamp = min_timestamp;
            while (previous_timestamp<max_timestamp){
                next_timestamp = get_nextDay(previous_timestamp, day_begin_time);
                double progress = get_averageCoalLineProgress(previous_timestamp, next_timestamp);
                std::cout << "Progress for time: " << previous_timestamp << "--" << next_timestamp << ": " << progress << std::endl;
                insert_query.clear();
                insert_query.append("INSERT INTO average_progress VALUES (");
                insert_query.append(QString::number(previous_timestamp));
                insert_query.append(", ");
                insert_query.append(QString::number(next_timestamp));
                insert_query.append(", ");
                insert_query.append(QString::number(progress));
                insert_query.append(");");
                std::cout << "query to execute: " << insert_query.toLocal8Bit().data() << std::endl;
                if(query.exec(insert_query)){
                    std::cout << "Succesfully inserted entry" << std::endl;
                }
                previous_timestamp = next_timestamp+1;
            }
        }
    }
}

long long MainWindow::get_nextDay(long long timestamp, int dayBeginHour){
    std::cout << "Received timestamp: " << timestamp << std::endl;
    long long timestamp_raw = timestamp - ((long long) dayBeginHour * 60 * 60 * 1000);
    long long full_days = timestamp_raw/(24* 60 * 60 * 1000);
    full_days++;
    timestamp_raw = (full_days*24* 60 * 60 * 1000) + ((long long) dayBeginHour * 60 * 60 * 1000);
    std::cout << "Generated timestamp: " << timestamp_raw << std::endl;
    return timestamp_raw;
}

void MainWindow::dialogSetDayShifBeginTime(){
    int default_day_begin_time = 6;
    int tmp = get_dayBeginTime_From_DB();
    if(tmp>=0){
        default_day_begin_time=tmp;
    }
    bool ok;
    int day_begin = QInputDialog::getInt(this, tr("Określ godzinę rozpoczęcia dnia"),
                                         tr("Wartość będzie brana pod uwagę podczas obliczania dziennych statystyk."), default_day_begin_time, 0, 24, 1, &ok);
    if(ok){
        std::cout << "Wybrana godzina: " << day_begin << std::endl;
        update_dayBeginTimeInDB(day_begin);
    }
}

/**
 * @brief MainWindow::get_shieldCoalLineProgress Retrieve from database coal line progress in a given time.
 * @param shieldID Shield for which progress should be received.
 * @param beginTimestamp Beginning of the search period.
 * @param endTimestamp End of the search period.
 * @return Distance travelled by shield, expressed in meters.
 */
double MainWindow::get_shieldCoalLineProgress(int shieldID, long long beginTimestamp, long long endTimestamp){
    QString select_query = "SELECT * FROM states WHERE shield = '";
    select_query.append(QString::number(shieldID));
    select_query.append("' AND time > '");
    select_query.append(QString::number(beginTimestamp));
    select_query.append("' AND time < '");
    select_query.append(QString::number(endTimestamp));
    select_query.append("' ORDER BY coal_line ASC;");
    long long beginCoalLine = 0;
    long long endCoalLine = 0;
    std::cout << "Obliczenie postępu sekcji z zapytaniem: " << select_query.toLocal8Bit().data() << std::endl;
    if (query.exec(select_query)){
        while(query.next() && beginCoalLine==0){
            beginCoalLine = query.value(5).toInt();
        }
        while (query.next()){
            endCoalLine = query.value(5).toInt();
        }
    }
    std::cout << "Wynik: " << ((double)(endCoalLine-beginCoalLine))/1000 << std::endl;
    return ((double)(endCoalLine-beginCoalLine))/1000;
}

/**
 * @brief MainWindow::get_averageCoalLineProgress Calculate average coal line progress in a given time.
 * @param beginTimestamp Beginning of the search period.
 * @param endTimestamp End of the search period.
 * @return Average distance travelled by all shields, expressed in meters.
 */
double MainWindow::get_averageCoalLineProgress(long long beginTimestamp, long long endTimestamp){
    double progress_sum = 0;
    for (int i=1; i<=number_of_shields; i++){
        progress_sum+=get_shieldCoalLineProgress(i, beginTimestamp, endTimestamp);
    }
    std::cout << "Średni postęp ściany dla zadanego przedziału czasowego: " << progress_sum/number_of_shields << std::endl;
    return progress_sum/number_of_shields;
}

/**
 * @brief MainWindow::get_dayBeginTime_From_DB Retrieve day begin time from database.
 * @return Hour of selected day shif begin as hour between 0 and 23, -1 if error or unset.
 */
int MainWindow::get_dayBeginTime_From_DB(){
    int day_begin_time = -1;
    QString select_query = "SELECT * FROM app_config WHERE param = 'DAY_BEGIN_TIME';";
    if(query.exec(select_query)){
        if(query.next()){
            day_begin_time = query.value(1).toInt();
            std::cout << "Time from db: " << day_begin_time << std::endl;
        }
    }
    return day_begin_time;
}

void MainWindow::update_dayBeginTimeInDB(int dayBeginTime){
    if (dayBeginTime>=0 && dayBeginTime<=24){
        if (dayBeginTime==24){
            dayBeginTime=0;
        }
        QString update_query = "UPDATE app_config SET value = '";
        update_query.append(QString::number(dayBeginTime));
        update_query.append("' WHERE param = 'DAY_BEGIN_TIME';");
        if(query.exec(update_query)){
            std::cout << "Updated day begin time" << std::endl;
        }
    }
}


void MainWindow::export_to_csv(){
    bool exportIntegral;
    bool exportRawData;
    bool exportDerivative;
    int interval;

    ExportCSVDialog exportDialog;
    if(exportDialog.exec()){
        if(exportDialog.getExportProcessedData())
        {
            exportDialog.getDataToProcess(&exportIntegral, &exportRawData, &exportDerivative, &interval);
            if(exportIntegral){
                std::cout << "Will export integral" << std::endl;
            }
            if(exportRawData){
                std::cout << "Will export raw data with interval " << interval << std::endl;
                //std::cout << "Interval counter: " << intervalCounter << std::endl;
            }
            if(exportDerivative){
                std::cout << "Will export derivative with interval " << interval << std::endl;
                //std::cout << "Interval counter: " << intervalCounter << std::endl;
            }
            if(!exportIntegral && !exportRawData && !exportDerivative){
                std::cout << "No data to export selected" << std::endl;
                QString statuBarMessage("No processed data to export selected");
                statusBar()->showMessage(statuBarMessage,0);
            } else {
                exportProcessedData(exportDerivative, exportRawData, exportIntegral, interval);
            }
        }
        if(exportDialog.getExportRawColumns()){
            exportRawColumns(interval);
        }
        if(exportDialog.getExportStackedColumns()){
            exportStackedColumns(interval);
        }
    } else {
        return;
    }
}

void MainWindow::exportRawColumns(int interval){
    int intervalCounter = interval/30;
    if(intervalCounter<=0){
        intervalCounter=1;
    }
    QString filename = QFileDialog::getSaveFileName(this, tr("Wybierz plik do zapisu danych w formie szeregowej"),tr(""),tr("Pliki csv (*.csv)"));
    if(filename.isNull()){
        std::cout << "No file selected" << std::endl;
        QString statuBarMessage("Operacja zapisu danych w formie szeregowej anulowana przez użytkownika");
        statusBar()->showMessage(statuBarMessage,0);
    } else {
        QString statuBarMessage("Wybrany plik: ");
        statuBarMessage.append(filename);
        statusBar()->showMessage(statuBarMessage,0);
        QFile csv_file(filename);
        csv_file.open(QIODevice::WriteOnly);
        QTextStream csv_out(&csv_file);   // we will serialize the data into the file
        QString select_query;
        std::vector<long long> timestampsVector;
        select_query.append("SELECT * FROM timestamps;");
        //std::cout << "Executing query" << std::endl;
        if(!query.exec(select_query)){
            std::cout << "Could not get timestamps from database." << std::endl;
            return;
        } else {
            while(query.next()){
                timestampsVector.push_back(query.value(0).toLongLong());
            }
        }
        QString fileHeader_line1;
        QString fileHeader_line2;
        QString line;
        QDateTime dateTime;
        fileHeader_line1.append("data; czas; ");
        fileHeader_line2.append(" ; ; ");

        for (int i=0; i<number_of_shields; i=i+intervalCounter){
            fileHeader_line1.append(std::to_string(i+1).c_str()).append("; ");
            fileHeader_line2.append("Linia ocoisu [m]; ");
            fileHeader_line1.append(std::to_string(i+1).c_str()).append("; ");
            fileHeader_line2.append("Ciśnienie C1 [MPa]; ");
            fileHeader_line1.append(std::to_string(i+1).c_str()).append("; ");
            fileHeader_line2.append("Ciśnienie C2 [MPa]; ");
            fileHeader_line1.append(std::to_string(i+1).c_str()).append("; ");
            fileHeader_line2.append("Cśnienie średnie [MPa]; ");
            fileHeader_line1.append(std::to_string(i+1).c_str()).append("; ");
            fileHeader_line2.append("Droga przesuwnika [m]; ");
        }
        csv_out << fileHeader_line1 << endl;
        csv_out << fileHeader_line2 << endl;

        for (int i=0;i<timestampsVector.size(); i++){
            //long shield;
            long long time = timestampsVector[i];
            double coalLine;
            double pressure_1;
            double pressure_2;
            double pressure_avg;
            double ramstroke;
            line.clear();
            select_query.clear();
            select_query.append("SELECT * FROM states WHERE time='");
            select_query.append(std::to_string(time).c_str());
            select_query.append("';");
            std::cout << "Selecting shied states with query: " << select_query.toLocal8Bit().data() << std::endl;
            if(!query.exec(select_query)){
                std::cout << "Could not get states for given timestamp from database." << std::endl;
                return;
            } else {
                dateTime.setMSecsSinceEpoch(time);
                line.append(dateTime.toString("dd.MM.yyyy; hh:mm:ss; ").toLocal8Bit());
                QString statuBarMessage("Przetwarzanie danych w formie szeregowej dla daty: ");
                statuBarMessage.append(dateTime.toString("dd.MM.yyyy; hh:mm:ss; ").toLocal8Bit());
                statusBar()->showMessage(statuBarMessage,0);

                while (query.next()) {
                    pressure_1 = query.value(2).toDouble()/10;
                    pressure_2 = query.value(3).toDouble()/10;
                    pressure_avg=(pressure_1+pressure_2)/2;
                    ramstroke = query.value(4).toDouble();
                    coalLine = query.value(5).toDouble()/1000;
                    line.append(std::to_string(coalLine).c_str()).append("; ");
                    line.append(std::to_string(pressure_1).c_str()).append("; ");
                    line.append(std::to_string(pressure_2).c_str()).append("; ");
                    line.append(std::to_string(pressure_avg).c_str()).append("; ");
                    line.append(std::to_string(ramstroke).c_str()).append("; ");
                }
                csv_out << line << endl;
                line.clear();
            }
        }
    }
}


void MainWindow::exportStackedColumns(int interval){
    int intervalCounter = interval/30;
    int interval_check = 0;
    if(intervalCounter<=0){
        intervalCounter=1;
    }

    QString filename = QFileDialog::getSaveFileName(this, tr("Wybierz plik do zapisu danych w formie kolumnowej"),tr(""),tr("Pliki csv (*.csv)"));
    if(filename.isNull()){
        std::cout << "No file selected" << std::endl;
        QString statuBarMessage("Operacja zapisu danych w formie kolumnowej anulowana przez użytkownika");
        statusBar()->showMessage(statuBarMessage,0);
    } else {
        QString statuBarMessage("Wybrany plik: ");
        statuBarMessage.append(filename);
        statusBar()->showMessage(statuBarMessage,0);
        QFile csv_file(filename);
        csv_file.open(QIODevice::WriteOnly);
        QTextStream csv_out(&csv_file);   // we will serialize the data into the file
        QString select_query;
        std::vector<long long> shieldsVector;
        select_query.append("SELECT * FROM shields;");
        //std::cout << "Executing query" << std::endl;
        if(!query.exec(select_query)){
            std::cout << "Could not get shields from database." << std::endl;
            return;
        } else {
            while(query.next()){
                shieldsVector.push_back(query.value(0).toLongLong());
            }
        }
        QString fileHeader;
        QString line;
        QDateTime dateTime;
        fileHeader.append("nr sekcji; data; czas; ");
        fileHeader.append("Linia ocoisu [m]; ");
        fileHeader.append("Ciśnienie C1 [MPa]; ");
        fileHeader.append("Ciśnienie C2 [MPa]; ");
        fileHeader.append("Cśnienie średnie [MPa]; ");
        fileHeader.append("Droga przesuwnika [m]; ");
        csv_out << fileHeader << endl;

        long shield;
        long long time;
        double coalLine;
        double pressure_1;
        double pressure_2;
        double pressure_avg;
        double ramstroke;

        for (int i=0;i<shieldsVector.size(); i++){
            shield = shieldsVector[i];
            line.clear();
            select_query.clear();
            select_query.append("SELECT * FROM states WHERE shield = '");
            select_query.append(std::to_string(shield).c_str());
            select_query.append("' ORDER BY time ASC;");
            std::cout << "Selecting shield states with query: " << select_query.toLocal8Bit().data() << std::endl;
            QString statuBarMessage("Przetwarzanie danych w formie kolumnowej dla obudowy: ");
            statuBarMessage.append(QString::number(shield).toLocal8Bit().data());
            statusBar()->showMessage(statuBarMessage,0);

            if(!query.exec(select_query)){
                std::cout << "Could not get states for given shield from database." << std::endl;
                return;
            } else {
                while (query.next()) {
                    if(interval_check==intervalCounter){
                        time = query.value(0).toLongLong();
                        dateTime.setMSecsSinceEpoch(time);
                        pressure_1 = query.value(2).toDouble()/10;
                        pressure_2 = query.value(3).toDouble()/10;
                        pressure_avg=(pressure_1+pressure_2)/2;
                        ramstroke = query.value(4).toDouble();
                        coalLine = query.value(5).toDouble()/1000;
                        line.append(std::to_string(shield).c_str()).append("; ");
                        line.append(dateTime.toString("dd.MM.yyyy; hh:mm:ss; ").toLocal8Bit());
                        line.append(std::to_string(coalLine).c_str()).append("; ");
                        line.append(std::to_string(pressure_1).c_str()).append("; ");
                        line.append(std::to_string(pressure_2).c_str()).append("; ");
                        line.append(std::to_string(pressure_avg).c_str()).append("; ");
                        line.append(std::to_string(ramstroke).c_str()).append("; ");
                        csv_out << line << endl;
                        line.clear();
                        interval_check=0;
                    }
                    interval_check++;
                }
            }
        }
    }
}

void MainWindow::exportProcessedData(bool exportDerivative, bool exportRawData, bool exportIntegral, int interval){
    int intervalCounter = interval/30;
    // SQLite query for selecting shield
    QString filename = QFileDialog::getSaveFileName(this, tr("Wybierz plik do zapisu danych przetworzonych"),tr(""),tr("Pliki csv (*.csv)"));
    if(filename.isNull()){
        std::cout << "No file selected" << std::endl;
        QString statuBarMessage("Operacja anulowana przez użytkownika");
        statusBar()->showMessage(statuBarMessage,0);
    } else {

        if(!db.isOpen()){
            std::cout << "Database not opened"<< std::endl;
            return;
        }
        std::cout << "Working on: " << db.databaseName().toLocal8Bit().data() << std::endl;
        average_pressure_index_query = QSqlQuery(db);
        average_progress_query = QSqlQuery(db);
        compressive_strength_query = QSqlQuery(db);
        QString average_pressure_index_query_body;
        QString average_progress_query_body;
        QString compressive_strength_query_body;
        double average_pressure_index;
        double average_progress;
        long long compressive_strength;

        QString statuBarMessage("Wybrany plik: ");
        statuBarMessage.append(filename);
        statusBar()->showMessage(statuBarMessage,0);
        QFile csv_file(filename);
        csv_file.open(QIODevice::WriteOnly);
        QTextStream csv_out(&csv_file);   // we will serialize the data into the file
        QString select_query;
        select_query.append("SELECT * FROM pressure_index;");
        //std::cout << "Executing query" << std::endl;
        query.exec(select_query);
        long shield;
        long long begin_time;
        long long end_time;
        float pressure_integral;
        int stoppage_entries;
        double coalLine;
        long long stoppage_duration;
        QString fileHeader;
        if(exportDerivative){
            fileHeader.append("Przebieg pochodnej ciśnienia zapisany jest w jednej linii, poprzedzony jest wpisem 'dP/dt'");
            csv_out << fileHeader << endl;
            fileHeader.clear();
        }
        if(exportRawData){
            fileHeader.append("Przebieg ciśnienia zapisany jest w jednej linii, poprzedzony jest wpisem 'P(t)'");
            csv_out << fileHeader << endl;
            fileHeader.clear();
        }
        fileHeader.append("Numer obudowy; początek przestoju; koniec przestoju; czas przestoju [minuty]; linia węgla [m]; średni postęp ściany [m]; średni wskaźnik ciśnienia; wytrzymałość na ściskanie");
        if(exportIntegral){
            fileHeader.append("; wartość całki");
        }
        if(exportRawData){
            fileHeader.append("; P(t) (interwał ");
            fileHeader.append(std::to_string((float)interval/60).c_str());
            fileHeader.append(" minut)");
        }
        if(exportDerivative){
            fileHeader.append("; dP/dt (interwał ");
            fileHeader.append(std::to_string((float)interval/60).c_str());
            fileHeader.append(" minut)");
        }
        csv_out << fileHeader << endl;
        QByteArray qBA_pressHist;
        QByteArray qBA_derivative;
        std::vector<double> pressure_history;
        std::vector<double> pressure_derivative;
        while(query.next()){
            average_pressure_index=0;
            average_progress = 0;
            compressive_strength = 0;
            shield = query.value(0).toInt();
            begin_time = query.value(1).toLongLong();
            end_time = query.value(2).toLongLong();
            pressure_integral = query.value(3).toFloat();
            stoppage_entries = query.value(4).toInt();

            coalLine = getCoalLineMeters(shield, begin_time);

            average_progress_query_body.clear();
            average_progress_query_body.append("SELECT * FROM average_progress WHERE begin_time <'");
            average_progress_query_body.append(QString::number(end_time)).append("' AND end_time > '");
            average_progress_query_body.append(QString::number(end_time)).append("';");
            std::cout << "Searching for average progress with query: " << average_progress_query_body.toLocal8Bit().data() << std::endl;
            average_progress_query.exec(average_progress_query_body);

            average_pressure_index_query_body.clear();
            average_pressure_index_query_body.append("SELECT * FROM average_pressure_index WHERE begin_time < '");
            average_pressure_index_query_body.append(QString::number(end_time)).append("' AND end_time > '");
            average_pressure_index_query_body.append(QString::number(end_time)).append("';");
            std::cout << "Searching for average pressure with query: " << average_pressure_index_query_body.toLocal8Bit().data() << std::endl;
            average_pressure_index_query.exec(average_pressure_index_query_body);

            compressive_strength_query_body.clear();
            compressive_strength_query_body.append("SELECT * FROM compressive_strength WHERE coal_line_begin < '");
            compressive_strength_query_body.append(QString::number(coalLine)).append("' AND coal_line_end > '");
            compressive_strength_query_body.append(QString::number(coalLine)).append("';");
            std::cout << "Searching for compressive strength with query: " << compressive_strength_query_body.toLocal8Bit().data() << std::endl;
            compressive_strength_query.exec(compressive_strength_query_body);

            while (average_progress_query.next()) {
                average_progress = average_progress_query.value(2).toDouble();
            }

            while(average_pressure_index_query.next()){
                average_pressure_index = average_pressure_index_query.value(2).toDouble();
            }

            while(compressive_strength_query.next()){
                compressive_strength = compressive_strength_query.value(2).toLongLong();
            }


            QDateTime dateTime;
            QString line;
            line.append(std::to_string(shield).c_str());
            line.append("; ");
            dateTime.setMSecsSinceEpoch(begin_time);
            line.append(dateTime.toString("dd.MM.yyyy hh:mm:ss").toLocal8Bit());
            line.append("; ");
            dateTime.setMSecsSinceEpoch(end_time);
            line.append(dateTime.toString("dd.MM.yyyy hh:mm:ss").toLocal8Bit());
            line.append("; ");
            stoppage_duration = (long long((long long)(end_time - begin_time))/((long long)60000));
            line.append(std::to_string(stoppage_duration).c_str());
            line.append("; ");
            line.append(std::to_string(coalLine).c_str());

            line.append("; ").append(QString::number(average_progress));
            line.append("; ").append(QString::number(average_pressure_index));
            line.append("; ").append(QString::number(compressive_strength));

            if(exportIntegral){
                line.append("; ");
                std::string integral_string = std::to_string(pressure_integral);
                std::replace( integral_string.begin(), integral_string.end(), '.', ',');
                line.append(integral_string.c_str());
            }
            int byte_array_size_press;
            double derivative_subsum=0;
            int byte_array_size_deriv;
            double tmp;
            //std::cout << "Entries = " << stoppage_entries << ", entry size = " << sizeof(tmp) << std::endl;
            if(exportRawData || exportDerivative){
                qBA_pressHist = query.value(5).toByteArray();
                qBA_derivative = query.value(6).toByteArray();
                byte_array_size_press = qBA_pressHist.size();
                byte_array_size_deriv = qBA_derivative.size();
                //std::cout << "pressure BLOB size: " << byte_array_size_press << " derivative BLOB size: " << byte_array_size_deriv << std::endl;
                if (byte_array_size_press/sizeof(tmp) == stoppage_entries && byte_array_size_deriv/sizeof(tmp) == stoppage_entries-1)
                {
                    //std::cout << "Read BLOB" <<std::endl;
                    for (int i=0; i< stoppage_entries; i++){
                        tmp = *reinterpret_cast<const double*>(qBA_pressHist.data());
                        //std::cout << "Extracted data: " << tmp << std::endl;
                        qBA_pressHist.remove(0,8);
                        pressure_history.push_back(tmp);
                    }
                    for (int i=0; i< stoppage_entries-1; i++){
                        tmp = *reinterpret_cast<const double*>(qBA_derivative.data());
                        //std::cout << "Extracted data: " << tmp << std::endl;
                        qBA_derivative.remove(0,8);
                        pressure_derivative.push_back(tmp);
                    }
                    if(exportRawData){
                        //std::cout << "   export Raw Data" <<std::endl;
                        line.append("; P(t) ");
                        for (unsigned int i =0; i< pressure_history.size();i++){
                            if(i%intervalCounter==0){
                                line.append("; ");
                                std::string pressure_string = std::to_string(pressure_history[i]);
                                std::replace( pressure_string.begin(), pressure_string.end(), '.', ',');
                                line.append(pressure_string.c_str());
                            }
                        }
                    }
                    if(exportDerivative){
                        //std::cout << "   export Derivative" <<std::endl;
                        line.append("; dP/dt");
                        for (unsigned int i = 0; i< pressure_derivative.size();i++){
                            derivative_subsum+=pressure_derivative[i];
                            if(i%intervalCounter==0 ){
                                line.append("; ");
                                std::string deriv_string = std::to_string(derivative_subsum/(double)intervalCounter);
                                std::replace( deriv_string.begin(), deriv_string.end(), '.', ',');
                                line.append(deriv_string.c_str());
                                derivative_subsum=0;
                            }
                        }
                    }
                } else
                {
                    std::cout << "Wrong BLOB size." <<std::endl;
                    csv_out << "Wrong BLOB size." << endl;
                }
            }
            csv_out << line << endl;
            line.clear();
            pressure_history.clear();
            pressure_derivative.clear();
        }
    }

}

long MainWindow::getCoalLine(int shieldNo, long long time){
    coal_line_query = QSqlQuery(db);
    coal_line_query_text.clear();
    coal_line_query_text.append("SELECT * FROM states WHERE time = '");
    coal_line_query_text.append(std::to_string(time).c_str());
    coal_line_query_text.append("' AND shield = '");
    coal_line_query_text.append(std::to_string(shieldNo).c_str());
    coal_line_query_text.append("';");
    if(coal_line_query.exec(coal_line_query_text)){
        if(coal_line_query.isSelect()){
            if(coal_line_query.first()){
                return coal_line_query.value(5).toLongLong();
            }
        }
    }
    return 0;
}

long MainWindow::getCoalLineRaw(int shieldNo, long long time){
    return getCoalLine(shieldNo, time);
}

double MainWindow::getCoalLineMeters(int shieldNo, long long time){
    return ((double)getCoalLineRaw(shieldNo, time))/1000;
}


void MainWindow::clear_index_table(){
    QString clear_query;
    clear_query.append("DELETE FROM pressure_index;");
    //std::cout << "Executing query" << std::endl;
    if(query.exec(clear_query)){
        QString statuBarMessage("Tabela wskaźników ciśnienia została wyczyszczona");
        statusBar()->showMessage(statuBarMessage,0);
    }
    else {
        std::cout << query.lastError().text().toStdString() << std::endl;
        QString statuBarMessage("Wystąpił błąd podczas czyszcenia tabeli wskaźników ciśnienia");
        statusBar()->showMessage(statuBarMessage,0);
    }
}

bool MainWindow::check_shields_table(std::vector <Shield> current_state, bool init_table){
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
                if(query.exec(queryInsert)){
                    std::cout << "Inserted new shield: " << id << " with query: " << queryInsert.toLocal8Bit().data() << std::endl;
                    shield_no++;
                }
            }
            number_of_shields = shield_no;
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

/** Slot for UI button action. Update plots accordingly to selected shields.
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

    QVector<double> time, press2, press3, avg_press, ramstroke, coalLine, suppPos, convPos;
    double max_val=0, tmp_double;
    int occ=0;
    while(query.next()){
        //std::cout << "Shield time: " << query.value(0).toString().toLocal8Bit().data() << ", int: " << query.value(0).toInt() << ", long: " << query.value(0).toLongLong() << std::endl;
        time.push_back(query.value(0).toLongLong()/1000);
        // division to change integer values to MPa
        tmp_double = query.value(2).toDouble()/10;
        if(tmp_double>max_val){
            max_val=tmp_double;
        }
        press2.push_back(tmp_double);
        tmp_double = query.value(3).toDouble()/10;
        if(tmp_double>max_val){
            max_val=tmp_double;
        }
        press3.push_back(tmp_double);
        avg_press.push_back(0.5 * press2.last() + 0.5 * press3.last());
        ramstroke.push_back(query.value(4).toDouble());
        // division to change integer value to meters
        coalLine.push_back(query.value(5).toDouble()/1000);
        suppPos.push_back(query.value(6).toDouble()/1000);
        convPos.push_back(query.value(7).toDouble()/1000);
        occ++;
    }
    std::cout << "Occurences: " << occ << std::endl;
    std::cout << "Time length: " << time.size() << std::endl;
    // zoom and interaction disabled, adjust plot axis at every data reload
    /*
     if(ui->customPlot->graphCount()==0){
        std::cout << "Begin time: " << time[0] << ", end time: " << time[time.size()-1] <<  std::endl;
        ui->customPlot->xAxis->setRange(time[0]-3600, time[time.size()-1]+3600);
        ui->customPlot->yAxis->setRange(0, 450);
    }
    */

    ui->customPlot->yAxis->setRange(-0.1*max_val, max_val+0.1*max_val);
    ui->customPlot->xAxis->setRange(time[0]-3600, time[time.size()-1]+3600);
    ui->customPlot->clearGraphs();

    QPen pen;
    int i;

    /*ui->customPlot->addGraph();
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
    */

    ui->customPlot->addGraph();
    i = ui->customPlot->graphCount()-1;
    ui->customPlot->graph(i)->setData(time, avg_press);
    ui->customPlot->graph(i)->setProperty("ID",QVariant(shield_id));
    ui->customPlot->graph(i)->setName("Ciśnienie średnie");
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
    ui->customPlot->graph(i)->setData(time, ramstroke);
    ui->customPlot->graph(i)->setProperty("ID",QVariant(shield_id));
    ui->customPlot->graph(i)->setName("Wysuw siłownika");
    pen.setColor(QColor(0, 0, 255));
    ui->customPlot->graph()->setPen(pen);

    ui->customPlot->yAxis2->setRange(0, 300); //ramstroke[0]-10, ramstroke[ramstroke.size()-1]+10);

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
    QStringList filenames = QFileDialog::getOpenFileNames(this, tr("Otwórz plik VShield"),tr(""),tr("Pliki VShield (*.LogVShield10)"));
    if(filenames.isEmpty()){
        QString statuBarMessage("No file selected");
        statusBar()->showMessage(statuBarMessage,0);
        std::cout << "No file selected" << std::endl;
        vshield_selected = false;
    } else {
        process_VShieldFiles(filenames, query);
    }

    /*if(filename.isNull()){
        QString statuBarMessage("D1: No file selected");
        statusBar()->showMessage(statuBarMessage,0);
        std::cout << "No file selected" << std::endl;
        vshield_selected = false;
    } else {
        QString statuBarMessage("D2: Wybrany plik: ");
        statuBarMessage.append(filename);
        statusBar()->showMessage(statuBarMessage,0);
        vShieldReader = new VShieldReader(filename.toStdString());
        QString statuBarMessage2("D3: VShieldReader created ");
        statusBar()->showMessage(statuBarMessage2,0);
        if(vShieldReader->get_vshield_file_state()){
            QString statuBarMessage("Wybrany plik: ");
            statuBarMessage.append(vShieldReader->get_vshield_file_name().c_str());
            statusBar()->showMessage(statuBarMessage,0);
            vshield_selected = true;
        } else {
            vshield_selected = false;
        }
    }*/
}

/** Slot for UI button action. Select folder and check files inside if they are compatible with VShield standard.
 * @brief MainWindow::openVShieldFolder
 */
void MainWindow::openVShieldFolder(){
    const QString homeFolder = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
    QString foldername = QFileDialog::getExistingDirectory(this, tr("Wybierz folder"), homeFolder,QFileDialog::ShowDirsOnly
                                                           | QFileDialog::DontResolveSymlinks);
    if(!foldername.isNull()){
        QDir vshDir(foldername);
        QStringList filters;
        filters << "*.LogVShield10";
        vshDir.setNameFilters(filters);
        QStringList files = vshDir.entryList();
        process_VShieldFiles(foldername, files, query);
    } else {
        QString statuBarMessage("No directory selected");
        statusBar()->showMessage(statuBarMessage,0);
        std::cout << "No directory selected" << std::endl;
    }
    vshield_selected = false;

}

/**
 * @brief MainWindow::process_VShieldFile Function to process VShield files
 * @param files List of files with full path
 * @param query Query to be used for inserting data to databse
 */
void MainWindow::process_VShieldFiles(QStringList files, QSqlQuery sqlQuery){
    if(!files.isEmpty()){
        QStringList filenamesList;
        QString filename = files.at(0);
        QFileInfo fi(filename);
        //QDir d = fi.absoluteDir();
        QString directory = fi.absolutePath();
        std::cout << "Chosen directory: " << directory.toLocal8Bit().data() << std::endl;
        for (int i=0; i<files.size(); i++){
            filename = files.at(i);
            fi = QFileInfo(filename);
            filenamesList.append(fi.fileName());
            std::cout << "Chosen file: " << filenamesList.at(i).toLocal8Bit().data()<< std::endl;
        }
        process_VShieldFiles(directory, filenamesList, sqlQuery);
    }
}

/**
 * @brief MainWindow::process_VShieldFile Function to process VShield files
 * @param directory Directory containing files
 * @param files List of file names
 * @param query Query to be used for inserting data to databse
 */
void MainWindow::process_VShieldFiles(QString directory, QStringList files, QSqlQuery sqlQuery){
    if(!files.isEmpty()){
        VShieldProgressDialog progressDialog(directory, files, sqlQuery);
        if(progressDialog.exec()){
            progressDialog.getNumberOfShields(&number_of_shields);
            //stayTimeDlg.getStayTime(&min_stay_time, &max_stay_time);
            /*QString statusBarMessage("Minimalny czas postoju: ");
            statusBarMessage.append(QString::number(min_stay_time));
            statusBarMessage.append(", maksymalny czas postoju: ");
            statusBarMessage.append(QString::number(max_stay_time));
            statusBar()->showMessage(statusBarMessage,0);*/
        }
    } else {
        QString statuBarMessage("No VShield files in selected directory");
        statusBar()->showMessage(statuBarMessage,0);
        std::cout << "No VShield files in selected directory" << std::endl;
    }
}

/** Slot for UI button action. Determine limiting pressures for searching integral.
 * @brief MainWindow::dialogGetPressureLimits
 */
void MainWindow::dialogGetPressureLimits(){
    PressureLimitDialog pld(min_pressure, max_pressure);
    if(pld.exec()){
        pld.getPressureLimts(&min_pressure, &max_pressure);

        QString statusBarMessage("Ciśnienie minimalne: ");
        statusBarMessage.append(QString::number(min_pressure));
        statusBarMessage.append(", ciśnienie maksymalne: ");
        statusBarMessage.append(QString::number(max_pressure));
        statusBar()->showMessage(statusBarMessage,0);
    }
}

/** Slot for UI button action. Determine time limits for searching integral.
 * @brief MainWindow::dialogGetStayTime
 */
void MainWindow::dialogGetStayTime(){
    StayTimeDialog stayTimeDlg(min_stay_time, max_stay_time);
    if(stayTimeDlg.exec()){
        stayTimeDlg.getStayTime(&min_stay_time, &max_stay_time);
        QString statusBarMessage("Minimalny czas postoju: ");
        statusBarMessage.append(QString::number(min_stay_time));
        statusBarMessage.append(", maksymalny czas postoju: ");
        statusBarMessage.append(QString::number(max_stay_time));
        statusBar()->showMessage(statusBarMessage,0);
    }
}

/**
 * @brief MainWindow::dialogGetCompressiveStrengths
 */
void MainWindow::dialogGetCompressiveStrengths(){

    /*    SQLite query for selecting shield
       select_query.append("SELECT * FROM states WHERE shield=");
       select_query.append(QString::number(shield_id));
       select_query.append(";");
       std::cout << "Executing query" << std::endl;
       query.exec(select_query);
       std::cout << "Processing query" << std::endl;
       while(query.next()){
           if(new_stay){
               new_stay = false;
               stay_begin_timestamp = query.value(0).toDouble()/(60*1000);
               raw_begin_timestamp = query.value(0).toLongLong();
               ramstroke_begin = query.value(4).toLongLong();
               ramstroke_current = query.value(4).toLongLong();
               support_begin_pos = (query.value(6).toDouble()/1000);
               support_end_pos = (query.value(6).toDouble()/1000);
               time_diff = 0;
               */
    QString select_query = "SELECT * FROM compressive_strength;";
    CompressiveStrengthDialog *compressStrDlg;

    std::cout << "Select query: " << select_query.toLocal8Bit().data() << std::endl;
    query.exec(select_query);
    single_entry tmp_entry;
    std::vector<single_entry> current_data;
    while(query.next()){
        std::cout << "add entry to initial data" << std::endl;
        tmp_entry.begin = query.value(0).toInt();
        tmp_entry.end = query.value(1).toInt();
        tmp_entry.strength = query.value(2).toInt();
        current_data.push_back(tmp_entry);
    }
    compressStrDlg = new CompressiveStrengthDialog(current_data);

    if(compressStrDlg->exec()){
        // clear current values from table
        QString clear_query = "DELETE FROM compressive_strength;";
        query.exec(clear_query);

        // insert new values into table
        std::vector<single_entry> strengths = compressStrDlg->getCompressiveStrengths();
        QString insert_query;
        for(int i=0; i<strengths.size(); i++){
            insert_query.clear();
            insert_query.append("INSERT INTO compressive_strength VALUES (");
            insert_query.append(QString::number(strengths[i].begin)).append(", ");
            insert_query.append(QString::number(strengths[i].end)).append(", ");
            insert_query.append(QString::number(strengths[i].strength)).append(");");
            std::cout << "query to insert: " << insert_query.toLocal8Bit().data() << std::endl;
            if(query.exec(insert_query)){
                std::cout << "Data inserted succesfully" << std::endl;
            } else {
                std::cout << "Error while insterting data" << std::endl;
            }
        }
    }
}

/** Slot for UI button action. Calculate pressure integral for shields selected in the dialog.
 * @brief MainWindow::determinePressureIndex
 */
void MainWindow::determinePressureIndex(){
    PressureGainIndexDialog gainDialog(number_of_shields);
    if(gainDialog.exec()){
        std::cout << "Shields were selected ;)" << std::endl;
        std::vector<int> selected_shields = gainDialog.getChosenShields();
        for (std::vector <int>::iterator cur_st = selected_shields.begin(); cur_st !=selected_shields.end(); cur_st++){
            int id = *cur_st;
            std::cout << "selected shield: " << id << std::endl;
            int found_integrals = calculate_pressure_integral(id);
            QString statusBarMessage("Obliczono ");
            statusBarMessage.append(QString::number(found_integrals));
            statusBarMessage.append(" wskaźników ciśnienia dla obudowy ");
            statusBarMessage.append(QString::number(id));
            statusBar()->showMessage(statusBarMessage,0);
        }
    }

}

/** Calculate pressure integrals for selected shield.
 * @brief MainWindow::calculate_pressure_integral
 * @param shield_id Id of shield.
 * @return Number of integrals found
 */
int  MainWindow::calculate_pressure_integral(int shield_id){
    // local variables
    double stay_begin_timestamp; //minutes
    double stay_end_timestamp; //minutes
    double time_diff; // minutes
    double support_begin_pos; // minutes
    double support_end_pos; // minutes
    double ramstroke_begin; // milimeters (PL: wysuw słownika)
    double ramstroke_current; // milimeters (PL: wysuw słownika)
    bool new_stay = true;
    long long raw_begin_timestamp;
    long long raw_end_timestamp;
    std::vector<double> avg_pressure; // MPa
    std::vector<double> time; //minutes
    QString select_query;
    double integral; // MPa
    std::vector<double> derivative; //derivative of pressure function

    std::vector<int> pi_shield;
    std::vector<long long> pi_begin;
    std::vector<long long> pi_end;
    std::vector<double> pi_integral;
    std::vector<std::vector<double>> pi_pressure_history;
    std::vector<std::vector<double>> pi_pressure_derivative;


    // Function begining
    //std::cout << "Calculating gain index" << std::endl;
    // SQLite query for selecting shield
    select_query.append("SELECT * FROM states WHERE shield=");
    select_query.append(QString::number(shield_id));
    select_query.append(";");
    std::cout << "Executing query" << std::endl;
    query.exec(select_query);
    std::cout << "Processing query" << std::endl;
    while(query.next()){
        if(new_stay){
            new_stay = false;
            stay_begin_timestamp = query.value(0).toDouble()/(60*1000);
            raw_begin_timestamp = query.value(0).toLongLong();
            ramstroke_begin = query.value(4).toLongLong();
            ramstroke_current = query.value(4).toLongLong();
            support_begin_pos = (query.value(6).toDouble()/1000);
            support_end_pos = (query.value(6).toDouble()/1000);
            time_diff = 0;
            std::cout << "timestamp " << raw_begin_timestamp << ", ramstroke begin: " << ramstroke_begin << ", suppotr pos: " << support_begin_pos << std::endl;
        } else {
            if(support_end_pos==support_begin_pos && ramstroke_begin>=(ramstroke_current-100))
            {
                avg_pressure.push_back((query.value(2).toDouble()+query.value(3).toDouble())/20);
                time.push_back(time_diff);
                if(support_begin_pos != (query.value(6).toDouble()/1000)){

                    if(time_diff>max_stay_time){
                        std::cout << "Postój od " << stay_begin_timestamp << " do " << stay_end_timestamp << std::endl;
                        std::cout << "Stay time " << time_diff << std::endl;
                        if(avg_pressure.at(0) > min_pressure && avg_pressure.at(0) < max_pressure){
                            std::cout << "Calculate integral... ";
                            integral = 0;
                            for(int i = 1; i< avg_pressure.size(); i++){
                                //std::cout << "Time diff: " << time[i]-time[i-1] << std::endl;
                                integral = integral + 0.5*(avg_pressure[i]+avg_pressure[i-1])*(60*(time[i]-time[i-1]));
                                derivative.push_back((avg_pressure[i]-avg_pressure[i-1])/(60*(time[i]-time[i-1])));
                            }
                            std::cout << "   " << integral << std::endl;

                            pi_shield.push_back(shield_id);
                            pi_begin.push_back(raw_begin_timestamp);
                            pi_end.push_back(raw_end_timestamp);
                            pi_integral.push_back(integral);
                            pi_pressure_history.push_back(avg_pressure);
                            pi_pressure_derivative.push_back(derivative);
                        }
                    }
                }
                support_end_pos = (query.value(6).toDouble()/1000);
                ramstroke_current = query.value(4).toLongLong();
                stay_end_timestamp = query.value(0).toDouble()/(60*1000);
                raw_end_timestamp = query.value(0).toLongLong();
                time_diff = (stay_end_timestamp - stay_begin_timestamp);
                std::cout << "timestamp " << raw_end_timestamp << ", ramstroke current: " << ramstroke_current << ", support end pos: " << support_end_pos << std::endl;

            } else {
                //std::cout << "Support pos change" << support_end_pos <<  std::endl;
                new_stay = true;
                avg_pressure.clear();
                time.clear();
                derivative.clear();
            }

        }
        //std::cout << "Shield number " << query.value(0).toInt() << std::endl;
    }
    query.finish();
    for (int i = 0; i<pi_shield.size(); i++){
        insertPressureIntegral(pi_shield.at(i), pi_begin.at(i), pi_end.at(i), pi_integral.at(i), pi_pressure_history.at(i), pi_pressure_derivative.at(i));
    }
    return (int)pi_shield.size();
}

int MainWindow::insertPressureIntegral(int shield, long long begin_time, long long end_time, double integral, std::vector<double> pressure_history, std::vector<double> derivative){
    QString queryInsertShield("INSERT INTO pressure_index VALUES (");
    queryInsertShield.append(std::to_string(shield).c_str());
    queryInsertShield.append(", ");
    queryInsertShield.append(std::to_string(begin_time).c_str());
    queryInsertShield.append(", ");
    queryInsertShield.append(std::to_string(end_time).c_str());
    queryInsertShield.append(", ");
    std::string integral_string = std::to_string(integral);
    std::replace( integral_string.begin(), integral_string.end(), ',', '.'); // replace all 'x' to 'y'
    queryInsertShield.append(integral_string.c_str());
    queryInsertShield.append(", ");
    queryInsertShield.append(":stoppage_duration");
    queryInsertShield.append(", ");
    queryInsertShield.append(":press_hist");
    queryInsertShield.append(", ");
    // TODO check if database is suited to this values????
    queryInsertShield.append(":press_deriv");
    queryInsertShield.append(");");

    QByteArray inByteArray;
    uint64_t stoppage_entries = pressure_history.size();
    int entry_size = sizeof(pressure_history[0]);
    for (uint64_t i=0; i<stoppage_entries; i++){
        inByteArray.append(reinterpret_cast<const char*>(&pressure_history[i]), entry_size);
    }

    QByteArray derivByteArray;
    uint64_t deriv_entries = derivative.size();
    int deriv_entry_size = sizeof(derivative[0]);
    for (uint64_t i=0; i<deriv_entries; i++){
        derivByteArray.append(reinterpret_cast<const char*>(&derivative[i]), deriv_entry_size);
    }

    //std::cout << "Query: " << queryInsertShield.toStdString().c_str() << std::endl;
    if(db.isOpen()){
        pressure_index_query = QSqlQuery(db);
        pressure_index_query.prepare(queryInsertShield);
        pressure_index_query.bindValue( ":stoppage_duration", (unsigned long long)stoppage_entries);
        pressure_index_query.bindValue( ":press_hist", inByteArray);
        pressure_index_query.bindValue( ":press_deriv", derivByteArray);
        std::cout << "Query to execute: " << queryInsertShield.toLocal8Bit().data() << std::endl;
        //std::cout << "pressure_history[0] = " << pressure_history[0] << std::endl;

        //if(pressure_index_query.exec(queryInsertShield)){
        if(pressure_index_query.exec()){
            std::cout << "Succesfully inserted pressure integral" << std::endl;
            std::cout << pressure_index_query.executedQuery().toLocal8Bit().data() << std::endl;
            return 0;
        } else {
            std::cout << "Database error" << std::endl;
            QSqlError err = pressure_index_query.lastError();
            std::cout << "Error message: " <<err.text().toStdString().c_str() << std::endl;
            return -1;
        }} else {
        std::cout << "Database NOT OPENED" << std::endl;
        return -1;
    }
    //return 0;
}


/** Slot for UI button action. Extract data from VShield file and save it to database.
 * @brief MainWindow::vShieldAnalyze
 */
/*
void MainWindow::vShieldAnalyze(){
    std::vector <FaceState> facestates;
    facestates = vShieldReader->extract_data();
    if(facestates.size()==0){
        QString qStr("Błąd pliku VShield");
        statusBar()->showMessage(qStr,0);
    } else{
        std::cout << "Entries found: " << facestates.size() << std::endl;
        int insert_counter = 0;
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
            query.exec(queryCheckTS);
            if(query.next()){
                std::cout << "Timestamp found in database, skip updating" << std::endl;
            } else {
                if(insert_begin == 0){
                    query.exec("BEGIN");
                }
                //std::cout << "Inserting face state " << insert_counter << " of " << facestates.size() << " with timestamp " << timestamp_epoch << std::endl;
                insert_counter++;
                QString queryInsertTS( "INSERT INTO timestamps VALUES (" );
                queryInsertTS.append(std::to_string(timestamp_epoch).c_str());
                queryInsertTS.append( ");" );
                if(query.exec(queryInsertTS)){

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
                    insert_begin++;
                    if(insert_begin == 20){
                        query.exec("COMMIT");
                        insert_begin = 0;
                    }
                } else {
                    std::cout << "Unable to add new timestamp int table" << std::endl;
                }
            }
        }
        if(insert_begin!=0){
            query.exec("COMMIT");
        }
        std::cout << "DONE ;)" << std::endl;
        std::cout << "Insterted all facestates" << std::endl;
    }
    vshield_selected = false;
}
*/

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
        if(query.exec("CREATE TABLE pressure_index(shield INT, begin_time INT, end_time INT, integral REAL, stoppage_duration INT, press_hist BLOB, press_deriv BLOB, FOREIGN KEY(begin_time) REFERENCES timestamps(timestamp), FOREIGN KEY(end_time) REFERENCES timestamps(timestamp), FOREIGN KEY(shield) REFERENCES shields(shield_number));")){
            std::cout << "Table pressure_index was created" << std::endl;
        } else {
            std::cout << "Error 4" << std::endl;
        }
        if(query.exec("CREATE TABLE compressive_strength(coal_line_begin INTEGER PRIMARY KEY, coal_line_end INTEGER, strength INTEGER);")){
            std::cout << "Table compressive_strength was created" << std::endl;
        } else {
            std::cout << "Error 5" << std::endl;
        }
        if(query.exec("CREATE TABLE app_config(param TEXT, value TEXT);")){
            std::cout << "Table app_config was created" << std::endl;
        } else {
            std::cout << "Error 6" << std::endl;
        }
        if(query.exec("INSERT INTO app_config VALUES('DAY_BEGIN_TIME', 6);")){
            std::cout << "Table app_config was created" << std::endl;
        } else {
            std::cout << "Error 6.1" << std::endl;
        }
        if(query.exec("CREATE TABLE average_progress(begin_time INTEGER PRIMARY KEY, end_time INTEGER, progress REAL);")){
            std::cout << "Table average_progress was created" << std::endl;
        } else {
            std::cout << "Error 7" << std::endl;
        }
        if(query.exec("CREATE TABLE average_pressure_index(begin_time INTEGER PRIMARY KEY, end_time INTEGER, pressure_index REAL);")){
            std::cout << "Table average_pressure_index was created" << std::endl;
        } else {
            std::cout << "Error 8" << std::endl;
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
                std::vector <Shield> current_state;
                check_shields_table(current_state, false);
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
        qAction_openVShieldFolder->setEnabled(true);
    } else {
        qAction_openVShield->setEnabled(false);
        qAction_openVShieldFolder->setEnabled(false);
    }
    if(db_selected && vshield_selected){
        qAction_analyze_vshield->setEnabled(true);
    } else {
        qAction_analyze_vshield->setEnabled(false);
    }
}

/*enum MainWindow::shield_params{
    press_1 = 1,
    press_2 = 2,
    ramstrk = 3,
    coalLn = 4,
    supp_Pos = 5,
    conv_Pos = 6
};*/

MainWindow::~MainWindow()
{
    if(db.isOpen()){
        db.close();
    }
    delete ui;
}
