#include "compressivestrengthdialog.h"

CompressiveStrengthDialog::CompressiveStrengthDialog()
{
    init_layout();
}


CompressiveStrengthDialog::CompressiveStrengthDialog(std::vector<single_entry> current_data){
    init_layout();
    qTableWidget->setRowCount((int)current_data.size());
    for(int i=0; i<current_data.size(); i++){
        qTableWidget->setItem(i, 0, new QTableWidgetItem(QString::number(current_data[i].begin)));
        qTableWidget->setItem(i, 1, new QTableWidgetItem(QString::number(current_data[i].end)));
        qTableWidget->setItem(i, 2, new QTableWidgetItem(QString::number(current_data[i].strength)));
    }
}

void CompressiveStrengthDialog::init_layout(){
    qTableWidget = new QTableWidget(this);
    qTableWidget->setRowCount(5);
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

}


/**
 * @brief CompressiveStrengthDialog::getCompressiveStrengths Read all data from dialog table
 * @return Vector of structs containing table data.
 */
std::vector<single_entry> CompressiveStrengthDialog::getCompressiveStrengths(){
    std::vector<single_entry> strengths;
    for (int i=0; i<qTableWidget->rowCount(); i++){
        strengths.push_back(read_single_entry(i));
    }
    return strengths;
}

/**
 * @brief CompressiveStrengthDialog::read_single_entry Read selected row from dialog table.
 * @param row Row number to read
 * @return struct containing data from selected row
 */
single_entry CompressiveStrengthDialog::read_single_entry(int row){
    single_entry entry;
    QTableWidgetItem *qTWI;
    QVariant qVariant;
    int value;
    for (int i=0; i<3; i++){
        qTWI = qTableWidget->takeItem(row, i);
        qVariant = qTWI->data(0);
        value = qVariant.toInt();
        switch (i) {
        case 0:
            entry.begin=value;
            break;
        case 1:
            entry.end=value;
        case 2:
            entry.strength=value;
        default:
            break;
        }
    }
    return entry;
}

/**
 * @brief CompressiveStrengthDialog::addNewEntry Add new row at the end of table
 */
void CompressiveStrengthDialog::addNewEntry(){
    qTableWidget->setRowCount(qTableWidget->rowCount()+1);
}

/**
 * @brief CompressiveStrengthDialog::removeLastEntry Remove last row from table, will execute regardless of data existing in row.
 */
void CompressiveStrengthDialog::removeLastEntry(){
    qTableWidget->setRowCount(qTableWidget->rowCount()-1);
}

/**
 * @brief CompressiveStrengthDialog::initFooter Initialize all buttons in table footer.
 */
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

/**
 * @brief CompressiveStrengthDialog::initExitFooter Initialize accept and cancel button.
 */
void CompressiveStrengthDialog::initExitFooter(){
    qPushButtonOK = new QPushButton(tr("Akceptuj"));
    connect(qPushButtonOK,SIGNAL(clicked()), this, SLOT(runDataValidation()));
    qPushButtonCancel = new QPushButton(tr("Anuluj"));
    connect(qPushButtonCancel, SIGNAL(clicked()), this, SLOT(reject()));
    exitFooter = new QHBoxLayout;
    exitFooter->addWidget(qPushButtonCancel);
    exitFooter->addWidget(qPushButtonOK);
}

/**
 * @brief CompressiveStrengthDialog::addEntry Slot for adding new table row.
 */
void CompressiveStrengthDialog::addEntry(){
    std::cout << "add clicked" << std::endl;
    addNewEntry();
}

/**
 * @brief CompressiveStrengthDialog::removeEntry Slot for removing last table row.
 */
void CompressiveStrengthDialog::removeEntry(){
    std::cout << "remove clicked" << std::endl;
    removeLastEntry();
}

/**
 * @brief CompressiveStrengthDialog::runDataValidation Slot for executon validation of data provided by user in table.
 */
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

/**
 * @brief CompressiveStrengthDialog::verify_data Check if data provided by user is correct.
 * @return True if data is correct, false otherwise.
 */
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

/**
 * @brief CompressiveStrengthDialog::verify_single_field Check if selected cell contain correct data.
 * @param row Row of table to be checked.
 * @param col Column of table to be checked.
 * @return True if cell contain correct data.
 */
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
