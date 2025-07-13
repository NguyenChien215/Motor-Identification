#ifndef ENCODER_H
#define ENCODER_H

#include <Arduino.h>

class Encoder {
public:
    Encoder() = default;
    ~Encoder() = default;

    bool Initialize(gpio_num_t pinA, gpio_num_t pinB, uint16_t pulsesPerRevolution);
    void StartRunEncoder();
    float GetVelocityRPM();

private:
    gpio_num_t encoderPinA = GPIO_NUM_NC;
    gpio_num_t encoderPinB = GPIO_NUM_NC;
    volatile int32_t pulseCount = 0;
    int32_t lastPulseCount = 0;

    float velocityRpm = 0.0f;
    uint32_t lastUpdateTimeMs = 0;
    uint16_t ppr = 1;
    bool initialized = false;

    static void IRAM_ATTR isrA();
    static void IRAM_ATTR isrB();
};

extern Encoder* encoder;

#endif // ENCODER_H
