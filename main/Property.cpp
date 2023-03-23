#include "Property.hpp"

Property::Property(string _name, string _id, Node *_node, PROPERTY_TYPE _type, bool _settable, bool _retained,
                   string _data_type, string _format, string _unit)
{
  name = _name;
  id =_id;
  type = _type;
  settable = _settable;
  retained = _retained;
  data_type = _data_type;
  format = _format;
  unit = _unit;
  node = _node;
  setTopic(name);
  node->addProperty(this);
}

Property::Property(string _name, string _id, IotDevice *_device, PROPERTY_TYPE _type, bool _settable, bool _retained,
                   string _data_type, string _format, string _unit)
{
  name = _name;
  id =_id;
  type = _type;
  settable = _settable;
  retained = _retained;
  data_type = _data_type;
  format = _format;
  unit = _unit;
  device = _device;
  setTopic(name);
  printf("Property topic for %s : %s\n", name.c_str(), topic.c_str());
  device->addProperty(this);
}

bool Property::init()
{
  printf("calling init property\n");
  // device->getClient()->usubscribe(topic, static_cast<MqttCaller*>(this), 0);
  // client->usubscribe(topic, this, 0);
  publish(topic + "$name", name, idf::mqtt::QoS::AtLeastOnce, true);
  publish(topic + "$settable", settable?"true":"false", idf::mqtt::QoS::AtLeastOnce, true);
  publish(topic + "$retained", retained?"true":"false", idf::mqtt::QoS::AtLeastOnce, true);
  publish(topic + "$datatype", "boolean", idf::mqtt::QoS::AtLeastOnce, true); // iterate over properties to get their list
  // subscribe();
  return true;

}

void Property::setTopic(string _topic)
{
  string type_for_topic = "";

  switch (type)
  {
  case OPTION:
    type_for_topic = "$options/";
    break;

  case TELEMETRY:
    type_for_topic = "$telemetry/";
    break;

  case SENSOR:
    type_for_topic = "";
    break;

  default:
    type_for_topic = "";
    break;

  }
  if (node != nullptr)
  {
    topic = node->getTopic() + _topic + "/";
    return;
  }
  if (device != nullptr)
  {
    topic = device->getMainTopic() + _topic + "/";
    return;
  }
}

string Property::getTopic()
{
  return topic;
}
void Property::subscribe()
{
  // if (node!=NULL)
  // {
  //   topic = node->subscribe() + _topic + "/";
  //   return;
  // }
  // if (device!=NULL)
  // {
  //   topic = device->getMainTopic() + _topic + "/";
  //   return;
  // }
}

void Property::publish(string _topic, string message, mqtt::QoS qos, bool retain)
{
  if (node != nullptr)
  {
    node->getDevice()->publish(_topic, message, qos, retain);
    return;
  }
  if (device != nullptr)
  {
    device->publish(_topic, message, qos, retain);
    return;
  }
}

void Property::read()
{
}

void Property::cMessageReceivedCallback(const string &topicStr, const string &message)
{
}

bool Property::Validate(string value)
{
  // printf("Validate inreger: %s \n", value.c_str());

  // for (int i = 0; i < value.length(); i++)
  // {
  //   if (isdigit(value[i]) == false)
  //   {
  //     // SetError(E_C_WRONG_FORMAT, E_M_WRONG_FORMAT);
  //     return false;
  //   }
  // }
  // if ((int)value > 3)
  //   {
  //     // SetError(E_C_WRONG_FORMAT, E_M_INVALID_INTERVAL);
  //     return false;
  //   }
  return true;
}
