#include <Arduino.h>
#include <LED.h>
#include <Motor.h>
#include <Encoder.h>
#include "esp_log.h"

#define LED_PIN GPIO_NUM_2
#define PWM_PIN GPIO_NUM_33       // PWM pin for motor control
#define ENCODER_A_PIN GPIO_NUM_27 // Encoder A pin
#define ENCODER_B_PIN GPIO_NUM_26 // Encoder B pin
#define LED_BLINK_TIME_MS 500
#define PPR_ENCODER 217
#define GEAR_RATIO 10
#define PPR_OF_MOTOR (PPR_ENCODER * GEAR_RATIO) // Pulses per revolution for the motor encoder

static const char *TAG = "MAIN";
void led_task_runner(void *param);
void motor_task_runner(void *param);
void encoder_task_runner(void *param);

void setup()
{
  esp_log_level_set("LED", ESP_LOG_INFO);
  led = new LED();         // Create a new instance of LED
  motor = new Motor();     // Create a new instance of Motor
  encoder = new Encoder(); // Create a new instance of Encoder

  // Create task for LED blinking

  led->Initialize(LED_PIN, LED_BLINK_TIME_MS); // Config pin and delay blink in ms

  xTaskCreate(
      led_task_runner,
      "LedBlinkTask",
      2048,
      led,
      1,
      nullptr);

  // // Create task for motor control
  if (!motor->Initialize(PWM_PIN, 1000, 8)) // Frequency 1 KHz
  {
    ESP_LOGV(TAG, "Failed to init motor");
    return;
  }

  // Set duty cycle to 80% (204 out of 255 for 8-bit resolution)
  motor->SetSpeed(DUTY_CYCLE);
  ESP_LOGI(TAG, "Set duty cycle to 128 (50%)");

  xTaskCreate(
      motor_task_runner,
      "MotorTask",
      2048,
      motor,
      1,
      nullptr);

  // Initialize the encoder and create its task
  encoder->Initialize(ENCODER_A_PIN, ENCODER_B_PIN, PPR_OF_MOTOR); // A/B pins, PPR
  xTaskCreate(
      encoder_task_runner,
      "EncoderTask",
      2048,
      encoder,
      1,
      nullptr);
}

void loop()
{
  // Do nothing
  vTaskDelay(1000 / portTICK_PERIOD_MS);
}

void led_task_runner(void *param)
{
  LED *led_ptr = static_cast<LED *>(param);
  led_ptr->StartBlinkLED();
}

void motor_task_runner(void *param)
{
  Motor *motor_ptr = static_cast<Motor *>(param);
  motor_ptr->StartRunMotor();
}

void encoder_task_runner(void *param)
{
  Encoder *encoder_ptr = static_cast<Encoder *>(param);
  encoder_ptr->StartRunEncoder();
}