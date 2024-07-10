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
std::atomic<bool> enabled(false);
std::atomic<double> sideAxis(0);
std::atomic<double> forwardsAxis(0);
std::atomic<double> maxSpeed(2500);

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
        network.sendJoystickPackets(sideAxis, forwardsAxis, maxSpeed, enabled);
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

    QLabel *maxSpeedLabel = new QLabel(&window);
    maxSpeedLabel->setGeometry(10, 70, 180, 30);
    maxSpeedLabel->setText("Max Speed: 2500");

    QSlider *slider = new QSlider(&window);
    slider->setOrientation(Qt::Horizontal);
    slider->setRange(0, 5000);
    slider->setValue(2500);
    slider->setGeometry(10, 100, 180, 30);

    QObject::connect(enableButton, &QPushButton::clicked, handleEnable);
    QObject::connect(disableButton, &QPushButton::clicked, handleDisable);
    QObject::connect(slider, &QSlider::valueChanged, [&](int value) {
        maxSpeed.store(value);
        maxSpeedLabel->setText("Max Speed: " + QString::number(value));
    });

    window.show();

    // QT loop
    int result = a.exec();

    cleanUp();
    return result;
}

void value(int k) {
    std::cout << k << std::endl;
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
    enabled.store(true);
}

void handleDisable() {
    changeButtonColor(enableButton, Qt::gray);
    changeButtonColor(disableButton, Qt::red);
    enabled.store(false);
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
