#ifndef MOTOR_TASK_H
#define MOTOR_TASK_H

#include <Arduino.h>

#define IN1 GPIO_NUM_32
#define IN2 GPIO_NUM_25
#define DUTY_CYCLE 80                           // Duty cycle percentage for the motor (0-100)

class Motor
{
public:
    Motor() = default;
    ~Motor() = default;

    bool Initialize(gpio_num_t pwmPin, uint16_t pwmFreq = 1000, uint8_t resolutionBits = 8);
    bool SetSpeed(uint8_t dutyCycle); // Returns false if not initialized or invalid duty
    void StartRunMotor();             // To be used inside FreeRTOS task
private:
    gpio_num_t pwmPin = GPIO_NUM_NC;
    uint16_t pwmFreq = 1000;
    uint8_t resolutionBits = 8;
    uint8_t currentDutyCycle = 0;
    bool initialized = false;

    uint8_t pwmChannel = 0; // 0~15 for LEDC on ESP32
};

extern Motor *motor; // Global motor instance

#endif
