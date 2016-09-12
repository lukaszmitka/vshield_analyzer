#ifndef INDEXEDQCHECKBOX_H
#define INDEXEDQCHECKBOX_H

#include <QString>
#include <QCheckBox>

class IndexedQCheckBox : public QCheckBox
{
public:
    using QCheckBox::QCheckBox;
    void setId(int id){checkBoxId = id;}
    int getId(){return checkBoxId;}
private:
    int checkBoxId;
};

#endif // INDEXEDQCHECKBOX_H
