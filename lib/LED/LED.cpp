#include "LED.h"
#include "esp_log.h"

static const char *LED_TAG = "LED";

void LED::Initialize(gpio_num_t gpio, uint32_t delay_ms)
{
    ESP_LOGI(LED_TAG, "Initializing LED task...");

    gpioPin = gpio;
    blinkDelayMs = delay_ms;
    initialized = true;

    ESP_LOGI(LED_TAG, "Finish Initializing LED task on GPIO %d with delay %d ms", gpioPin, blinkDelayMs);
}

void LED::StartBlinkLED()
{
    if (!initialized)
    {
        ESP_LOGE(LED_TAG, "LED task not initialized!");
        vTaskDelete(NULL); // Delete the task if not initialized
        return;
    }

    pinMode(gpioPin, OUTPUT);
    bool ledState = false;

    while (1)
    {
        digitalWrite(gpioPin, ledState);
        ESP_LOGV(LED_TAG, "LED %d is %s", gpioPin, ledState ? "ON" : "OFF");
        ledState = !ledState;
        vTaskDelay(pdMS_TO_TICKS(blinkDelayMs));
    }
}

LED *led; // Global instance for task access
