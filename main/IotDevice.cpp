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
  client.upublish(mainTopic+"/ready", "ready", idf::mqtt::QoS::AtLeastOnce, true);
}

void IotDevice::cMessageReceivedCallback(const string &topicStr, const string &message)
{
}

void IotDevice::publish(string topic, string message, idf::mqtt::QoS qos, bool retain)
{
  client.upublish(topic, message, qos, retain);
}

void IotDevice::publishError(string message)
{
  client.upublish(errorTopic, message, idf::mqtt::QoS::AtLeastOnce, false);
}

void IotDevice::publishNotification(string message)
{
  client.upublish(notificationTopic, message, idf::mqtt::QoS::AtLeastOnce, false);
}


void IotDevice::process()
{
  // printf("reading input\n");
  for (Node* node: nodes)
  {
    node->read();
  }
}
