#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ui_mainwindow.h"
#include "vshieldreader.h"
#include <QFileDialog>
#include <QTextCodec>
#include <QString>
#include <QMessageBox>
#include <QtSql/QtSql>

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
    bool check_shields_table(std::vector <Shield> current_state);
    enum shield_params;
    bool populate_shield_list(QSqlDatabase database);
    QMenu *fileMenu;
    QAction *qAction_close;
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
private slots:
    void nowy_plik();
    void vShieldAnalyze();
    void openVShieldFile();
    void openDatabase();
    void newDatabase();
    void setActiveActions();
    void shieldClicked(QListWidgetItem* item);
};

#endif // MAINWINDOW_H
