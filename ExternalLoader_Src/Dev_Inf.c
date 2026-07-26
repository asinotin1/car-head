#include <stdint.h>

#define W25Q64_FLASH_SIZE 0x800000 // 8 MBytes

struct StorageInfo {
    char DeviceName[100];
    uint16_t DeviceType;
    uint32_t DeviceStartAddress;
    uint32_t DeviceSize;
    uint32_t PageSize;
    uint8_t Reserved;
    uint8_t Empty;
    uint32_t Sectors[2];
};

__attribute__((section(".Loader_Info"))) struct StorageInfo const StorageInfo = {
    "WeAct_H743_W25Q64",                // Tên hiển thị trên STM32CubeProgrammer
    0x08,                               // Device Type: SPI FLASH = 0x08
    0x90000000,                         // Địa chỉ bắt đầu của QSPI Memory (Bank 1)
    W25Q64_FLASH_SIZE,                  // Kích thước chip (8MB)
    256,                                // Page Size
    0,                                  // Reserved
    0xFF,                               // Initial content of erased memory
    {
        (W25Q64_FLASH_SIZE / 4096),     // Tổng số Sector
        4096                            // Kích thước 1 Sector
    }
};
