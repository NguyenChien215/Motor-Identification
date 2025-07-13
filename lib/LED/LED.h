#ifndef LED_H
#define LED_H

#include <Arduino.h>

class LED
{
public:
    LED() = default;  // Constructor
    ~LED() = default; // Destructor

    void Initialize(gpio_num_t gpio, uint32_t delay_ms);
    void StartBlinkLED();

private:
    gpio_num_t gpioPin = GPIO_NUM_NC; // GPIO pin number, default to not connected
    uint32_t blinkDelayMs = 500;      // Delay in milliseconds for blinking
    bool initialized = false;         // Flag to check if the LED task is initialized
};

extern LED *led; // Global LED instance

#endif
