#include "IotDevice.hpp"

IotDevice::IotDevice(MyClient &_client): client(_client)
{
}


void IotDevice::addNode(Node *_node)
{
  nodes.push_back(_node);
}

void IotDevice::cOnConnectCallback()
{
  printf("Connected");  
  for (Node* node: nodes)
  {
    node->init();
  }
  client.upublish(std::string{CONFIG_MQTT_MAIN_TOPIC}+"/status", "ready", idf::mqtt::QoS::AtLeastOnce, true);
}

void IotDevice::cMessageReceivedCallback(const string &topicStr, const string &message)
{
}

void IotDevice::publish(string topic, string message, idf::mqtt::QoS qos, bool retain)
{
  client.upublish(topic, message, qos, retain);
}

void IotDevice::publishError(string topic, string message, idf::mqtt::QoS qos, bool retain)
{
  client.upublish(topic, message, qos, retain);
}

void IotDevice::publishNotification(string topic, string message, idf::mqtt::QoS qos, bool retain)
{
  client.upublish(topic, message, qos, retain);
}


void IotDevice::process()
{
  // printf("reading input\n");
  for (Node* node: nodes)
  {
    node->read();
  }
}
