#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ui_mainwindow.h"
#include "vshieldreader.h"
#include "pressuregainindexdialog.h"
#include "pressurelimitdialog.h"
#include "staytimedialog.h"
#include "vshieldanalyzerworker.h"
#include "vshieldprogressdialog.h"
#include "compressivestrengthdialog.h"
#include <QFileDialog>
#include <QTextCodec>
#include <QString>
#include <QMessageBox>
#include <QtSql/QtSql>
#include <iostream>
#include <QAction>
#include <QWidgetAction>
#include <QWidget>
#include <QStandardPaths>
#include <algorithm>
#include <exportcsvdialog.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    std::vector<QListWidgetItem> checkboxShieldList;
    Ui::MainWindow *ui;
    bool open_database(QString database_name);
    bool open_database(QString database_name, bool init);
    bool check_db_integrity(QSqlDatabase database);
    bool create_new_database();
    bool check_shields_table(std::vector <Shield> current_state, bool init_table);
    void process_VShieldFiles(QString directory, QStringList files, QSqlQuery sqlQuery);
    void process_VShieldFiles(QStringList files, QSqlQuery sqlQuery);
    int calculate_pressure_integral(int shield_id);
    int insertPressureIntegral(int shield, long long begin_time, long long end_time, double integral, std::vector<double> pressure_history,  std::vector<double> derivative);
    long getCoalLine(int shieldNo, long long time);
    long getCoalLineRaw(int shieldNo, long long time);
    double getCoalLineMeters(int shieldNo, long long time);
    int get_dayBeginTime_From_DB();
    void update_dayBeginTimeInDB(int dayBeginTime);
    double get_shieldCoalLineProgress(int shieldID, long long beginTimestamp, long long endTimestamp);
    double get_averageCoalLineProgress(long long beginTimestamp, long long endTimestamp);
    double get_averagePressureIndex(long long beginTimestamp, long long endTimestamp);
    long long get_nextDay(long long timestamp, int dayBeginHour);
    void exportProcessedData(bool exportDerivative, bool exportRawData, bool exportIntegral, int interval);
    void exportRawColumns(int interval);
    void exportStackedColumns(int interval);

    //enum shield_params;
    enum shield_params{
        press_1 = 1,
        press_2 = 2,
        ramstrk = 3,
        coalLn = 4,
        supp_Pos = 5,
        conv_Pos = 6
    };

    bool populate_shield_list(QSqlDatabase database);
    int desired_min_press = 24; //MPa
    int desired_max_press = 29; //MPa
    double min_pressure = 24; //MPa
    double max_pressure = 29; //MPa
    int min_stay_time = 0; // minutes
    int max_stay_time = 250; // minutes
    int number_of_shields = 0;

    QMenu *fileMenu;
    QAction *qAction_close;
    QAction *qAction_select_existing_db;
    QAction *qAction_create_db;
    QAction *qAction_openVShield;
    QAction *qAction_openVShieldFolder;
    QAction *qAction_analyze_vshield;
    QAction *qAction_stay_time;
    QAction *qAction_pressure_limit;
    QAction *qAction_press_index;
    QAction *qAction_export;
    QAction *qAction_clear_pressure_table;
    QAction *qAction_get_compressive_strengths;
    QAction *qAction_set_day_shift_begin_time;
    QAction *qAction_calculate_wall_progress;
    QAction *qAction_calculate_avg_press_index;
    QString db_file;
    QSqlDatabase db;
    QSqlQuery query;
    QSqlQuery coal_line_query;
    QSqlQuery pressure_index_query;
    QSqlQuery average_pressure_index_query;
    QSqlQuery average_progress_query;
    QSqlQuery compressive_strength_query;
    QString coal_line_query_text;
    const static int DATABASE_APP_ID = 700612651;
    VShieldReader *vShieldReader;
    bool db_selected = false;
    bool vshield_selected = false;
private slots:
    void dialogGetPressureLimits();
    void dialogGetStayTime();
    void dialogGetCompressiveStrengths();
    void determinePressureIndex();
    //void vShieldAnalyze();
    void openVShieldFile();
    void openVShieldFolder();
    void openDatabase();
    void newDatabase();
    void setActiveActions();
    void shieldClicked(QListWidgetItem* item);
    void export_to_csv();
    void clear_index_table();
    void dialogSetDayShifBeginTime();
    void calculateWallProgress();
    void calculateAvgPressIndex();
};

#endif // MAINWINDOW_H
