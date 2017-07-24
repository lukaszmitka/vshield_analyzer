#ifndef VSHIELDANALYZERWORKER_H
#define VSHIELDANALYZERWORKER_H

#include <QObject>
#include <QThread>
#include <QDir>
#include <QtSql/QtSql>
#include <iostream>
#include "vshieldreader.h"

class VShieldAnalyzerWorker : public QObject
{
    Q_OBJECT
public:
    //explicit VShieldAnalyzerWorker(QObject *parent = nullptr);
    VShieldAnalyzerWorker(QSqlQuery query, QString directory, QStringList files);
signals:
    void finished();
    void setShieldNo(int shield_no);
    void setValue(int value);
    void error(QString err);
public slots:
        void process();
        void fileProgress(double progress);
private:
        QStringList filesToProcess;
        QString filesDirectory;
        QString file;
        QSqlQuery insertQuery;
        VShieldReader* vShieldReader;
        bool check_shields_table(std::vector <Shield> current_state, bool init_table);
        unsigned int num_of_files_to_process;
        unsigned int current_file_id;
        int major_progress;
        int minor_progress;
        int number_of_shields = 0;
};

#endif // VSHIELDANALYZERWORKER_H
