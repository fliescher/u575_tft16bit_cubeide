/*
*
*   Author: Matthias Guthörl (Github - fliescher)
*   Based on fmc.h published on https://github.com/NickNagy/Cortet/blob/master/Inc/fmc.h
*
*/

#ifndef FMC_H
#define FMC_H

#ifdef __cplusplus
extern "C" {
#endif

//#include "stm32f7xx_hal.h"//_sram.h"
#include "stm32u5xx_hal.h" //_sram.h"

//#define MEM_SWAPPING 1
#if MEM_SWAPPING
#define NORSRAM_BASE_ADDRESS 0xC0000000
#else
#define NORSRAM_BASE_ADDRESS 0x60000000
#endif

#define MIN_ADDSET_TIME  0
#define MAX_ADDSET_TIME  15
#define MIN_ADDHOLD_TIME 1
#define MAX_ADDHOLD_TIME 15
#define MIN_DATAST_TIME  1
#define MAX_DATAST_TIME  256

#define NORSRAM_BANK_BASE_ADDRESS(x) NORSRAM_BASE_ADDRESS + ((x-1)*0x10000000)

/* LCD */
#define FMC_LCD_8BIT_SEND_COMMAND(bank, command) *(__IO uint8_t *)(NORSRAM_BANK_BASE_ADDRESS(bank)) = command;
#define FMC_LCD_16BIT_SEND_COMMAND(bank, command) *(__IO uint16_t *)(NORSRAM_BANK_BASE_ADDRESS(bank)) = command;
#define FMC_LCD_8BIT_SEND_DATA(bank, a_x, data)  *(__IO uint8_t *)(NORSRAM_BANK_BASE_ADDRESS(bank) + (1 << a_x)) = data;
#define FMC_LCD_16BIT_SEND_DATA(bank, a_x, data) *(__IO uint16_t *)(NORSRAM_BANK_BASE_ADDRESS(bank) + (1 << (a_x + 1))) = data;

/* SRAM
 *
 * from pg 337 of STM32F76xx datasheet                      //Same for U575 (p.875, Table 181 in reference manual)
 *
 * Memory width | Data address issued to memory
 * ---------------------------------------------
 * 8bit         | HADDR[25:0]
 * 16bit        | HADDR[25:1] >> 1
 * 32bit        | HADDR[25:2] >> 2
 *
 * */
#define FMC_SRAM_8BIT_SEND_DATA(bank, address, data)  *(__IO uint8_t *)(NORSRAM_BANK_BASE_ADDRESS(bank) + address) = data;
#define FMC_SRAM_8BIT_READ_DATA(bank, address) *(__IO uint8_t *)(NORSRAM_BANK_BASE_ADDRESS(bank) + address);
#define FMC_SRAM_16BIT_SEND_DATA(bank, address, data) *(__IO uint16_t *)(NORSRAM_BANK_BASE_ADDRESS(bank) + (address<<1)) = data;
#define FMC_SRAM_16BIT_READ_DATA(bank, address) *(__IO uint16_t *)(NORSRAM_BANK_BASE_ADDRESS(bank) + (address<<1));
#define FMC_SRAM_32BIT_SEND_DATA(bank, address, data) *(__IO uint32_t *)(NORSRAM_BANK_BASE_ADDRESS(bank) + (address<<2)) = data;
#define FMC_SRAM_32BIT_READ_DATA(bank, address) *(__IO uint32_t *)(NORSRAM_BANK_BASE_ADDRESS(bank) + (address<<2));

typedef struct ExternalSRAMSpecStruct {
	uint8_t dataSize;
	uint8_t tCycRead;
	uint8_t tCycWrite;
	uint8_t tWRLW;
	uint8_t tACC;
	uint8_t tAS;
	uint8_t writeOnly;
} ExternalSRAMSpecStruct;

void FMCComputeAddsetDatast(ExternalSRAMSpecStruct * sramSpec, FMC_NORSRAM_TimingTypeDef * timing);
void FMCSRAMInit(ExternalSRAMSpecStruct * sramSpec, uint8_t bank);
void FMC_Error_Handler();

#ifdef __cplusplus
}
#endif

#endif
