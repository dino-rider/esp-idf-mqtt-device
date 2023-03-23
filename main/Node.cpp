#include "Node.hpp"


Node::Node(IotDevice *_device, string _name, string _type)
{
  device =_device;
  name =_name;
  type = _type;
  setTopic(_name);
  device->addNode(this);
}

void Node::publish(string _topic,const string message, mqtt::QoS qos, bool retain)
{
 device->publish(_topic, message, qos, retain);
}

void Node::init()
{
  // device->getClient()->usubscribe(topic, static_cast<MqttCaller*>(this), 0);
  // client->usubscribe(topic, this, 0);

  publish(topic+"$name", name, idf::mqtt::QoS::AtLeastOnce, true);
  publish(topic+"$state", state, idf::mqtt::QoS::AtLeastOnce, true);
  publish(topic+"$type", type, idf::mqtt::QoS::AtLeastOnce, true);
  publish(topic+"$properties", getPropertyString(properties), idf::mqtt::QoS::AtLeastOnce, true); // iterate over properties to get their list
  publish(topic+"$options", getPropertyString(options), idf::mqtt::QoS::AtLeastOnce, true); // iterate over properties to get their list
  publish(topic+"$telemetry", getPropertyString(telemetry), idf::mqtt::QoS::AtLeastOnce, true); // iterate over properties to get their list
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
}

string Node::getPropertyString(const std::vector <Property*> &props)
{
  string prop_string = "";
  if (!props.empty())
  {
  for (Property* property: props)
  {
    prop_string += property->getName()+",";
  }
  }
  return prop_string;
}

void Node::addProperty(Property *_property)
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

// in the future we might want to store settings on flash
void Node::storeSettings()
{

}

// in the future we might want to read settings from flash
void Node::readSettings()
{

}


void Node::read()
{

}

void Node::setTopic(string _topic)
{
  topic = device->getMainTopic()+_topic+"/";
}

string Node::getTopic()
{
 return topic;
}

void Node::cMessageReceivedCallback(const string &topicStr, const string &message)
{

}

// WifiStrengthSensor::WifiStrengthSensor(IotDevice *_device, string _topic): Sensor(_device,_topic)
// {
//   printf("wifi signal topic: %s \n", topic.c_str());
// }

// int WifiStrengthSensor::clamp(int value, int min, int max)
// {
//   if (value<min) return min;
//   if (value>max) return max;
//   return value;
// }

// void WifiStrengthSensor::read()
// {
//   esp_wifi_sta_get_ap_info(&ap);
//   int reading = ap.rssi;
//   if (abs(value-reading)>5)
//   {
//     value = reading;
//     percent = clamp(2*(100 + value),0,100);
//     onChange();
//   }
// }

// void WifiStrengthSensor::onChange()
// {
//   publish(std::to_string(percent),mqtt::QoS::AtMostOnce ,true);
// }


// LedOutput::LedOutput(IotDevice *_device, string _topic, int _ledPin): Output(_device,_topic) ,ledPin(_ledPin), gpio_0(GPIO_Output(GPIONum(ledPin)))
// {
//   printf("onBoardLed topic: %s \n", topic.c_str());
// }

// void LedOutput::execute(string command)
// {
//     printf("LED command: %s \n", command.c_str());

//   if (command == "ON")
//   {
//     printf("Onboard LED ON\n");
//     gpio_0.set_high();
//     return;
//   }
//   if (command == "OFF")
//   {
//     printf("Onboard LED OFF\n");
//     gpio_0.set_low();
//     return;
//   }
// }