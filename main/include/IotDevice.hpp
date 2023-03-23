#pragma once
#include <vector>
#include <string>
#include "MqttClient.hpp"
#include "Node.hpp"
#include "Property.hpp"

constexpr auto *TAG = "MQTT_DEVICE";

using namespace idf;
using namespace std;

class MyClient;
class Node;
class Property;

class IotDevice: public MqttCaller
{
private:
  string mainTopic = std::string{CONFIG_MQTT_USERNAME}+"/sweet-home/"+std::string{CONFIG_MQTT_DEVICE_ID}+"/";
  string errorTopic = std::string{CONFIG_MQTT_USERNAME}+"/errors";
  string notificationTopic = std::string{CONFIG_MQTT_USERNAME}+"/notifications";
  string getPropertyString(const std::vector <Property*> &props);
  MyClient &client;
  std::vector< Node* > nodes;
  std::vector< Property* > options;
  std::vector< Property* > properties;
  std::vector< Property* > telemetry;

public:
  string device_name = "Test device";
  string fw_name = "1678446122462726";
  string fw_version = "1";
  string implementation = "Test device";
  string mac_adress = "30-b8-3d-01-f6-7f";
  IotDevice(MyClient &_client);
  MyClient* getClient(){return &client;};
  void addNode(Node *_node);
  void addProperty(Property *_property);
  void cOnConnectCallback();
  void cMessageReceivedCallback(const string &topicStr, const string &message);
  void publish(string topic, string message, idf::mqtt::QoS qos, bool retain);
  void publishError(string message);
  void publishNotification(string message);
  void process();
  int  getStatus();
  string getMainTopic(){return mainTopic;};
};
