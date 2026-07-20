#include "button.h"

//2nd and 4th states are implemented to prevent reaction on button bouncing
typedef enum {
  BUTTON_STATE_RELEASED=0,
  BUTTON_STATE_PRESSED_RECENTLY,
  BUTTON_STATE_PRESSED,
  BUTTON_STATE_RELEASED_RECENTLY
} button_state_t;

typedef struct {
  button_state_t state;
  uint8_t debounce_counter;
} button_handle_t;

static button_handle_t button;
/*
 * true for LED glimmering between blue and red
 * false for LED glimmering between green and red
 */
extern bool led_glow_mode;

void button_init(void) {
  button.state = BUTTON_STATE_RELEASED;
  button.debounce_counter = 0;
}

static inline bool button_is_pressed(void) {
    return HAL_GPIO_ReadPin(BUTTON_GPIO_Port, BUTTON_Pin) == GPIO_PIN_RESET;
}

void button_handler(void) {
  switch (button.state) {
    case BUTTON_STATE_RELEASED:
      if (button_is_pressed())
        button.state = BUTTON_STATE_PRESSED_RECENTLY;
      break;
    case BUTTON_STATE_PRESSED_RECENTLY:
      button.debounce_counter++;
      if(button.debounce_counter == 20) {
        if(button_is_pressed()) {
          button.state = BUTTON_STATE_PRESSED;
          led_glow_mode = !led_glow_mode;
        }
        else {
          button.state = BUTTON_STATE_RELEASED;
        }
        button.debounce_counter = 0;
      }
      break;
    case BUTTON_STATE_PRESSED:
      if (!button_is_pressed())
        button.state = BUTTON_STATE_RELEASED_RECENTLY;
      break;
    case BUTTON_STATE_RELEASED_RECENTLY:
      button.debounce_counter++;
      if(button.debounce_counter == 20) {
        if(!button_is_pressed()) {
          button.state = BUTTON_STATE_RELEASED;
        }
        else {
          button.state = BUTTON_STATE_PRESSED;
        }
        button.debounce_counter = 0;
      }
      break;
  }
}