#include "main.h"
#include <stdbool.h>

#define PWM_MAX                 65535U      /**< 16-бит разрешение ШИМ */

void led_init(void);

void led_set_color(float new_cl_red, float new_cl_green, float new_cl_blue);
//turn LED on\off
void led_set_mode(bool new_mode);

//update hardware preferences (PWM fill coefficient) according to current LED state
void led_handler(void);

//pushes LED through default work routine; supposed to be called every 10..100 ms
void led_default_control(bool mode);