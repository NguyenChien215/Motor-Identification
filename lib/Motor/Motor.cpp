#include "Motor.h"
#include "esp_log.h"

static const char *MOTOR_TAG = "MOTOR";

bool Motor::Initialize(gpio_num_t pwmPin, uint16_t pwmFreq, uint8_t resolutionBits)
{
    if (pwmPin == GPIO_NUM_NC)
    {
        ESP_LOGE(MOTOR_TAG, "Invalid PWM GPIO pin");
        return false;
    }

    if (resolutionBits > 16)
    {
        ESP_LOGE(MOTOR_TAG, "PWM resolution too high: %d bits", resolutionBits);
        return false;
    }

    this->pwmPin = pwmPin;
    this->pwmFreq = pwmFreq;
    this->resolutionBits = resolutionBits;

    if (!ledcSetup(pwmChannel, pwmFreq, resolutionBits))
    {
        ESP_LOGE(MOTOR_TAG, "Failed to setup LEDC on channel %d", pwmChannel);
        return false;
    }

    ledcAttachPin(pwmPin, pwmChannel);
    initialized = true;

    ESP_LOGI(MOTOR_TAG, "Motor initialized on GPIO %d (Freq: %d Hz, Resolution: %d-bit)",
             pwmPin, pwmFreq, resolutionBits);

    return true;
}

bool Motor::SetSpeed(uint8_t dutyCycle)
{
    if (!initialized)
    {
        ESP_LOGE(MOTOR_TAG, "Motor not initialized");
        return false;
    }

    dutyCycle = DUTY_CYCLE * 255 / 100;
    uint32_t maxDuty = (1 << resolutionBits) - 1;
    if (dutyCycle > maxDuty)
    {
        ESP_LOGW(MOTOR_TAG, "Duty cycle %d exceeds max (%lu)", dutyCycle, maxDuty);
        dutyCycle = maxDuty;
    }

    currentDutyCycle = dutyCycle;
    ledcWrite(pwmChannel, currentDutyCycle);

    ESP_LOGI(MOTOR_TAG, "Motor speed set to duty cycle: %d", currentDutyCycle);
    return true;
}

void Motor::StartRunMotor()
{
    if (!initialized)
    {
        ESP_LOGE(MOTOR_TAG, "Cannot start motor task - not initialized");
        vTaskDelete(NULL);
        return;
    }

    // Config pin for set motor direction
    ESP_LOGI(MOTOR_TAG, "Start config pins for motor direction");
    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW); 
    ESP_LOGI(MOTOR_TAG, "Finish config pins for motor direction");

    while (1)
    {
        // Do nothing here
        vTaskDelay(pdMS_TO_TICKS(1000)); // Loop every 1000ms
    }
}

Motor *motor; // Global instance for task access
