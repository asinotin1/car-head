#include <touchgfx/hal/OSWrappers.hpp>
#include <touchgfx/hal/HAL.hpp>
#include "main.h"

using namespace touchgfx;

// Không dùng RTOS: mô phỏng semaphore/queue bằng cờ volatile đơn giản
static volatile bool vsync_flag = false;

void OSWrappers::initialize()
{
    // Không cần khởi tạo gì thêm khi chạy bare-metal
}

void OSWrappers::takeFrameBufferSemaphore()
{
    // Bare-metal: không có tranh chấp task, để trống
}

void OSWrappers::giveFrameBufferSemaphore()
{
    // Bare-metal: để trống
}

void OSWrappers::tryTakeFrameBufferSemaphore()
{
    // Bare-metal: để trống
}

void OSWrappers::giveFrameBufferSemaphoreFromISR()
{
    // Bare-metal: để trống
}

void OSWrappers::signalVSync()
{
    // Được gọi từ LTDC_IRQHandler mỗi khi có ngắt line/vsync
    vsync_flag = true;
}

void OSWrappers::waitForVSync()
{
    // Chờ bận (busy-wait) tới khi có vsync tiếp theo
    while (!vsync_flag)
    {
    }
    vsync_flag = false;
}

bool OSWrappers::isVSyncAvailable()
{
    // Kiểm tra không chặn (non-blocking): nếu có vsync thì tiêu thụ cờ và trả về true
    if (vsync_flag)
    {
        vsync_flag = false;
        return true;
    }
    return false;
}

void OSWrappers::taskDelay(uint16_t ms)
{
    HAL_Delay(ms);
}