#include <QApplication>
#include <QFont>
#include <QLabel>
#include <QPalette>
#include <QPushButton>
#include <QSlider>
#include <atomic>
#include <chrono>
#include <csignal>
#include <iostream>
#include <thread>

#include "Joystick.h"
#include "Network.h"

Network network;
Joystick joystick;

std::atomic<bool> running(true);
std::atomic<double> sideAxis(0);
std::atomic<double> forwardsAxis(0);

void handleEnable();
void handleDisable();
void signalHandler(int signum);
void cleanUp();

QPushButton *enableButton;
QPushButton *disableButton;
std::thread networkThread;
std::thread inputThread;

int main(int argc, char *argv[]) {
    std::signal(SIGINT, signalHandler);

    network.init();
    joystick.init();

    inputThread = std::thread([]() {
        joystick.readJoystickInput(running, sideAxis, forwardsAxis);
    });

    networkThread = std::thread([]() {
        network.sendPacket(sideAxis, forwardsAxis);
    });

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

    // QSlider *slider = new QSlider(&window);
    // slider->setOrientation(Qt::Horizontal);
    // slider->setRange(0, 100);
    // slider->setValue(0);
    // slider->setGeometry(10, 90, 180, 30);

    // QLabel *forwardsAxisLabel = new QLabel(&window);

    // forwardsAxisLabel->setText(QString::number(forwardsAxis));

    QObject::connect(enableButton, &QPushButton::clicked, handleEnable);
    QObject::connect(disableButton, &QPushButton::clicked, handleDisable);

    window.show();

    // QT loop
    int result = a.exec();

    cleanUp();
    return result;
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

void signalHandler(int signum) {
    std::cout << "Interrupt signal " << signum << " received\n";
    running = false;
    cleanUp();
    exit(signum);
}

void cleanUp() {
    running = false;
    if (inputThread.joinable()) {
        inputThread.join();
        std::cout << "Input thread shut down\n";
    }

    if (networkThread.joinable()) {
        networkThread.join();
        std::cout << "Network thread shut down\n";
    }
    joystick.cleanUp();
    std::cout << "Clean up finished";
}
