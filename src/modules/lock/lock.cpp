#include "lock.h"
#define OK_SOUND 1000, 50, 2
#define FAIL_SOUND 2000, 15, 20

#include "esp32-hal-ledc.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"

static TaskHandle_t _lock_task = NULL;
static QueueHandle_t _lock_queue = NULL;

typedef struct
{
  uint8_t pin;
  uint32_t duration;
} lock_msg_t;

static void lock_task(void *)
{
  lock_msg_t lock_msg;
  while (1)
  {
    lock_msg.pin = MAX_INPUT;
    xQueueReceive(_lock_queue, &lock_msg, portMAX_DELAY);
    digitalWrite(lock_msg.pin, HIGH);
    alert_open();
    delay(lock_msg.duration);
    digitalWrite(lock_msg.pin, LOW);
    alert_close();
  } // infinite loop
}

void sound(uint frequency, uint32_t interval = 500, uint times = 1)
{
  // ledcAttachPin(BUZZER_PIN, 0);
  // ledcWriteTone(_channel, tone_msg.frequency);

  // if (tone_msg.duration)
  // {
  //   delay(tone_msg.duration);
  //   ledcDetachPin(tone_msg.pin);
  //   ledcWriteTone(_channel, 0);
  // }
  noTone(BUZZER_PIN);
  for (uint i = 0; i < times; i++)
  {
    tone(BUZZER_PIN, frequency, interval);
    tone(BUZZER_PIN, 0, interval);
  }
}

void setup_lock()
{
  pinMode(LOCKER_PIN, OUTPUT);
  pinMode(34, OUTPUT);
  pinMode(32, OUTPUT);
  if (_lock_queue == NULL)
  {
    _lock_queue = xQueueCreate(128, sizeof(lock_msg_t));
    if (_lock_queue == NULL)
    {
      log_e("Could not create lock queue");
      return; // ERR
    }
  }

  if (_lock_task == NULL)
  {
    log_v("Creating lock task");
    xTaskCreate(
        lock_task,  // Function to implement the task
        "lock",     // Name of the task
        3500,       // Stack size in words
        NULL,       // Task input parameter
        1,          // Priority of the task
        &_lock_task // Task handle.
    );
    if (_lock_task == NULL)
    {
      log_e("Could not create lock task");
      return; // ERR
    }
    log_v("Lock task created");
  }
  sound(OK_SOUND);
}

void open_lock(uint32_t duration)
{
  log_d("Open lock, duration=%lu ms", duration);
  if (_lock_task == NULL || _lock_queue == NULL)
    return (void)log_e("Lock module not started");

  lock_msg_t lock_msg = {
      .pin = LOCKER_PIN,
      .duration = duration,
  };
  xQueueSend(_lock_queue, &lock_msg, portMAX_DELAY);
}

void alert_open()
{
  sound(OK_SOUND);
}
void alert_close()
{
  sound(OK_SOUND);
}
void alert_denied() {}
void alert_allowed() {}
