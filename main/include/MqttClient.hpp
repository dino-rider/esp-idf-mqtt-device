#pragma once
// #include <cstdlib>
#include <map>
#include <string>
#include "esp_mqtt.hpp"
#include "esp_mqtt_client_config.hpp"

using namespace std;

namespace mqtt = idf::mqtt;

extern bool connected_flag;

class MyClient;

// Caller interface to inherit and call back object methods
class MqttCaller
{
public:
  virtual void cOnConnectCallback(){};
  virtual void cMessageReceivedCallback(const string &topicStr, const string &message){printf("caller: %s", message.c_str());};
};

class MyClient final : public mqtt::Client
{
public:
  using mqtt::Client::Client;
  void setDevice(MqttCaller *_device){ device = _device; };
  void usubscribe(const string topicStr, MqttCaller* caller, int qos);
  void upublish(const string &topicStr,  const string &message, mqtt::QoS qos, bool retain);
  // may need to add some way to clear subscriptions on disconnect (!connected_flag)
  // on reconecct every device resubscribes to their topic
  void clean_subscriptions(){};

private:
  MqttCaller *device;
  std::map <string, MqttCaller*> topic_callbacks;
  void on_connected(esp_mqtt_event_handle_t const event) override;
  void on_subscribed(esp_mqtt_event_handle_t const event){};
  void on_data(esp_mqtt_event_handle_t const event) override;
  void on_disconnected(esp_mqtt_event_handle_t const event) override;
};