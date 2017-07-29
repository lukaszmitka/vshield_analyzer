#include "vshieldprogressdialog.h"

VShieldProgressDialog::VShieldProgressDialog(QString directory, QStringList files, QSqlQuery query){

    insertQuery = query;
    label_number_of_files = new QLabel(tr("Ilość plików do analizy: "));
    numberOfFIlesSpinBox = new QSpinBox();
    numberOfFIlesSpinBox->setValue(files.size());
    numberOfFIlesSpinBox->setDisabled(true);

    progressBar = new QProgressBar;
    progressBar->setRange(0,100);

    //cancelButton = new QPushButton(tr("Anuluj"));
    //connect(cancelButton, SIGNAL(clicked()), this, SLOT(close()));
    okButton = new QPushButton(tr("OK"));
    okButton->setDisabled(true);
    connect(okButton, SIGNAL(clicked()), this, SLOT(accept()));

    numberOfFilesLayout = new QHBoxLayout;
    numberOfFilesLayout->addWidget(label_number_of_files);
    numberOfFilesLayout->addWidget(numberOfFIlesSpinBox);

    buttons_layout = new QHBoxLayout;
    buttons_layout->addWidget(okButton);
    //buttons_layout->addSpacing(20);
    //buttons_layout->addWidget(cancelButton);

    mainLayout = new QVBoxLayout;
    mainLayout->addLayout(numberOfFilesLayout);
    mainLayout->addSpacing(12);
    mainLayout->addWidget(progressBar);
    mainLayout->addSpacing(12);
    mainLayout->addLayout(buttons_layout);
    setLayout(mainLayout);

    QThread* thread = new QThread;
    worker = new VShieldAnalyzerWorker(insertQuery, directory, files);
    worker->moveToThread(thread);
    //connect(worker, SIGNAL(error(QString)), this, SLOT(errorString(QString)));

    connect(thread, SIGNAL(started()), worker, SLOT(process()));
    connect(worker, SIGNAL(setValue(int)), progressBar, SLOT(setValue(int)));

    // cleanup
    connect(worker, SIGNAL(finished()), thread, SLOT(quit()));
    connect(worker, SIGNAL(finished()), worker, SLOT(deleteLater()));
    connect(worker, SIGNAL(setShieldNo(int)), this, SLOT(filesProcessed(int)));

    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));

    thread->start();
}

void VShieldProgressDialog::filesProcessed(int shield_no){
    number_of_shields = shield_no;
    okButton->setDisabled(false);
}

void VShieldProgressDialog::getNumberOfShields(int *shield_no){
    *shield_no = number_of_shields;
}
