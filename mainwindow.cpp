#include "mainwindow.h"

QMenu *fileMenu;
QAction *qAction_close;
//QAction *qAction_one;
QAction *qAction_select_existing_db;
QAction *qAction_create_db;

QAction *qAction_openVShield;
QAction *qAction_analyze_vshield;

QString db_file;
QSqlDatabase db;
QSqlQuery query;

const static int DATABASE_APP_ID = 700612650;

VShieldReader *vShieldReader;

bool db_selected = false;
bool vshield_selected = false;

MainWindow::MainWindow(QWidget *parent):QMainWindow(parent),ui(new Ui::MainWindow)
{
    //QTextCodec::setCodecForTr (QTextCodec::codecForName ("Windows-1250"));
    QTextCodec::setCodecForLocale(QTextCodec::codecForName ("Windows-1250"));
    ui->setupUi(this);
    setWindowTitle(tr("VShield analyzer"));
    setMinimumSize(300, 300);
    resize(400, 250);
    //showFullScreen();

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

    QString qStr("tooltip");
    statusBar()->showMessage(qStr,0);

    vShieldReader = new VShieldReader();

    //QCoreApplication.addLibraryPath(".");
}

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

void MainWindow::nowy_plik()
{
    QString qStr("New File");
    statusBar()->showMessage(qStr,0);
    return;
}

void MainWindow::vShieldAnalyze(){
    std::vector <FaceState> facestates;
    facestates = vShieldReader->extract_data();
    std::cout << "Entries found: " << facestates.size() << std::endl;

    for (std::vector<FaceState>::iterator it = facestates.begin() ; it != facestates.end(); ++it){
        std::cout << (*it).get_status() << std::endl;
    }

    /*if(vShieldReader->extract_data()==0){

    } else {
        QString qStr("Błąd pliku VShield");
        statusBar()->showMessage(qStr,0);
    }*/
    vshield_selected = false;
}

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

void MainWindow::openDatabase(){
    QString database_fileName = QFileDialog::getOpenFileName(this, tr("Otwórz bazę danych"),tr(""),tr("Baza danych SQlite (*.db3)"));
    if(database_fileName.isEmpty()){
        std::cout << "Brak wybranego pliku" << std::endl;
        statusBar()->showMessage("Nie wybrano żadnego pliku",0);
        db_selected = false;
        return;
    }else {
        QFile *database_file = new QFile(database_fileName);
        if(database_file->exists()){
            if(open_database(database_fileName)){
                std::cout << "Otwarto bazę danych" << std::endl;
                statusBar()->showMessage("Otwarto bazę danych",0);
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
            // TODO create new database
            open_database(database_fileName, true);
        }
    }
}


bool MainWindow::open_database(QString database_name){
    return open_database(database_name, false);
}
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
    }
    if(check_db_integrity(db)){
        db_selected = true;
        return true;
    } else {
        db_selected = false;
        return false;
    }
}

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
        QString status = query.value(0).toString();
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
            QString status = query.value(0).toString();
            //std::cout << "Application_id: " << status.toLocal8Bit().constData() << std::endl;
            if(query.value(0).toInt()==DATABASE_APP_ID){
                return true;
            }
        }
    }
    return false;
}

MainWindow::~MainWindow()
{
    if(db.isOpen()){
        db.close();
    }
    delete ui;
}

/*if(query.exec("PRAGMA application_id = 12345;")){
   std::cout << "application_id set to 12345 "<< std::endl;
}*/
//db.exec("CREATE TABLE LOG( ID INT PRIMARY KEY NOT NULL, PRESSURE INT, COAL_LINE INT);");
