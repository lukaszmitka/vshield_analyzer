#include "compressivestrengthdialog.h"

CompressiveStrengthDialog::CompressiveStrengthDialog()
{
    qTableWidget = new QTableWidget(this);
    qTableWidget->setRowCount(10);
    qTableWidget->setColumnCount(3);
    qTableHeader<<"Początek strefy [m]"<<"Koniec strefy [m]"<<"Odporność [???]";
    qTableWidget->setHorizontalHeaderLabels(qTableHeader);
    qTableWidget->setMinimumWidth(500);
    qTableWidget->setMinimumHeight(400);
    qTableWidget->setColumnWidth(0,150);
    qTableWidget->setColumnWidth(1,150);
    qTableWidget->setColumnWidth(2,150);
    initFooter();
    initExitFooter();
    mainLayout = new QVBoxLayout;
    mainLayout->addWidget(qTableWidget);
    mainLayout->addLayout(footer);
    mainLayout->addLayout(exitFooter);
    setLayout(mainLayout);

    // getting data
    //qTableWidget->takeItem(1,1)->data(0).toInt();
    // setting data
    //qTableWidget->setItem(0, 1, new QTableWidgetItem("Hello"));
}



std::vector<double> CompressiveStrengthDialog::getCompressiveStrengths(){
    std::vector<double> strengths;
    return strengths;
}

void CompressiveStrengthDialog::addNewEntry(){
    qTableWidget->setRowCount(qTableWidget->rowCount()+1);
    /*minSpinBoxes.push_back(new QSpinBox);
    minSpinBoxes.at(minSpinBoxes.size()-1)->setMinimumWidth(COAL_LINE_COLUMN_WIDTH);
    minSpinBoxes.at(minSpinBoxes.size()-1)->setMaximumWidth(COAL_LINE_COLUMN_WIDTH);
    maxSpinBoxes.at(maxSpinBoxes.size()-1)->setMinimumWidth(COAL_LINE_COLUMN_WIDTH);
    maxSpinBoxes.at(maxSpinBoxes.size()-1)->setMaximumWidth(COAL_LINE_COLUMN_WIDTH);
    compressionStrengths.push_back(new QSpinBox);
    compressionStrengths.at(compressionStrengths.size()-1)->setMinimumWidth(STRENGTH_COLUMN_WIDTH);
    compressionStrengths.at(compressionStrengths.size()-1)->setMaximumWidth(STRENGTH_COLUMN_WIDTH);
    strenghtTable->addWidget(minSpinBoxes.at(minSpinBoxes.size()-1),minSpinBoxes.size()-1,0);
    strenghtTable->addWidget(maxSpinBoxes.at(maxSpinBoxes.size()-1),maxSpinBoxes.size()-1,1);
    strenghtTable->addWidget(compressionStrengths.at(compressionStrengths.size()-1),compressionStrengths.size()-1,2);*/
}

void CompressiveStrengthDialog::removeLastEntry(){
    qTableWidget->setRowCount(qTableWidget->rowCount()-1);
}

void CompressiveStrengthDialog::initFooter(){
    qPushButtonAddEntry = new QPushButton(tr("Dodaj przedział"));
    connect(qPushButtonAddEntry, SIGNAL(clicked()), this, SLOT(addEntry()));
    qPushButtonRemoveEntry = new QPushButton(tr("Usuń ostatni przedział"));
    connect(qPushButtonRemoveEntry, SIGNAL(clicked()), this, SLOT(removeEntry()));
    //qPushButtonValidateData = new QPushButton(tr("Sprawdź poprawność danych"));
    //connect(qPushButtonValidateData, SIGNAL(clicked()), this, SLOT(runDataValidation()));
    footer = new QHBoxLayout;
    footer->addWidget(qPushButtonAddEntry);
    footer->addWidget(qPushButtonRemoveEntry);
    //footer->addWidget(qPushButtonValidateData);
}

void CompressiveStrengthDialog::initExitFooter(){
    qPushButtonOK = new QPushButton(tr("Akceptuj"));
    connect(qPushButtonOK,SIGNAL(clicked()), this, SLOT(runDataValidation()));
    qPushButtonCancel = new QPushButton(tr("Anuluj"));
    connect(qPushButtonCancel, SIGNAL(clicked()), this, SLOT(reject()));
    exitFooter = new QHBoxLayout;
    exitFooter->addWidget(qPushButtonCancel);
    exitFooter->addWidget(qPushButtonOK);
}

void CompressiveStrengthDialog::addEntry(){
    std::cout << "add clicked" << std::endl;
    addNewEntry();
}

void CompressiveStrengthDialog::removeEntry(){
    std::cout << "remove clicked" << std::endl;
    removeLastEntry();
}

void CompressiveStrengthDialog::runDataValidation(){
    QString message;
    bool data_verified;
    if(verify_data()){
        accept();
        return;
    } else {
        message.append("Dane NIE są poprawne");
        data_verified = false;
    }
    QMessageBox msgBox;
    msgBox.setText(message);
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.exec();
}

bool CompressiveStrengthDialog::verify_data(){
    bool data_valid = true;
    for(int i =0; i< qTableWidget->rowCount(); i++){
        if(!verify_single_field(i,0)){
            data_valid = false;
        }
        if(!verify_single_field(i, 1)){
            data_valid=false;
        }
        if(!verify_single_field(i, 2)){
            data_valid=false;
        }
    }
    return data_valid;
}

bool CompressiveStrengthDialog::verify_single_field(int row, int col){
    QTableWidgetItem *qTWI;
    QVariant qVariant;
    QString sValue;
    bool stringConverted;
    int value;
    qTWI = qTableWidget->takeItem(row, col);
    if(qTWI){
        qVariant = qTWI->data(0);
        if(qVariant.isValid()){
            //std::cout << "Data type: " << qVariant.type() << std::endl;
            sValue = qVariant.toString();
            value = sValue.toInt(&stringConverted);
            std::cout << "Conversion status: " << stringConverted << std::endl;
            if(stringConverted){
                //std::cout << "Obtained qVaraint: " << value << std::endl;
                qTableWidget->setItem(row, col, new QTableWidgetItem(QString::number(value)));
                return true;
            } else {
                qTableWidget->setItem(row, col, new QTableWidgetItem(sValue));
                return false;
            }
        }
        return false;
    }
    return false;
}
