#pragma once

// #include <cstdlib>
#include <map>
#include <string>
#include "esp_mqtt.hpp"
#include "esp_mqtt_client_config.hpp"

using namespace std;

namespace mqtt = idf::mqtt;

extern bool connected_flag;

class MyClient; // forward declaration

// Caller interface to inherit and call back object methods

class MqttCaller
{
public:
  virtual void cVoidCallback(){};
  virtual void cMessageReceivedCallback(const string &topicStr, const string &message){};
};


class MyClient final : public mqtt::Client
{
public:
  using mqtt::Client::Client;
  void usubscribe(const string &topicStr, MqttCaller* caller, int qos);
  void upublish(const string &topicStr,  const string &message, bool retain);

private:
  std::map <std::string, MqttCaller*> topic_callbacks;
  void on_connected(esp_mqtt_event_handle_t const event) override;
  void on_data(esp_mqtt_event_handle_t const event) override;
  void on_disconnected(esp_mqtt_event_handle_t const event) override;
  mqtt::Filter messages{"$SYS/broker/messages/received"};
  mqtt::Filter sent_load{"$SYS/broker/load/+/sent"};
};