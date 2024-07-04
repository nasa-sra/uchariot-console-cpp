#include <QApplication>
#include <QPushButton>
#include <QSlider>
#include <QLabel>
#include <QFont>
#include <iostream>
#include "include/Network.h"

Network network;

void handleButton();


int main(int argc, char *argv[])
{
    network.Start();

    QApplication a(argc, argv);

    QWidget window;
    window.setMinimumSize(500, 500);
    window.setWindowTitle("µChariot Driverstation");

    QLabel* enableHeader = new QLabel(&window);
    enableHeader->setText("Enable/Disable");
    enableHeader->setGeometry(10, 10, 100, 10);

    QPushButton *enableButton = new QPushButton("Enable", &window);
    QPushButton *disableButton = new QPushButton("Disable", &window);

    enableButton->setGeometry(10, 30, 80, 30);
    disableButton->setGeometry(90, 30, 80, 30);

    QLabel *sliderLabel = new QLabel(&window);
    sliderLabel->setText("Max Speed");
    sliderLabel->setGeometry(10, 70, 100, 10);

    QSlider *slider = new QSlider(&window);
    slider->setOrientation(Qt::Horizontal);
    slider->setRange(0, 100);
    slider->setValue(0);
    slider->setGeometry(10, 90, 180, 30);

    QObject::connect(enableButton, &QPushButton::clicked, handleButton);

    window.show();
    return a.exec();
}

void handleButton()
{
    std::cout << "enabled" << std::endl;
    network.sendPacket();
}
