#include "pressuregainindexdialog.h"
#include "indexedqcheckbox.h"

PressureGainIndexDialog::PressureGainIndexDialog(int shield_count)
{
    QLabel *select_label = new QLabel(tr("Wybierz sekcje do analizy:"));
    int cols = shield_count/20;
    int last_col_height = shield_count % 20;

    std::cout << "Add " << shield_count << " shields"<< std::endl;
    QHBoxLayout *shield_cols = new QHBoxLayout;
    for(int c=0; c<cols; c++){
        QVBoxLayout *shields_layout = new QVBoxLayout;
        for(int i=0; i<20; i++){
            QString checkBoxLabel("Sekcja ");
            checkBoxLabel.append(QString::number((c*20) + i+1));
            IndexedQCheckBox *iqcb = new IndexedQCheckBox(checkBoxLabel);
            iqcb->setId((c*20) + i+1);
            shields_layout->addWidget(iqcb);
        }
        shield_cols->addLayout(shields_layout);
    }

    QVBoxLayout *last_col = new QVBoxLayout;
    for(int i=0; i<last_col_height; i++){
        QString checkBoxLabel("Sekcja ");
        checkBoxLabel.append(QString::number((cols*20) + i+1));
        IndexedQCheckBox *iqcb = new IndexedQCheckBox(checkBoxLabel);
        iqcb->setId((cols*20) + i+1);
        last_col->addWidget(iqcb);
    }
    shield_cols->addLayout(last_col);

    QPushButton *checkAllButton = new QPushButton(tr("Zaznacz wszytskie"));
    connect(checkAllButton, SIGNAL(clicked()), this, SLOT(check_all()));
    QPushButton *uncheckAllButton = new QPushButton(tr("Odznacz wszystkie"));
    connect(uncheckAllButton, SIGNAL(clicked()), this, SLOT(uncheck_all()));

    QPushButton *cancelButton = new QPushButton(tr("Anuluj"));
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(close()));
    QPushButton *okButton = new QPushButton(tr("OK"));
    connect(okButton, SIGNAL(clicked()), this, SLOT(accept()));


    QHBoxLayout *buttons_layout = new QHBoxLayout;
    buttons_layout->addWidget(checkAllButton);
    buttons_layout->addSpacing(20);
    buttons_layout->addWidget(uncheckAllButton);
    buttons_layout->addSpacing(80);
    buttons_layout->addWidget(okButton);
    buttons_layout->addSpacing(20);
    buttons_layout->addWidget(cancelButton);

    mainLayout = new QVBoxLayout;
    mainLayout->addWidget(select_label);
    mainLayout->addSpacing(12);
    mainLayout->addLayout(shield_cols);
    mainLayout->addSpacing(12);
    mainLayout->addLayout(buttons_layout);

    setLayout(mainLayout);
}

std::vector <int> PressureGainIndexDialog::getChosenShields(){
    std::vector<int> shields;
    //std::cout << "number of layout elements: " << mainLayout->count() << std::endl;
    for (int i = 0; i < mainLayout->count(); ++i)
    {
        //std::cout << "i = " << i << std::endl;
        QLayout *hlayout = mainLayout->itemAt(i)->layout();
        if (hlayout != NULL)
        {
            for (int j = 0; j < hlayout->count(); ++j)
            {
                //std::cout << "j = " << j << std::endl;
                QLayout *vlayout = hlayout->itemAt(j)->layout();
                if (vlayout != NULL)
                {
                    for (int k = 0; k < vlayout->count(); ++k){
                        IndexedQCheckBox *indexedQCB = (IndexedQCheckBox*)vlayout->itemAt(k)->widget();
                        if(indexedQCB->isChecked()){
                            int shield_id = indexedQCB->getId();
                            //std::cout << "found widget number " << shield_id << " with text: " << indexedQCB->text().toLocal8Bit().data() << std::endl;
                            shields.push_back(shield_id);
                        }
                    }
                }
            }
        }
    }
    return shields;
}

void PressureGainIndexDialog::check_all(){
    for (int i = 0; i < mainLayout->count(); ++i)
    {
        QLayout *hlayout = mainLayout->itemAt(i)->layout();
        if (hlayout != NULL)
        {
            for (int j = 0; j < hlayout->count(); ++j)
            {
                QLayout *vlayout = hlayout->itemAt(j)->layout();
                if (vlayout != NULL)
                {
                    for (int k = 0; k < vlayout->count(); ++k){
                        IndexedQCheckBox *indexedQCB = (IndexedQCheckBox*)vlayout->itemAt(k)->widget();
                        indexedQCB->setChecked(true);
                    }
                }
            }
        }
    }
}

void PressureGainIndexDialog::uncheck_all(){
    for (int i = 0; i < mainLayout->count(); ++i)
    {
        QLayout *hlayout = mainLayout->itemAt(i)->layout();
        if (hlayout != NULL)
        {
            for (int j = 0; j < hlayout->count(); ++j)
            {
                QLayout *vlayout = hlayout->itemAt(j)->layout();
                if (vlayout != NULL)
                {
                    for (int k = 0; k < vlayout->count(); ++k){
                        IndexedQCheckBox *indexedQCB = (IndexedQCheckBox*)vlayout->itemAt(k)->widget();
                        indexedQCB->setChecked(false);
                    }
                }
            }
        }
    }
}
