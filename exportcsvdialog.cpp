#include "exportcsvdialog.h"

ExportCSVDialog::ExportCSVDialog()
{
    dataSelectLabel = new QLabel(tr("Wybierz dane do eksportu:"));

    exportRawColumns = new QCheckBox(tr("Forma szeregowa"));
    exportRawColumns->setChecked(true);
    connect(exportRawColumns, SIGNAL(toggled(bool)), this, SLOT(set_active_interval_spinBox(bool)));

    exportStackedColumns = new QCheckBox(tr("Forma kolumnowa"));
    exportStackedColumns->setChecked(true);
    connect(exportStackedColumns, SIGNAL(toggled(bool)), this, SLOT(set_active_interval_spinBox(bool)));

    exportProcessedData = new QCheckBox(tr("Forma przetworzona"));
    exportProcessedData->setChecked(true);
    connect(exportProcessedData, SIGNAL(toggled(bool)), this, SLOT(set_active_processing_checkboxes(bool)));

    exportTypeLayout = new QVBoxLayout();
    exportTypeLayout->addWidget(exportRawColumns);
    exportTypeLayout->addWidget(exportStackedColumns);
    exportTypeLayout->addWidget(exportProcessedData);

    processedDataParamLabel = new QLabel(tr("Parametry eksportu danych przetworzonych"));

    integralCheckBox = new QCheckBox(tr("Wartość obliczonej całki"));
    integralCheckBox->setChecked(true);

    derivativeCheckBox = new QCheckBox(tr("Wartość pochodnej"));
    derivativeCheckBox->setChecked(false);
    connect(derivativeCheckBox, SIGNAL(toggled(bool)), this, SLOT(set_active_interval_spinBox(bool)));
    rawDataCheckBox = new QCheckBox(tr("Dane surowe"));
    rawDataCheckBox->setChecked(false);
    connect(rawDataCheckBox, SIGNAL(toggled(bool)), this, SLOT(set_active_interval_spinBox(bool)));

    intervalLayoutLabel = new QLabel(tr("Interwał czasowy eksportowanych danych"));

    intervalLabel = new QLabel(tr("Interwał pomiędzy wpisami (sekundy)"));
    intervalSpinBox = new QSpinBox();
    intervalSpinBox->setMaximum(900);
    intervalSpinBox->setMinimum(30);
    intervalSpinBox->setSingleStep(30);
    intervalSpinBox->setToolTip(QString("Wielokrotności 30"));
    intervalSpinBox->setValue(30);
    intervalSpinBox->setDisabled(true);

    checkBoxesLayout = new QVBoxLayout();
    checkBoxesLayout->addWidget(processedDataParamLabel);
    checkBoxesLayout->addWidget(integralCheckBox);
    //checkBoxesLayout->addSpacing(4);
    checkBoxesLayout->addWidget(derivativeCheckBox);
    //checkBoxesLayout->addSpacing(4);
    checkBoxesLayout->addWidget(rawDataCheckBox);

    intervalLayout = new QHBoxLayout();
    intervalLayout->addWidget(intervalSpinBox);
    intervalLayout->addSpacing(12);
    intervalLayout->addWidget(intervalLabel);

    cancelButton = new QPushButton(tr("Anuluj"));
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(close()));
    okButton = new QPushButton(tr("OK"));
    connect(okButton, SIGNAL(clicked()), this, SLOT(accept()));

    buttons_layout = new QHBoxLayout;
    buttons_layout->addWidget(okButton);
    buttons_layout->addSpacing(20);
    buttons_layout->addWidget(cancelButton);

    mainLayout = new QVBoxLayout;
    mainLayout->addWidget(dataSelectLabel);
    mainLayout->addSpacing(12);
    mainLayout->addLayout(exportTypeLayout);
    mainLayout->addSpacing(16);
    mainLayout->addWidget(intervalLayoutLabel);
    mainLayout->addLayout(intervalLayout);
    mainLayout->addSpacing(20);
    mainLayout->addLayout(checkBoxesLayout);
    mainLayout->addSpacing(8);
    mainLayout->addLayout(buttons_layout);
    setLayout(mainLayout);
    set_active_interval_spinBox(true);
}

void ExportCSVDialog::set_active_interval_spinBox(bool active){
    if(derivativeCheckBox->isChecked() || rawDataCheckBox->isChecked() || exportRawColumns->isChecked() || exportStackedColumns->isChecked()){
        intervalSpinBox->setEnabled(true);
    } else {
        intervalSpinBox->setDisabled(true);
    }
    updateOkButtonCheckable();
}

void ExportCSVDialog::set_active_processing_checkboxes(bool active){
    integralCheckBox->setEnabled(active);
    derivativeCheckBox->setEnabled(active);
    rawDataCheckBox->setEnabled(active);
    if(!active){
        integralCheckBox->setChecked(false);
        derivativeCheckBox->setChecked(false);
        rawDataCheckBox->setChecked(false);
    }
    updateOkButtonCheckable();
}

void ExportCSVDialog::updateOkButtonCheckable(){
    if(!exportRawColumns->isChecked() && !exportStackedColumns->isChecked() && !exportProcessedData->isChecked()){
        okButton->setDisabled(true);
    } else {
        okButton->setEnabled(true);
    }
}

void ExportCSVDialog::getDataToProcess(bool *integral, bool *rawData, int *interval){
    *integral = integralCheckBox->isChecked();
    *rawData = rawDataCheckBox->isChecked();
    *interval = intervalSpinBox->value();
}

void ExportCSVDialog::getDataToProcess(bool *integral, bool *rawData, bool *derivative, int *interval){
    *integral = integralCheckBox->isChecked();
    *rawData = rawDataCheckBox->isChecked();
    *derivative = derivativeCheckBox->isChecked();
    *interval = intervalSpinBox->value();
}

bool ExportCSVDialog::getExportRawColumns(){
    return exportRawColumns->isChecked();
}

bool ExportCSVDialog::getExportStackedColumns(){
    return exportStackedColumns->isChecked();
}

bool ExportCSVDialog::getExportProcessedData(){
    return exportRawColumns->isChecked();
}

int ExportCSVDialog::getInterval(){
    return intervalSpinBox->value();
}
