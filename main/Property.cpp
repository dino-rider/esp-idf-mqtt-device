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
  publish(topic + "/$name", name, idf::mqtt::QoS::AtLeastOnce, true);
  publish(topic + "/$settable", settable?"true":"false", idf::mqtt::QoS::AtLeastOnce, true);
  publish(topic + "/$retained", retained?"true":"false", idf::mqtt::QoS::AtLeastOnce, true);
  publish(topic + "/$datatype", data_type, idf::mqtt::QoS::AtLeastOnce, true);
  publish(topic + "/$format", format, idf::mqtt::QoS::AtLeastOnce, true);
  publish(topic + "/$unit", unit, idf::mqtt::QoS::AtLeastOnce, true);
  publish(topic, value, idf::mqtt::QoS::AtLeastOnce, true);


  if (settable)
  {
    printf ("subscribing to %s\n", (topic+"/set").c_str());
    subscribe();
  }
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
    topic = node->getTopic() + type_for_topic + _topic;
    return;
  }
  if (device != nullptr)
  {
    topic = device->getMainTopic() + type_for_topic + _topic;
    return;
  }
}

string Property::getTopic()
{
  return topic;
}
void Property::subscribe()
{
  if (node!=nullptr)
  {
    node->getDevice()->getClient()->usubscribe(topic+"/set", this, 1);
  }
  if (device!=nullptr)
  {
    device->getClient()->usubscribe(topic+"/set", this, 0);
  }
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

void Property::publishError(string message)
{
  if (node != nullptr)
  {
    string topic_ext = node->getName()+"/"+name;
    node->getDevice()->publishError(message, topic_ext);
    return;
  }
  if (device != nullptr)
  {
    string topic_ext = name;
    device->publishError(message, topic_ext);
    return;
  }
}

void Property::publishNotification( string message)
{
  if (node != nullptr)
  {
    node->getDevice()->publishNotification(message);
    return;
  }
  if (device != nullptr)
  {
    device->publishNotification(message);
    return;
  }
}

void Property::read()
{
  // read sensor values priodically if needed
}

void Property::cMessageReceivedCallback(const string &topicStr, const string &message)
{
  printf("%s set to: %s\n", name.c_str() ,message.c_str());
  // validate input
  // execute action on command
  // publish to topic to confirm setting attribute
}

bool Property::Validate(string value)
{
  // here goes your validation code
  // in case of error call publishError(<your errormessage>)
  return false;
}


//// Validation property

ButtonNotificationProperty::ButtonNotificationProperty(string _name, string _id, Node *_node, PROPERTY_TYPE _type, bool _settable, bool _retained,
           string _data_type, string _format, string _unit):Property(_name,_id, _node, _type, _settable, _retained, _data_type, _format, _unit)
{}

ButtonNotificationProperty::ButtonNotificationProperty(string _name, string _id, IotDevice *_device, PROPERTY_TYPE _type, bool _settable, bool _retained,
           string _data_type, string _format, string _unit):Property(_name,_id, _device, _type, _settable, _retained, _data_type, _format, _unit)
{}

void ButtonNotificationProperty::setFlag(bool *_flag)
{
  flag = _flag;
}

void ButtonNotificationProperty::onChange()
{
  state = !state;
  publish(topic, state?"true":"false", idf::mqtt::QoS::AtLeastOnce, true);
  publishNotification("Toggle button state: "+string{state?"ON":"OFF"});
}

void ButtonNotificationProperty::read()
{
  if (*flag)
  {
  (*flag) = false;
  onChange();
  }
}

//// Validation property

ValidationTestProperty::ValidationTestProperty(string _name, string _id, Node *_node, PROPERTY_TYPE _type, bool _settable, bool _retained,
           string _data_type, string _format, string _unit):Property(_name,_id, _node, _type, _settable, _retained, _data_type, _format, _unit)
{}

  // ----------------------------Constructor for Device`s properties
ValidationTestProperty::ValidationTestProperty(string _name, string _id, IotDevice *_device, PROPERTY_TYPE _type, bool _settable, bool _retained,
           string _data_type, string _format, string _unit):Property(_name,_id, _device, _type, _settable, _retained, _data_type, _format, _unit)
{}


bool ValidationTestProperty::Validate(string value)
{
  printf("Validate integer: %s \n", value.c_str());
  for (int i = 0; i < value.length(); i++)
  {
    if (isdigit(value[i]) == false)
    {
      publishError("Not integer");
      return false;
    }
  }
  if (std::stoi( value ) > 3)
    {
      publishError("Value must be less than 3");
      return false;
    }
  if (std::stoi( value ) < 0)
    {
      publishError("Value must be greater than 0");
      return false;
    }
  return true;
}

void ValidationTestProperty::cMessageReceivedCallback(const string &topicStr, const string &message)
{
    if (Validate(message))
    {
      publish(topic, message, idf::mqtt::QoS::AtLeastOnce, true);
    }
}

