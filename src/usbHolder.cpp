#include <QDebug>
#include <QCoreApplication>

#include <thread>
#include <chrono>
#include <future>

#include <cmath>
#include <cstdlib>

#include "usbHolder.h"

USBHolder::USBHolder(QObject *parent) : QObject{parent}
{
  auto gamepads = QGamepadManager::instance()->connectedGamepads();
  m_data.resize(4);
  m_perefData.resize(4);

  if(gamepads.isEmpty())
  {
    qDebug() << "[-] Gamepad is not connected\n";
    return;
  }
  else qDebug() << "[+] Gamepad detected\n";

  emit sig_setPowerLimit(100);
  m_camerasPositions_arr = {0, 0};

  m_gamepad_ptr = std::make_shared<QGamepad>(*gamepads.begin(), this);
  connect(m_gamepad_ptr.get(), &QGamepad::axisLeftXChanged, this, [this](double value)
  {
    qDebug() << "Left X" << value;
    m_data[2] = int8_t(m_powerLimit * value);
    emit joystickData(m_data, 2);
//    printControlData();
  });
  connect(m_gamepad_ptr.get(), &QGamepad::axisLeftYChanged, this, [this](double value)
  {
    qDebug() << "Left Y" << value;
    m_data[3] = int8_t(m_powerLimit * value);
    emit joystickData(m_data, 2);
//    printControlData();
  });
  connect(m_gamepad_ptr.get(), &QGamepad::axisRightXChanged, this, [this](double value)
  {
    qDebug() << "Right X" << value;
    m_data[1] = int8_t(m_powerLimit * value);
    emit joystickData(m_data, 2);
//    printControlData();
  });

  connect(m_gamepad_ptr.get(), &QGamepad::axisRightYChanged, this, [this](double value){
    qDebug() << "Right Y" << value;
    m_data[0] = int8_t(m_powerLimit * value);
    emit joystickData(m_data, 2);
//    printControlData();
  });

  connect(m_gamepad_ptr.get(), &QGamepad::buttonXChanged, this, [this](bool pressed){
    qDebug() << "Button X" << pressed;
    emit sig_setPowerLimit(10);

  });
 connect(m_gamepad_ptr.get(), &QGamepad::buttonBChanged, this, [this](bool pressed){
    qDebug() << "Button B" << pressed;
    emit sig_setPowerLimit(25);

  });
  connect(m_gamepad_ptr.get(), &QGamepad::buttonAChanged, this, [this](bool pressed){
    qDebug() << "Button A" << pressed;
    emit sig_setPowerLimit(50);

  });
  connect(m_gamepad_ptr.get(), &QGamepad::buttonYChanged, this, [this](bool pressed){
    qDebug() << "Button Y" << pressed;
    emit sig_setPowerLimit(75);

  });
           connect(m_gamepad_ptr.get(), &QGamepad::buttonR3Changed, this, [](bool pressed){
          qDebug() << "Button R3" << pressed;
      });
      connect(m_gamepad_ptr.get(), &QGamepad::buttonL3Changed, this, [](bool pressed){
          qDebug() << "Button L3" << pressed;
      });

  connect(m_gamepad_ptr.get(), &QGamepad::buttonL1Changed, this, [this](bool pressed)
  {
    qDebug() << "Button L1" << pressed;
    m_perefData[2] = ((pressed) ? -100 : 0);
    emit joystickData(m_perefData, 29);

  });
  connect(m_gamepad_ptr.get(), &QGamepad::buttonR1Changed, this, [this](bool pressed)
  {
    qDebug() << "Button R1" << pressed;
    m_perefData[2] = ((pressed) ? 100 : 0);
    emit joystickData(m_perefData, 29);

  });

  connect(m_gamepad_ptr.get(), &QGamepad::buttonL2Changed, this, [this](double value)
  {
    qDebug() << "Button L2: " << value;
    m_perefData[3] = ((value > 0.01) ? -100 : 0);
    emit joystickData(m_perefData, 29);
  });
  connect(m_gamepad_ptr.get(), &QGamepad::buttonR2Changed, this, [this](double value)
  {
    qDebug() << "Button R2: " << value;
    m_perefData[3] = ((value > 0.01) ? 100 : 0);
    emit joystickData(m_perefData, 29);

  });

  connect(m_gamepad_ptr.get(), &QGamepad::buttonSelectChanged, this, [](bool pressed)
  {
    qDebug() << "Button Select" << pressed;
  });
      connect(m_gamepad_ptr.get(), &QGamepad::buttonCenterChanged, this, [](bool pressed){
          qDebug() << "Button Center" << pressed;
      });


  connect(m_gamepad_ptr.get(), &QGamepad::buttonDownChanged, this, [this](bool pressed)
  {
    qDebug() << "Button Down" << pressed;
    int8_t &pos = m_camerasPositions_arr[0];
    pos = (pos < -90) ? -100 : (pos - 10);
    m_perefData[0] = pos;
    emit joystickData(m_perefData, 29);
    emit sig_camerasPositions(m_camerasPositions_arr);

  });
  connect(m_gamepad_ptr.get(), &QGamepad::buttonUpChanged, this, [this](bool pressed)
  {
    qDebug() << "Button Up" << pressed;
    int8_t &pos = m_camerasPositions_arr[0];
    pos = (pos > 90) ? 100 : (pos + 10);
    m_perefData[0] = pos;
    emit joystickData(m_perefData, 29);
    emit sig_camerasPositions(m_camerasPositions_arr);

  });
  connect(m_gamepad_ptr.get(), &QGamepad::buttonLeftChanged, this, [this](bool pressed)
  {
    qDebug() << "Button Left" << pressed;
    int8_t &pos = m_camerasPositions_arr[1];
    pos = (pos < -90) ? -100 : (pos - 10);
    m_perefData[1] = pos;
    emit joystickData(m_perefData, 29);
    emit sig_camerasPositions(m_camerasPositions_arr);

  });
  connect(m_gamepad_ptr.get(), &QGamepad::buttonRightChanged, this, [this](bool pressed)
  {
    qDebug() << "Button Right" << pressed;
    int8_t &pos = m_camerasPositions_arr[1];
    pos = (pos > 90) ? 100 : (pos + 10);
    m_perefData[1] = pos;
    emit joystickData(m_perefData, 29);
    emit sig_camerasPositions(m_camerasPositions_arr);

  });


      connect(m_gamepad_ptr.get(), &QGamepad::buttonStartChanged, this, [](bool pressed){
          qDebug() << "Button Start" << pressed;
      });

      connect(m_gamepad_ptr.get(), &QGamepad::buttonGuideChanged, this, [](bool pressed){
          qDebug() << "Button Guide" << pressed;
      });
  
}
USBHolder::~USBHolder()
{
}
void USBHolder::setPowerLimit(uint8_t vl)
{
  if(vl <= 100) m_powerLimit = vl;
}
/*void USBHolder::printRawData()
{
}
*/
void USBHolder::printControlData()
{
  std::system("clear");
  for(uint8_t i = 0; i < 4; ++i)
  {
    qDebug() << int8_t(m_data[i]) << " ";
  }
}

void USBHolder::readJoystickData()
{

}
void USBHolder::s_openDevice()
{
}
void USBHolder::s_processEvents()
{
}

void USBHolder::s_setCamerasPositions(const std::array<int8_t, 2> &camerasPositions)
{

}
/*
void USBHolder::s_openDevice()
{
}
void USBHolder::closeDevice()
{
}
*/


