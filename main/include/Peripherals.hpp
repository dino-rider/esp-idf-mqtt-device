#pragma once

// #include <thread>
#include <string>
#include "gpio_cxx.hpp"
#include "MqttClient.hpp"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

using namespace std;

class IotDevice; // forward declaration

class Peripheral: public MqttCaller
{
protected:
  IotDevice *device;
  string topic;
  void storeState();
  void readState();
public:
  Peripheral(IotDevice *_device, string _topic);
  void publish(string message, bool retain);
  void subscribe();
  string read();
  void setTopic(string _topic);
  string getTopic();
  virtual void cMessageReceivedCallback(const string &topicStr, const string &message);
};

class Sensor: public Peripheral
{
public:
  Sensor(IotDevice *_device, string _topic);
  virtual void onChange(){};
  virtual void cMessageReceivedCallback(const string &topicStr, const string &message);
};

class Output: public Peripheral
{
public:
  Output(IotDevice *_device, string _topic);
  virtual void execute(string command) {printf(command.c_str());};
  virtual void cMessageReceivedCallback(const string &topicStr, const string &message);
};

class LedOutput: public Output
{
private:
  int ledPin;
  bool state;
public:
  LedOutput(IotDevice *_device, string _topic, int _ledPin = 2);
  void execute(string &command);
};

// void Demo_Task(void *arg)
// {
//     for(;;)
//     {
//     while (!connected_flag)
//     {
//         printf("LED ON\n");
//         gpio.set_high();
//         vTaskDelay(500/ portTICK_RATE_MS);
//         printf("LED OFF\n");
//         gpio.set_low();
//         vTaskDelay(500/ portTICK_RATE_MS);
//     }
//     while (connected_flag)
//     {
//     vTaskDelay(500/ portTICK_RATE_MS);
//     }
//     }
// }

// TaskHandle_t myTaskHandle = NULL;

