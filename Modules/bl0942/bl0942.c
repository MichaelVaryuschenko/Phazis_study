#include "bl0942.h"

typedef struct {
  uint32_t I_WAVE_RAW; //should be processed into int32
  int32_t I_WAVE;
  
  uint32_t V_WAVE_RAW; //should be processed into int32
  int32_t V_WAVE;
  
  uint32_t I_RMS;
  
  uint32_t V_RMS;
  
  uint32_t WATT_RAW; //should be processed into int32
  int32_t WATT;
  
  uint32_t CF_CNT;
  
  uint32_t FREQ;
  
  uint32_t STATUS;
} bl_regs_data_t;

typedef enum {
  BL_STATE_IDLE,
  BL_STATE_READING
} bl_state_t;

volatile static bl_regs_data_t bl_data;
volatile static bl_state_t bl_state = BL_STATE_IDLE;
volatile static uint8_t current_reg_address = REG_I_WAVE;
static uint8_t __attribute__((aligned(4))) bl_tx_buf[6] = {0x58, 0x00, 0xFF, 0xFF, 0xFF, 0xFF}; // [CMD, REG?, D, D, D, D];
static uint8_t __attribute__((aligned(4))) bl_rx_buf[6];
extern SPI_HandleTypeDef hspi1;
extern volatile uint8_t it_events;

void bl_start_reading_regs(void) {
  if(bl_state == BL_STATE_IDLE) {
    bl_state = BL_STATE_READING;
    bl_tx_buf[1] = current_reg_address;
    HAL_SPI_TransmitReceive_DMA(&hspi1, bl_tx_buf, bl_rx_buf, 6);
  }
}

void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef *hspi) {
  uint32_t val = ((uint32_t)bl_rx_buf[2] << 16) + ((uint32_t)bl_rx_buf[3] << 8) + (uint32_t)bl_rx_buf[4]; // BL0942 register data acquired via SPI
  switch (current_reg_address) {
  case REG_I_WAVE:
    bl_data.I_WAVE_RAW = val;
    break;
  case REG_V_WAVE:
    bl_data.V_WAVE_RAW = val;
    break;
  case REG_I_RMS:
    bl_data.I_RMS = val;
    break;
  case REG_V_RMS:
    bl_data.V_RMS = val;
    break;
  case REG_WATT:
    bl_data.WATT_RAW = val;
    break;
  case REG_CF_CNT:
    bl_data.CF_CNT = val;
    break;
  case REG_FREQ:
    bl_data.FREQ = val;
    break;
  case REG_STATUS:
    bl_data.STATUS = val;
    break;
  }
  if (current_reg_address == REG_STATUS) {
    current_reg_address = REG_I_WAVE;
    bl_state = BL_STATE_IDLE;
    it_events |= IT_EVENT_SPI_CPLT; // Начать обработку данных регистров в суперцикле
  }
  else {
    current_reg_address++;
    bl_tx_buf[1] = current_reg_address;
    HAL_SPI_TransmitReceive_DMA(&hspi1, bl_tx_buf, bl_rx_buf, 6);
  }
}

static int32_t int24_to_int32(uint32_t int24_v) {
  int32_t int32_v;
  int32_v = (int32_t)(int24_v << 8);
  int32_v = int32_v >> 8;
  return int32_v;
}

static int32_t int20_to_int32(uint32_t int20_v) {
  int32_t int32_v;
  int32_v = (int32_t)(int20_v << 12);
  int32_v = int32_v >> 12;
  return int32_v;
}

void bl_process_data(void) {
  bl_data.I_WAVE = int20_to_int32(bl_data.I_WAVE_RAW);
  bl_data.V_WAVE = int20_to_int32(bl_data.V_WAVE_RAW);
  bl_data.WATT = int24_to_int32(bl_data.WATT_RAW);
}