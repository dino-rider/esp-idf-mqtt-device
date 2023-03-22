#include "nvs_flash.h"
#include "protocol_examples_common.h"
#include "esp_log.h"
#include "IotDevice.hpp"

bool connected_flag = false;

// onboard LED
GPIO_Output gpio(GPIONum(2));

// Blink LED when no connection
// works on disconnect too
void Blink_led(void *arg)
{
    for(;;)
    {
    while (!connected_flag)
    {
        gpio.set_high();
        vTaskDelay(500/ portTICK_RATE_MS);
        gpio.set_low();
        vTaskDelay(500/ portTICK_RATE_MS);
    }
    while (connected_flag)
    {
    vTaskDelay(500/ portTICK_RATE_MS);
    }
    }
    vTaskDelete(NULL);
}

void Read_sensors(void *device)
{
    for(;;)
    {
        static_cast<IotDevice*>(device)->process();
        vTaskDelay(3000/ portTICK_RATE_MS);
    }
    vTaskDelete(NULL);
}

TaskHandle_t ReadTaskHandle = NULL;
TaskHandle_t BlinkTaskHandle = NULL;

extern "C" void app_main(void)
{
  ESP_LOGI(TAG, "[APP] Startup..");
  ESP_LOGI(TAG, "[APP] Free memory: %d bytes", esp_get_free_heap_size());
  ESP_LOGI(TAG, "[APP] IDF version: %s", esp_get_idf_version());

  esp_log_level_set("*", ESP_LOG_INFO);
  esp_log_level_set("MQTT_CLIENT", ESP_LOG_VERBOSE);
  esp_log_level_set("MQTT_EXAMPLE", ESP_LOG_ERROR);
  esp_log_level_set("TRANSPORT_TCP", ESP_LOG_ERROR);
  esp_log_level_set("TRANSPORT_SSL", ESP_LOG_ERROR);
  esp_log_level_set("TRANSPORT", ESP_LOG_ERROR);
  esp_log_level_set("OUTBOX", ESP_LOG_VERBOSE);

  ESP_ERROR_CHECK(nvs_flash_init());
  ESP_ERROR_CHECK(esp_netif_init());
  ESP_ERROR_CHECK(esp_event_loop_create_default());

  xTaskCreatePinnedToCore(Blink_led, "Blink_led", 4096, NULL, 10, &BlinkTaskHandle, 1);

  // printf("LastWill topic: %s, message is lost length is %d \n",(std::string{CONFIG_MQTT_MAIN_TOPIC}+std::string{"/status"}).c_str(), strlen("lost"));
  std::string lastwilltopic = (std::string{CONFIG_MQTT_USERNAME}+"/"+std::string{CONFIG_MQTT_DEVICE_ID}+"/$state");
  mqtt::BrokerConfiguration broker{
      .address = {mqtt::URI{std::string{CONFIG_MQTT_BROKER_URL}},
                .port = CONFIG_MQTT_BROKER_PORT},
      .security = mqtt::Insecure{}
      };
  mqtt::ClientCredentials credentials{
    .client_id = std::string{CONFIG_MQTT_USERNAME};
    .username = std::string{CONFIG_MQTT_USERNAME};
    .authentication = mqtt::Password{std::string{CONFIG_MQTT_PASSWORD};}
    };
  mqtt::Configuration config{};
  mqtt::Session session{};
  mqtt::LastWill lastwill{
    .lwt_topic = lastwilltopic,
    .lwt_msg = "lost",
    .lwt_qos = 1,
    .lwt_retain = 1,
    .lwt_msg_len = 4,
  };
  session.last_will = lastwill;
  session.keepalive = 120;
  session.disable_clean_session = 0;
  config.session = session;

  MyClient client{broker, credentials, config};
  IotDevice device{client};
  // LedOutput onboardLed{&device, "light_0", 2};
  // WifiStrengthSensor wifiSignal{&device, "wifisignal"};
  // device.addOutput(&onboardLed);
  // device.addSensor(&wifiSignal);
  client.setDevice(&device);

  ESP_ERROR_CHECK(example_connect());
  xTaskCreatePinnedToCore(Read_sensors, "Read_sensors", 4096, &device, 15, &ReadTaskHandle, 0);


  while (true)
  {
    constexpr TickType_t xDelay = 500 / portTICK_PERIOD_MS;
    vTaskDelay(xDelay);
  }
}