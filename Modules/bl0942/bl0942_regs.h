// Read-only registers
#define REG_I_WAVE         0x01  // Current waveform data, signed
#define REG_V_WAVE         0x02  // Voltage waveform data, signed
#define REG_I_RMS          0x03  // Current RMS, unsigned
#define REG_V_RMS          0x04  // Voltage RMS, unsigned
#define REG_I_FAST_RMS     0x05  // Current fast RMS, unsigned
#define REG_WATT           0x06  // Active power, signed
#define REG_CF_CNT         0x07  // Active energy pulse counter, unsigned
#define REG_FREQ           0x08  // Line voltage frequency
#define REG_STATUS         0x09  // System status

// Read and write registers
#define REG_I_RMSOS        0x12  // Current RMS offset
#define REG_WA_CREEP       0x14  // Active power no-load threshold
#define REG_I_FAST_RMS_TH  0x15  // Current fast RMS threshold
#define REG_I_FAST_RMS_CYC 0x16  // Line cycle for current fast RMS measurement
#define REG_FREQ_CYC       0x17  // Line cycle for line voltage frequency measurement
#define REG_OT_FUNX        0x18  // Logic output configuration
#define REG_MODE           0x19  // User mode selection
#define REG_GAIN_CR        0x1A  // Current channel gain
#define REG_SOFT_RESET     0x1C  // Software reset
#define REG_USR_WRPROT     0x1D  // User write protection