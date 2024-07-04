#include <QApplication>
#include <QFont>
#include <QLabel>
#include <QPalette>
#include <QPushButton>
#include <QSlider>
#include <iostream>

#include "Joystick.h"
#include "Network.h"

Network network;
Joystick joystick;

void handleEnable();

void handleDisable();

QPushButton *enableButton;
QPushButton *disableButton;

int main(int argc, char *argv[]) {
    network.Start();
    joystick.joystick();

    QApplication a(argc, argv);

    QWidget window;
    window.setMinimumSize(500, 500);
    window.setWindowTitle("µChariot Driverstation");

    QLabel *enableHeader = new QLabel(&window);
    enableHeader->setText("Enable/Disable");
    enableHeader->setGeometry(10, 10, 100, 10);

    enableButton = new QPushButton("Enable", &window);
    disableButton = new QPushButton("Disable", &window);

    enableButton->setGeometry(10, 30, 80, 30);
    enableButton->setFlat(true);
    disableButton->setGeometry(90, 30, 80, 30);
    disableButton->setFlat(true);

    handleDisable();

    QSlider *slider = new QSlider(&window);
    slider->setOrientation(Qt::Horizontal);
    slider->setRange(0, 100);
    slider->setValue(0);
    slider->setGeometry(10, 90, 180, 30);

    QObject::connect(enableButton, &QPushButton::clicked, handleEnable);
    QObject::connect(disableButton, &QPushButton::clicked, handleDisable);

    window.show();
    return a.exec();
}

void changeButtonColor(QPushButton *button, Qt::GlobalColor color) {
    QPalette pal = button->palette();
    pal.setColor(QPalette::Button, QColor(color));

    button->setAutoFillBackground(true);
    button->setPalette(pal);
    button->update();
}

void handleEnable() {
    changeButtonColor(disableButton, Qt::gray);
    changeButtonColor(enableButton, Qt::green);
}

void handleDisable() {
    changeButtonColor(enableButton, Qt::gray);
    changeButtonColor(disableButton, Qt::red);
}
