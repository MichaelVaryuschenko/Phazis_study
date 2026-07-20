#include "led.h"

typedef struct {
  bool on;
  float cl_red;
  float cl_green;
  float cl_blue;
} led_handle_t;

extern TIM_HandleTypeDef htim1;

static led_handle_t led;

void led_init(void) {
  led.on = true;
  led.cl_red = 1.0;
  led.cl_green  = 0.0;
  led.cl_blue = 0.0;
  HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_1);
  HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_2);
  HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_3);
  led_handler();
}

void led_set_color(float new_cl_red, float new_cl_green, float new_cl_blue) {
  led.cl_red = new_cl_red;
  led.cl_green = new_cl_green;
  led.cl_blue = new_cl_blue;
}

void led_set_mode(bool new_mode) {
  led.on = new_mode;
}

void led_handler(void) {
  if(led.on) {
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, (uint32_t)(led.cl_red * PWM_MAX));
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, (uint32_t)(led.cl_green * PWM_MAX));
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, (uint32_t)(led.cl_blue * PWM_MAX));
  }
  else {
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 0);
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, 0);
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, 0);
  }
}

void led_default_control(bool mode) {
  static float r = 1.0;
  static float g = 0.0;
  static float b = 0.0;
  static bool r_rising = false;
  if (r < 0.01)
    r_rising = true;
  if (r > 0.99)
    r_rising = false;
  if (r_rising) {
    if (mode) {
      b += g; //this line makes b equal to g if the glow mode has been changed just before the function call, and does nothing otherwise
      b -= 0.01;
      g = 0;
    }
    else {
      g += b;
      g -= 0.01;
      b = 0;
    }
    r += 0.01;
  }
  else {
    if (mode) {
      b += g;
      b += 0.01;
      g = 0;
    }
    else {
      g += b;
      g += 0.01;
      b = 0;
    }
    r -= 0.01;
  }
  led_set_color(r, g, b);
}