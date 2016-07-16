#include "mainwindow.h"
#include <QApplication>

//#include "facestate.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    // TODO remove before deploument
    a.addLibraryPath("C:/Qt/5.7/msvc2015_64/plugins");
    MainWindow w;
    w.show();
    return a.exec();
}


/*
 * Shields test
 *
 * Shield shield_a(0,100,100,100);
    Shield shield_b(1,101,101,101);
    Shield shield_c(2,102,102,102);
    Shield shield_d(3,103,103,103);
    Shield shield_e(4,104,104,104);

    std::vector <Shield> shields;
    shields.push_back(shield_a);
    shields.push_back(shield_b);
    shields.push_back(shield_c);
    shields.push_back(shield_d);
    shields.push_back(shield_e);


    for (unsigned i=0; i<shields.size(); i++){
        std::cout <<shields.at(i).string_desc() << std::endl;
    }

    std::cout <<"Updating shields..." << std::endl;
    QDateTime ts = QDateTime();
    ts = ts.currentDateTime();
    FaceState faceState(ts, shields);
    Shield shield_to_up(2, 106,106,106);
    faceState.update_state(shield_to_up);

    std::cout <<"Done." << std::endl;

    shields = faceState.get_state();
    for (unsigned i=0; i<shields.size(); i++){
        std::cout <<shields.at(i).string_desc() << std::endl;
    }

    */
