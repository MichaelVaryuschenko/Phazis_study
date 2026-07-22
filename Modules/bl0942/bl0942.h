#include "main.h"
#include "bl0942_regs.h"

/*#define INT24_SIGN_MASK 0xFF800000
#define INT20_SIGN_MASK 0xFFF80000
#define INT32_SIGN_MASK 0x80000000*/

void bl_init(void);
void bl_start_reading_regs(void);
void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef *hspi);
static int32_t int24_to_int32(uint32_t int24_v);
static int32_t int20_to_int32(uint32_t int20_v);
void bl_process_data(void);