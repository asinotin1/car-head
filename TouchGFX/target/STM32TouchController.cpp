/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : STM32TouchController.cpp
  ******************************************************************************
  * This file was created by TouchGFX Generator 4.26.0. This file is only
  * generated once! Delete this file from your project and re-generate code
  * using STM32CubeMX or change this file manually to update it.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* USER CODE BEGIN STM32TouchController */

#include <STM32TouchController.hpp>
#include "Bsp_Gt9xx.h"
#include "tim.h"
#include <stdlib.h>

// Debounce & Noise Filter Configuration
static const uint32_t DEBOUNCE_PRESS_TICKS = 2;   // Min consecutive frames (~33ms) to confirm touch press
static const uint32_t DEBOUNCE_RELEASE_TICKS = 2; // Min consecutive frames to confirm touch release
static const int32_t  JITTER_THRESHOLD = 4;        // Ignore coordinate jitter smaller than 4 pixels

static int32_t  lastX = -1;
static int32_t  lastY = -1;
static uint32_t pressCount = 0;
static uint32_t releaseCount = 0;
static bool     isTouchActive = false;

void STM32TouchController::init()
{
    Touch_IIC_SetTimerBase(&htim15);
    Touch_Init();

    lastX = -1;
    lastY = -1;
    pressCount = 0;
    releaseCount = 0;
    isTouchActive = false;
}

bool STM32TouchController::sampleTouch(int32_t& x, int32_t& y)
{
    Touch_Scan();

    if (touchInfo.flag && touchInfo.num > 0)
    {
        releaseCount = 0;
        pressCount++;

        int32_t rawX = touchInfo.x[0];
        int32_t rawY = touchInfo.y[0];

        // Apply Press Debounce: Confirm press only after stable consecutive frames
        if (pressCount >= DEBOUNCE_PRESS_TICKS)
        {
            // Apply Coordinate Jitter Filtering for close buttons
            if (isTouchActive && lastX >= 0 && lastY >= 0)
            {
                if (abs(rawX - lastX) < JITTER_THRESHOLD)
                {
                    rawX = lastX;
                }
                if (abs(rawY - lastY) < JITTER_THRESHOLD)
                {
                    rawY = lastY;
                }
            }

            lastX = rawX;
            lastY = rawY;
            x = rawX;
            y = rawY;
            isTouchActive = true;
            return true;
        }
        else if (isTouchActive)
        {
            x = lastX;
            y = lastY;
            return true;
        }
    }
    else
    {
        pressCount = 0;
        releaseCount++;

        // Apply Release Debounce: Prevent false release from momentary signal drops
        if (isTouchActive && releaseCount < DEBOUNCE_RELEASE_TICKS)
        {
            x = lastX;
            y = lastY;
            return true;
        }

        isTouchActive = false;
        lastX = -1;
        lastY = -1;
    }

    return false;
}

/* USER CODE END STM32TouchController */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
