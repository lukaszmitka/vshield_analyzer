#include "staytimedialog.h"

StayTimeDialog::StayTimeDialog(int time_min, int time_max)
{
    QLabel *label_time_min = new QLabel(tr("Minimalny czas postoju"));
    minStayTimeSpinBox = new QSpinBox();
    minStayTimeSpinBox->setMinimumWidth(60);
    minStayTimeSpinBox->setMaximumWidth(60);
    minStayTimeSpinBox->setMaximum(2880);
    minStayTimeSpinBox->setValue(time_min);
    QHBoxLayout *min_time_layout = new QHBoxLayout;
    min_time_layout->addWidget(label_time_min);
    min_time_layout->addWidget(minStayTimeSpinBox);

    QLabel *label_time_max = new QLabel(tr("Maksymalny czas postoju"));
    maxStayTimeSpinBox = new QSpinBox();
    maxStayTimeSpinBox->setMinimumWidth(60);
    maxStayTimeSpinBox->setMaximumWidth(60);
    maxStayTimeSpinBox->setMaximum(2880);
    maxStayTimeSpinBox->setValue(time_max);
    QHBoxLayout *max_time_layout = new QHBoxLayout;
    max_time_layout->addWidget(label_time_max);
    max_time_layout->addWidget(maxStayTimeSpinBox);

    QPushButton *cancelButton = new QPushButton(tr("Anuluj"));
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(close()));
    QPushButton *okButton = new QPushButton(tr("OK"));
    connect(okButton, SIGNAL(clicked()), this, SLOT(accept()));
    QHBoxLayout *buttons_layout = new QHBoxLayout;
    buttons_layout->addWidget(okButton);
    buttons_layout->addSpacing(20);
    buttons_layout->addWidget(cancelButton);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(min_time_layout);
    mainLayout->addSpacing(12);
    mainLayout->addLayout(max_time_layout);
    mainLayout->addSpacing(12);
    mainLayout->addLayout(buttons_layout);
    setLayout(mainLayout);
}

void StayTimeDialog::getStayTime(int *min_stay_time, int *max_stay_time){
    *min_stay_time = minStayTimeSpinBox->value();
    *max_stay_time = maxStayTimeSpinBox->value();
}
