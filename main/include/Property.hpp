#pragma once

// #include <thread>
#include <string>
#include "gpio_cxx.hpp"
#include "MqttClient.hpp"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_wifi.h"
#include "esp_mqtt.hpp"
#include "IotDevice.hpp"
#include "Node.hpp"



using namespace std;

class IotDevice;
class Node;

enum PROPERTY_TYPE
{
  OPTION,
  TELEMETRY,
  SENSOR
};

class Property : public MqttCaller
{
public:
  // ----------------------------Constructor for Node`s properties
  Property(string _name, string _id, Node *_node, PROPERTY_TYPE _type, bool _settable, bool _retained,
           string _data_type, string _format = "", string _unit = "");

  // ----------------------------Constructor for Device`s properties
  Property(string _name, string _id, IotDevice *_device, PROPERTY_TYPE _type, bool _settable, bool _retained,
           string _data_type, string _format = "", string _unit = "");

  ~Property() { printf("Property %s destroyed\r\n", name.c_str()); }

  virtual bool init();
  void publish(string _topic, string message, mqtt::QoS qos, bool retain);
  void publishNotification(string message);
  void publishError(string message);
  void subscribe();
  void setTopic(string _topic);
  string getTopic();
  virtual void read();
  virtual void cMessageReceivedCallback(const string &topicStr, const string &message) override;

  // string GetValue() const;

  string getName() const {return name;};
  PROPERTY_TYPE getType() const {return type;};

protected:
  virtual bool Validate(string value);
  string name;
  string id;
  PROPERTY_TYPE type;
  bool settable;
  bool retained;
  string data_type;
  string format;
  string unit;
  string value = "";
  string error_code = "";
  string error_message = "";
  IotDevice *device = nullptr;
  Node *node = nullptr;
  string topic;
};

class ButtonNotificationProperty: private Property
{
public:
  ButtonNotificationProperty(string _name, string _id, Node *_node, PROPERTY_TYPE _type, bool _settable, bool _retained,
           string _data_type, string _format = "", string _unit = "");

  ButtonNotificationProperty(string _name, string _id, IotDevice *_device, PROPERTY_TYPE _type, bool _settable, bool _retained,
           string _data_type, string _format = "", string _unit = "");

  void setFlag(bool *_flag);
private:
  bool *flag = nullptr;
  bool state = false;
  void onChange();
  void read();
};

class ValidationTestProperty: private Property
{
public:
  ValidationTestProperty(string _name, string _id, Node *_node, PROPERTY_TYPE _type, bool _settable, bool _retained,
           string _data_type, string _format = "", string _unit = "");

  ValidationTestProperty(string _name, string _id, IotDevice *_device, PROPERTY_TYPE _type, bool _settable, bool _retained,
           string _data_type, string _format = "", string _unit = "");

  bool Validate(string value) override;
private:
  void cMessageReceivedCallback(const string &topicStr, const string &message) override;
};
