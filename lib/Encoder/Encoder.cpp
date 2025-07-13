#include "Encoder.h"
#include "Motor.h"
#include "esp_log.h"

static const char *ENCODER_TAG = "ENCODER_TASK";

bool Encoder::Initialize(gpio_num_t pinA, gpio_num_t pinB, uint16_t pulsesPerRevolution)
{
    if (pinA == GPIO_NUM_NC || pinB == GPIO_NUM_NC)
    {
        ESP_LOGE(ENCODER_TAG, "Invalid encoder pins");
        return false;
    }

    encoderPinA = pinA;
    encoderPinB = pinB;
    ppr = 2*pulsesPerRevolution;

    pinMode(encoderPinA, INPUT_PULLUP);
    pinMode(encoderPinB, INPUT_PULLUP);

    attachInterrupt(digitalPinToInterrupt(encoderPinA), isrA, RISING);
    attachInterrupt(digitalPinToInterrupt(encoderPinB), isrB, RISING);

    lastUpdateTimeMs = millis();
    initialized = true;

    ESP_LOGI(ENCODER_TAG, "Encoder initialized on pins A=%d B=%d, PPR=%u", pinA, pinB, ppr);
    return true;
}

void IRAM_ATTR Encoder::isrA()
{
    if (encoder == nullptr)
        return;
        
    bool signalA = digitalRead(encoder->encoderPinA);
    bool signalB = digitalRead(encoder->encoderPinA);

    if (signalA == signalB)
    {
        encoder->pulseCount++;
    }
    else
    {
        encoder->pulseCount--;
    }
}

void IRAM_ATTR Encoder::isrB()
{
    if (encoder == nullptr)
        return;

    bool signalA = digitalRead(encoder->encoderPinA);
    bool signalB = digitalRead(encoder->encoderPinB);

    if (signalA != signalB)
    {
        encoder->pulseCount++;
    }
    else
    {
        encoder->pulseCount--;
    }
}

float Encoder::GetVelocityRPM()
{
    return velocityRpm;
}

void Encoder::StartRunEncoder()
{
    uint32_t sample = 0;
    if (!initialized)
    {
        ESP_LOGE(ENCODER_TAG, "Encoder not initialized");
        vTaskDelete(NULL);
        return;
    }

    while (true)
    {
        uint32_t currentTimeMs = millis();
        uint32_t elapsedTimeMs = currentTimeMs - lastUpdateTimeMs;

        if (elapsedTimeMs >= 15)
        {
            sample++;
            int32_t currentPulseCount = pulseCount;
            int32_t pulseDelta = currentPulseCount - lastPulseCount;

            float revolutions = static_cast<float>(pulseDelta) / static_cast<float>(ppr);
            velocityRpm = (revolutions * 60000.0f) / static_cast<float>(elapsedTimeMs);

            lastPulseCount = currentPulseCount;
            lastUpdateTimeMs = currentTimeMs;

            ESP_LOGI(ENCODER_TAG, "Sample %D, PWM: %d %, Motor Speed: %.2f RPM",sample, DUTY_CYCLE, velocityRpm);
        }

        vTaskDelay(pdMS_TO_TICKS(1));
    }
}

Encoder *encoder; // Global instance for task access
