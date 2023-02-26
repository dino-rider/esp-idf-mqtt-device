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

class Peripheral: public MqttCaller
{
protected:
  IotDevice *device;
  string topic;
  void storeSettings();
  void readSettings();
public:
  Peripheral(IotDevice *_device, string _topic);
  void publish(string message,mqtt::QoS qos, bool retain);
  void subscribe();
  void setTopic(string _topic);
  string getTopic();
  virtual void read();
  virtual void cMessageReceivedCallback(const string &topicStr, const string &message) override;
};

class Sensor: public Peripheral
{
public:
  Sensor(IotDevice *_device, string _topic);
  virtual void onChange(){};
  virtual void cMessageReceivedCallback(const string &topicStr, const string &message) override;
};

class Output: public Peripheral
{
public:
  Output(IotDevice *_device, string _topic);
  virtual void execute(string command) {printf(command.c_str());};
  virtual void cMessageReceivedCallback(const string &topicStr, const string &message) override;
};

class WifiStrengthSensor: public Sensor
{
private:
  wifi_ap_record_t ap;
  int value = 0;
  int percent = 0;
  int clamp(int value, int min, int max);
public:
  WifiStrengthSensor(IotDevice *_device, string _topic);
  void read() override;
  void onChange() override;
};

class LedOutput: public Output
{
private:
  int ledPin;
  bool state;
  idf::GPIO_Output gpio_0;
public:
  LedOutput(IotDevice *_device, string _topic, int _ledPin = 2);
  void execute(string command) override;
};





