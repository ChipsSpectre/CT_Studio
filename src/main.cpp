
#include <iostream>

#include <QApplication>

#include "gui/MainWindow.h"

/**
 * Main function.
 * Entry point for program execution.
 */
int main(int argc, char *argv[]){
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
