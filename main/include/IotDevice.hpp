#pragma once
#include <vector>
#include <string>
#include "MqttClient.hpp"
#include "Node.hpp"

constexpr auto *TAG = "MQTT_DEVICE";

using namespace idf;
using namespace std;

class MyClient;
// calling Nodes Sensors here !
class Node;

// class Output;
// class LedOutput;

class IotDevice: public MqttCaller
{
private:
  string mainTopic = std::string{CONFIG_MQTT_USERNAME}+"/"+std::string{CONFIG_MQTT_DEVICE_ID};
  string errorTopic = std::string{CONFIG_MQTT_USERNAME}+"/errors";
  string notificationTopic = std::string{CONFIG_MQTT_USERNAME}+"/notifications";
  MyClient &client;
  std::vector< Node* > nodes;
  // set up peripherials

public:
  IotDevice(MyClient &_client);
  MyClient* getClient(){return &client;};
  void addNode(Node *_sensor);
  void cOnConnectCallback();
  void cMessageReceivedCallback(const string &topicStr, const string &message);
  void publish(string topic, string message, idf::mqtt::QoS qos, bool retain);
  void publishError(string message);
  void publishNotification(string message);
  void process();
  int  getStatus();
  string getMainTopic(){return mainTopic;};
};
