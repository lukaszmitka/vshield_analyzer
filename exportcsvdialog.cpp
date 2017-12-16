#include "exportcsvdialog.h"

ExportCSVDialog::ExportCSVDialog()
{
    dataSelectLabel = new QLabel(tr("Wybierz dane do eksportu:"));

    integralCheckBox = new QCheckBox(tr("Wartość obliczonej całki"));
    integralCheckBox->setChecked(true);

    derivativeCheckBox = new QCheckBox(tr("Wartość pochodnej"));
    derivativeCheckBox->setChecked(false);
    connect(derivativeCheckBox, SIGNAL(toggled(bool)), this, SLOT(set_active_interval_spinBox(bool)));
    rawDataCheckBox = new QCheckBox(tr("Dane surowe"));
    rawDataCheckBox->setChecked(false);
    connect(rawDataCheckBox, SIGNAL(toggled(bool)), this, SLOT(set_active_interval_spinBox(bool)));

    intervalLabel = new QLabel(tr("Interwał pomiędzy wpisami (sekundy)"));
    intervalSpinBox = new QSpinBox();
    intervalSpinBox->setMaximum(900);
    intervalSpinBox->setMinimum(30);
    intervalSpinBox->setSingleStep(30);
    intervalSpinBox->setToolTip(QString("Wielokrotności 30"));
    intervalSpinBox->setValue(30);
    intervalSpinBox->setDisabled(true);

    checkBoxesLayout = new QVBoxLayout();
    checkBoxesLayout->addWidget(integralCheckBox);
    checkBoxesLayout->addSpacing(12);
    checkBoxesLayout->addWidget(derivativeCheckBox);
    checkBoxesLayout->addSpacing(12);
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
    mainLayout->addLayout(checkBoxesLayout);
    mainLayout->addSpacing(12);
    mainLayout->addLayout(intervalLayout);
    mainLayout->addSpacing(12);
    mainLayout->addLayout(buttons_layout);
    setLayout(mainLayout);
}

void ExportCSVDialog::set_active_interval_spinBox(bool active){
    if(derivativeCheckBox->isChecked() || rawDataCheckBox->isChecked()){
        intervalSpinBox->setEnabled(true);
    } else {
        intervalSpinBox->setDisabled(true);
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
