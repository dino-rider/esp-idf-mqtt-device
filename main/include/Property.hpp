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
class Sensor;

class Property: public MqttCaller
{
public:
    // ----------------------------Constructor for Node`s properties
    Property(const char* _name, const char* id, Node* _node, PROPERTY_TYPE type, bool settable, bool retained,
             const char* _data_type, string _format = "", string _unit = "");

    // ----------------------------Constructor for Device`s properties
    Property(const char* name, const char* id, IotDevice* _device, PROPERTY_TYPE type, bool settable, bool retained,
             const char* data_type, string _format = "", string _unit = "");

    ~Property() { Serial.printf("Property %s destroyed\r\n", name_.c_str()); }

    virtual bool Init(Homie* homie);

    String GetValue() const;
    String GetId() const;
    PROPERTY_TYPE GetType() const;
    Node* GetNode() const;
    Device* GetDevice() const;

    bool HasNewValue();
    virtual bool Validate(String value);
    void ClearError();
    void SetError(String code, String message);

    void SetValue(String value);
    void SetHasNewValue(bool has_new_value);

    virtual void HandleCurrentState();

    virtual void HandleSettingNewValue();
    bool Subscribe(Homie* homie);

 protected:
    PROPERTY_TYPE type_;

    bool has_new_value_ = false;
    bool settable_;
    bool retained_;

    string name_ = "";
    string id_ = "";
    string data_type_ = "";
    string format_ = "";
    string unit_ = "";
    string value_ = "";

    string error_code = "";
    string error_message = "";

    protected:
    IotDevice *device;
    Sensor *sensor;

    string topic;

public:
  Node(IotDevice *_device, string _topic);
  void publish(string message,mqtt::QoS qos, bool retain);
  void subscribe();
  void setTopic(string _topic);
  string getTopic();
  virtual void read();
  virtual void cMessageReceivedCallback(const string &topicStr, const string &message) override;
};
