#pragma once

// #include <thread>
#include <string>
#include "gpio_cxx.hpp"
#include "MqttClient.hpp"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_wifi.h"
#include "esp_mqtt.hpp"

using namespace std;

class IotDevice;

class Node: public MqttCaller
{
protected:
  IotDevice *device;
  string name;
  string state;
  string type;
  string topic;
  void storeSettings();
  void readSettings();
public:
  Node(IotDevice *_device, string _topic, string _name, string _type);
  void publish(string _topic, string message,mqtt::QoS qos, bool retain);
  void init();
  void setTopic(string _topic);
  string getTopic();
  virtual void read();
  virtual void onChange(){};
  virtual void execute(string command) {printf(command.c_str());};
  virtual void cMessageReceivedCallback(const string &topicStr, const string &message) override;
};

// class WifiStrengthSensor: public Sensor
// {
// private:
//   wifi_ap_record_t ap;
//   int value = 0;
//   int percent = 0;
//   int clamp(int value, int min, int max);
// public:
//   WifiStrengthSensor(IotDevice *_device, string _topic);
//   void read() override;
//   void onChange() override;
// };

// class LedOutput: public Output
// {
// private:
//   int ledPin;
//   bool state;
//   idf::GPIO_Output gpio_0;
// public:
//   LedOutput(IotDevice *_device, string _topic, int _ledPin = 2);
//   void execute(string command) override;
// };





