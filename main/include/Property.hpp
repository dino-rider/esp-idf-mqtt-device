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
  Property(string _name, string id, Node *_node, PROPERTY_TYPE type, bool settable, bool retained,
           string _data_type, string _format = "", string _unit = "");

  // ----------------------------Constructor for Device`s properties
  Property(string name, string id, IotDevice *_device, PROPERTY_TYPE type, bool settable, bool retained,
           string data_type, string _format = "", string _unit = "");

  ~Property() { printf("Property %s destroyed\r\n", name.c_str()); }

  virtual bool init();
  void publish(string _topic, string message, mqtt::QoS qos, bool retain);
  void subscribe();
  void setTopic(string _topic);
  string getTopic();
  virtual void read();
  virtual void cMessageReceivedCallback(const string &topicStr, const string &message) override;

  // string GetValue() const;

  string getName() const {return name;};
  PROPERTY_TYPE getType() const {return type;};
  
  // IotDevice *GetNode() const;
  // Device *GetDevice() const;
  // bool HasNewValue();
  virtual bool Validate(string value);
  // void ClearError();
  // void SetError(string code, string message);

  // void SetValue(string value);
  // void SetHasNewValue(bool has_new_value);

  // virtual void HandleCurrentState();
  // virtual void HandleSettingNewValue();

protected:
  // bool has_new_value_ = false;

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
