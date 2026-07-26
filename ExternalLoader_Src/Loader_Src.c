#include "Loader_Src.h"
#include "Bsp_W25q64.h"
#include "quadspi.h"
#include "main.h"

extern QSPI_HandleTypeDef hqspi;

/**
 * Hàm Error_Handler để fix lỗi undefined reference
 */
void Error_Handler(void)
{
    while(1);
}

/**
 * Hàm Init() được gọi bởi STM32CubeProgrammer
 */
__attribute__((used)) int Init (void)
{
    /* Reset cấu hình hệ thống */
    HAL_Init();

    /* Khởi tạo clock (có thể tùy chỉnh theo SystemClock_Config() của bạn) */
    /* Trong loader, đôi khi dùng clock mặc định HSI là đủ để giao tiếp SPI */
    
    /* Cấu hình QSPI */
    MX_QUADSPI_Init();

    /* Khởi tạo giao tiếp W25Qxx */
    if (QSPI_W25Qxx_Init() == QSPI_W25Qxx_OK)
    {
        return 1; // 1 = Thành công
    }

    return 0; // 0 = Thất bại
}

/**
 * Ghi dữ liệu vào QSPI
 */
__attribute__((used)) int Write (uint32_t Address, uint32_t Size, uint8_t* buffer)
{
    uint32_t offset = Address - W25Qxx_Mem_Addr;

    if (QSPI_W25Qxx_WriteBuffer(buffer, offset, Size) == QSPI_W25Qxx_OK)
    {
        return 1;
    }

    return 0;
}

/**
 * Đọc dữ liệu từ QSPI (Quan trọng cho Verify của CubeProgrammer)
 */
__attribute__((used)) int Read (uint32_t Address, uint32_t Size, uint8_t* buffer)
{
    uint32_t offset = Address - W25Qxx_Mem_Addr;
    
    if (QSPI_W25Qxx_ReadBuffer(buffer, offset, Size) == QSPI_W25Qxx_OK)
    {
        return 1;
    }
    
    return 0;
}

/**
 * Xóa một khoảng bộ nhớ Sector theo khoảng địa chỉ
 */
__attribute__((used)) int SectorErase (uint32_t EraseStartAddress, uint32_t EraseEndAddress)
{
    uint32_t localStart = EraseStartAddress - W25Qxx_Mem_Addr;
    uint32_t localEnd = EraseEndAddress - W25Qxx_Mem_Addr;

    while (localStart <= localEnd)
    {
        if (QSPI_W25Qxx_SectorErase(localStart) != QSPI_W25Qxx_OK)
        {
            return 0;
        }
        localStart += 4096; // W25Q64 Sector Size là 4KB
    }

    return 1;
}

/**
 * Xóa toàn bộ chip Flash
 */
__attribute__((used)) int MassErase (void)
{
    if (QSPI_W25Qxx_ChipErase() == QSPI_W25Qxx_OK)
    {
        return 1;
    }
    return 0;
}
