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
    Ui::MainWindow *ui;
    bool open_database(QString database_name);
    bool open_database(QString database_name, bool init);
    bool check_db_integrity(QSqlDatabase database);
    bool create_new_database();
private slots:
    void nowy_plik();
    void vShieldAnalyze();
    void openVShieldFile();
    void openDatabase();
    void newDatabase();
    void setActiveActions();
};

#endif // MAINWINDOW_H
