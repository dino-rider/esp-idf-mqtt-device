#include "IotDevice.hpp"

IotDevice::IotDevice(MyClient &_client): client(_client)
{
}


void IotDevice::addNode(Node *_node)
{
  nodes.push_back(_node);
}


void IotDevice::addProperty(Property *_property)
{
  switch (_property->getType())
  {
  case OPTION:
    options.push_back(_property);
    break;

  case TELEMETRY:
    telemetry.push_back(_property);
    break;

  case SENSOR:
    properties.push_back(_property);
    break;

  default:
    break;
  }
}

void IotDevice::cOnConnectCallback()
{
  printf("Connected\n");
  publish(mainTopic+"$name", device_name, idf::mqtt::QoS::AtLeastOnce, true);
  publish(mainTopic+"$fw/name", fw_name, idf::mqtt::QoS::AtLeastOnce, true);
  publish(mainTopic+"$fw/version", fw_version, idf::mqtt::QoS::AtLeastOnce, true);
  publish(mainTopic+"$localip", "192.168.1.52", idf::mqtt::QoS::AtLeastOnce, true);
  publish(mainTopic+"$implementation", implementation, idf::mqtt::QoS::AtLeastOnce, true);
  publish(mainTopic+"$mac", mac_adress, idf::mqtt::QoS::AtLeastOnce, true);
  publish(mainTopic+"$state", "ready", idf::mqtt::QoS::AtLeastOnce, true);
  publish(mainTopic+"$properties", getPropertyString(properties), idf::mqtt::QoS::AtLeastOnce, true); // iterate over properties to get their list
  publish(mainTopic+"$options", getPropertyString(options), idf::mqtt::QoS::AtLeastOnce, true); // iterate over properties to get their list
  publish(mainTopic+"$telemetry", getPropertyString(telemetry), idf::mqtt::QoS::AtLeastOnce, true); // iterate over properties to get their list
  
  if (!properties.empty())
    {
      for (Property* property: properties)
      {
        property->init();
      }
    }
  if (!options.empty())
    {
      for (Property* property: options)
      {
        property->init();
      }
    }
  if (!telemetry.empty())
    {
    for (Property* property: telemetry)
    {
      property->init();
    }
    }
  string nodes_string = "";
  for (Node* node: nodes)
  {
    if (nodes_string.size()>0)
    {
      nodes_string+=", ";
    }
    nodes_string += node->getName();
  }
  publish(mainTopic+"$nodes", nodes_string, idf::mqtt::QoS::AtLeastOnce, true);
  for (Node* node: nodes)
  {
    node->init();
  }
  publish(mainTopic+"$heartbeat", "ping", idf::mqtt::QoS::AtLeastOnce, false);
}

string IotDevice::getPropertyString(const std::vector <Property*> &props)
{
  string prop_string = "";
  if (!props.empty())
  {
  for (Property* property: props)
  {
    if (prop_string.size()>0)
    {
      prop_string+=", ";
    }
    prop_string += property->getName();
  }
  }
  return prop_string;
}

void IotDevice::cMessageReceivedCallback(const string &topicStr, const string &message)
{
}

void IotDevice::publish(string topic, string message, idf::mqtt::QoS qos, bool retain)
{
  client.upublish(topic, message, qos, retain);
}

void IotDevice::publishError(string message, string topic_ext)
{
    char str[300];
  sprintf(str,
"{\"fields\":{},\
\"code\":\"ERROR\",\
\"message\":\"%s\"}", message.c_str());
  string error_json = (str);
  publish(errorTopic+topic_ext, error_json, idf::mqtt::QoS::AtLeastOnce, false);
}

void IotDevice::publishNotification(string message)
{
  char str[300];
  sprintf(str,
"{\"type\":\"text\",\
\"senderType\":\"device\",\
\"senderId\":\"%s\",\
\"logLevel\":\"info\",\
\"message\":\"%s\"}", device_id.c_str(), message.c_str());
  string notification_json = (str);
  publish(notificationTopic, notification_json, idf::mqtt::QoS::AtLeastOnce, false);
}


void IotDevice::process()
{
  if (connected_flag)
  {
  for (Node* node: nodes)
  {
    node->read();
  }
  }
}

void IotDevice::heartbeat()
{
  if (connected_flag)
  {
  client.upublish(mainTopic+"$heartbeat", "ping", idf::mqtt::QoS::AtLeastOnce, false);
  }
}
