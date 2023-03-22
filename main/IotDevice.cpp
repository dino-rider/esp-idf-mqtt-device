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
  printf("Connected\n");
  client.upublish(mainTopic+"$name", device_name, idf::mqtt::QoS::AtLeastOnce, true);
  client.upublish(mainTopic+"$fw/name", fw_name, idf::mqtt::QoS::AtLeastOnce, true);
  client.upublish(mainTopic+"$fw/version", fw_version, idf::mqtt::QoS::AtLeastOnce, true);
  // client.upublish(main_topic+'$localip', "192.168.1.52", idf::mqtt::QoS::AtLeastOnce, true);
  client.upublish(mainTopic+"$implementation", implementation, idf::mqtt::QoS::AtLeastOnce, true);
  client.upublish(mainTopic+"$mac", mac_adress, idf::mqtt::QoS::AtLeastOnce, true);
  client.upublish(mainTopic+"$state", "ready", idf::mqtt::QoS::AtLeastOnce, true);
  client.upublish(mainTopic+"$heartbeat", "ping", idf::mqtt::QoS::AtLeastOnce, false);

  for (Node* node: nodes)
  {
    node->init();
  }
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
  printf("ping broker and reading input\n");
  client.upublish(mainTopic+"$heartbeat", "ping", idf::mqtt::QoS::AtLeastOnce, false);
  for (Node* node: nodes)
  {
    node->read();
  }
}
