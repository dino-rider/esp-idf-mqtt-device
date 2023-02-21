#include "nvs_flash.h"
#include "protocol_examples_common.h"
#include "esp_log.h"
#include "IotDevice.hpp"
#include "config.hpp"

// constexpr auto *TAG = "MQTT_DEVICE";
bool connected_flag = false;

extern "C" void app_main(void)
{
  ESP_LOGI(TAG, "[APP] Startup..");
  ESP_LOGI(TAG, "[APP] Free memory: %d bytes", esp_get_free_heap_size());
  ESP_LOGI(TAG, "[APP] IDF version: %s", esp_get_idf_version());

  esp_log_level_set("*", ESP_LOG_INFO);
  esp_log_level_set("MQTT_CLIENT", ESP_LOG_VERBOSE);
  esp_log_level_set("MQTT_EXAMPLE", ESP_LOG_VERBOSE);
  esp_log_level_set("TRANSPORT_TCP", ESP_LOG_VERBOSE);
  esp_log_level_set("TRANSPORT_SSL", ESP_LOG_VERBOSE);
  esp_log_level_set("TRANSPORT", ESP_LOG_VERBOSE);
  esp_log_level_set("OUTBOX", ESP_LOG_VERBOSE);

  ESP_ERROR_CHECK(nvs_flash_init());
  ESP_ERROR_CHECK(esp_netif_init());
  ESP_ERROR_CHECK(esp_event_loop_create_default());

  // xTaskCreatePinnedToCore(Demo_Task, "Demo_Task", 4096, NULL, 10, &myTaskHandle, 1);

  ESP_ERROR_CHECK(example_connect());

  mqtt::BrokerConfiguration broker{
      .address = {mqtt::URI{std::string{CONFIG_BROKER_URL}}},
      .security = mqtt::Insecure{}};
  mqtt::ClientCredentials credentials{};
  mqtt::Configuration config{};

  MyClient client{broker, credentials, config};
  IotDevice device{client};

  while (true)
  {
    device.process();
    constexpr TickType_t xDelay = 500 / portTICK_PERIOD_MS;
    vTaskDelay(xDelay);
  }
}