#pragma once

#include <iostream>
#include <string>
#include <vector>

#include <QDialog>
#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QSpinBox>
#include <QListWidget>
#include <QListWidgetItem>
#include <QPushButton>

/**
 * The phantom dialog enables selection of 
 * appropiate phantoms. Size can be adjusted, too.
 */
class PhantomDialog: public QDialog {
    Q_OBJECT
private:
    std::vector<char*> _names;
    int _sideLength;
    std::string _phantomName;
    QSpinBox *_lengthSelector;
    QListWidget *_listWidget;
    QPushButton *_loadButton;
public:
PhantomDialog(QWidget* parent = 0);

void updateSideLength() {
    _sideLength = _lengthSelector->value();
}

int getSideLength();
std::string getPhantomName();
};