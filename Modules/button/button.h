#include "main.h"
#include <stdbool.h>

//initialize button handle structure
void button_init(void);

static inline bool button_is_pressed(void);

//switch button state if button is pressed (supposed to be called every 10 ms)
void button_handler(void);