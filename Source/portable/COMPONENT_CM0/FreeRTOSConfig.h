/*
 * FreeRTOS Kernel V10.3.1
 * Copyright (C) 2020 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 * Copyright (C) 2019-2020 Cypress Semiconductor Corporation, or a subsidiary of
 * Cypress Semiconductor Corporation.  All Rights Reserved.
 *
 * Updated configuration to support PSoC 4 MCU.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * http://www.FreeRTOS.org
 * http://aws.amazon.com/freertos
 * http://www.cypress.com
 *
 */

#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

/*-----------------------------------------------------------
 * Application specific definitions.
 *
 * These definitions should be adjusted for your particular hardware and
 * application requirements.
 *
 * THESE PARAMETERS ARE DESCRIBED WITHIN THE 'CONFIGURATION' SECTION OF THE
 * FreeRTOS API DOCUMENTATION AVAILABLE ON THE FreeRTOS.org WEB SITE.
 *
 * See http://www.freertos.org/a00110.html.
 *----------------------------------------------------------*/

#include "cy_utils.h"
#include "cy_syslib.h"

/* Get the low power configuration parameters from
 * the ModusToolbox Device Configurator GeneratedSource:
 * CY_CFG_PWR_SYS_IDLE_MODE     - System Idle Power Mode
 * CY_CFG_PWR_DEEPSLEEP_LATENCY - Deep Sleep Latency (ms)
 */
#include "cycfg_system.h"

#warning This is a template. Copy this file to your project and remove this line. Refer to FreeRTOS README.md for usage details.

#define configUSE_PREEMPTION                    1
#define configUSE_PORT_OPTIMISED_TASK_SELECTION 0
#define configCPU_CLOCK_HZ                      SystemCoreClock
#define configTICK_RATE_HZ                      1000u
#define configMAX_PRIORITIES                    7
#define configMINIMAL_STACK_SIZE                64
#define configMAX_TASK_NAME_LEN                 16
#define configUSE_16_BIT_TICKS                  0
#define configIDLE_SHOULD_YIELD                 1
#define configUSE_TASK_NOTIFICATIONS            1
#define configUSE_MUTEXES                       1
#define configUSE_RECURSIVE_MUTEXES             1
#define configUSE_COUNTING_SEMAPHORES           1
#define configQUEUE_REGISTRY_SIZE               10
#define configUSE_QUEUE_SETS                    0
#define configUSE_TIME_SLICING                  0
#define configENABLE_BACKWARD_COMPATIBILITY     0
#define configNUM_THREAD_LOCAL_STORAGE_POINTERS 5

/* Memory allocation related definitions. */
#define configSUPPORT_STATIC_ALLOCATION         1
#define configSUPPORT_DYNAMIC_ALLOCATION        1
#define configTOTAL_HEAP_SIZE                   1024
#define configAPPLICATION_ALLOCATED_HEAP        0

/* Hook function related definitions. */
#define configUSE_IDLE_HOOK                     0
#define configUSE_TICK_HOOK                     0
#define configCHECK_FOR_STACK_OVERFLOW          2
#define configUSE_MALLOC_FAILED_HOOK            1
#define configUSE_DAEMON_TASK_STARTUP_HOOK      0

/* Run time and task stats gathering related definitions. */
#define configGENERATE_RUN_TIME_STATS           0
#define configUSE_TRACE_FACILITY                1
#define configUSE_STATS_FORMATTING_FUNCTIONS    0

/* Co-routine related definitions. */
#define configUSE_CO_ROUTINES                   0
#define configMAX_CO_ROUTINE_PRIORITIES         1

/* Software timer related definitions. */
#define configUSE_TIMERS                        1
#define configTIMER_TASK_PRIORITY               3
#define configTIMER_QUEUE_LENGTH                10
#define configTIMER_TASK_STACK_DEPTH            configMINIMAL_STACK_SIZE


/* Set the following definitions to 1 to include the API function, or zero
to exclude the API function. */
#define INCLUDE_vTaskPrioritySet                1
#define INCLUDE_uxTaskPriorityGet               1
#define INCLUDE_vTaskDelete                     1
#define INCLUDE_vTaskSuspend                    1
#define INCLUDE_xResumeFromISR                  1
#define INCLUDE_vTaskDelayUntil                 1
#define INCLUDE_vTaskDelay                      1
#define INCLUDE_xTaskGetSchedulerState          1
#define INCLUDE_xTaskGetCurrentTaskHandle       1
#define INCLUDE_uxTaskGetStackHighWaterMark     0
#define INCLUDE_xTaskGetIdleTaskHandle          0
#define INCLUDE_eTaskGetState                   0
#define INCLUDE_xEventGroupSetBitFromISR        1
#define INCLUDE_xTimerPendFunctionCall          1
#define INCLUDE_xTaskAbortDelay                 0
#define INCLUDE_xTaskGetHandle                  0
#define INCLUDE_xTaskResumeFromISR              1

/* Normal assert() semantics without relying on the provision of an assert.h
header file. */
#if defined(NDEBUG)
#define configASSERT( x ) CY_UNUSED_PARAMETER( x )
#else
#define configASSERT( x ) if( ( x ) == 0 ) { taskDISABLE_INTERRUPTS(); CY_HALT(); }
#endif

/* Definitions that map the FreeRTOS port interrupt handlers to their CMSIS
standard names - or at least those used in the unmodified vector table. */
#define vPortSVCHandler     SVC_Handler
#define xPortPendSVHandler  PendSV_Handler
#define xPortSysTickHandler SysTick_Handler

/* Dynamic Memory Allocation Schemes */
#define HEAP_ALLOCATION_TYPE1                   (1)     /* heap_1.c*/
#define HEAP_ALLOCATION_TYPE2                   (2)     /* heap_2.c*/
#define HEAP_ALLOCATION_TYPE3                   (3)     /* heap_3.c*/
#define HEAP_ALLOCATION_TYPE4                   (4)     /* heap_4.c*/
#define HEAP_ALLOCATION_TYPE5                   (5)     /* heap_5.c*/
#define NO_HEAP_ALLOCATION                      (0)

#define configHEAP_ALLOCATION_SCHEME            (HEAP_ALLOCATION_TYPE3)

/* Check if the ModusToolbox Device Configurator Power personality parameter
 * "System Idle Power Mode" is set to either "CPU Sleep" or "System Deep Sleep".
 */
#if defined(CY_CFG_PWR_SYS_IDLE_MODE) && \
    ((CY_CFG_PWR_SYS_IDLE_MODE == CY_CFG_PWR_MODE_SLEEP) || \
     (CY_CFG_PWR_SYS_IDLE_MODE == CY_CFG_PWR_MODE_DEEPSLEEP))

/* Enable low power tickless functionality. The RTOS abstraction library
 * provides the compatible implementation of the vApplicationSleep hook:
 * https://github.com/cypresssemiconductorco/abstraction-rtos#freertos
 */
extern void vApplicationSleep( uint32_t xExpectedIdleTime );
#define portSUPPRESS_TICKS_AND_SLEEP( xIdleTime ) vApplicationSleep( xIdleTime )
#define configUSE_TICKLESS_IDLE                 2

#else
#define configUSE_TICKLESS_IDLE                 0
#endif

/* Deep Sleep Latency Configuration */
#if( CY_CFG_PWR_DEEPSLEEP_LATENCY > 0 )
#define configEXPECTED_IDLE_TIME_BEFORE_SLEEP   CY_CFG_PWR_DEEPSLEEP_LATENCY
#endif

/* Allocate newlib reeentrancy structures for each RTOS task.
 * The system behavior is toolchain-specific.
 *
 * GCC toolchain: the application must provide the implementation for the required
 * newlib hook functions: __malloc_lock, __malloc_unlock, __env_lock, __env_unlock.
 * FreeRTOS-compatible implementation is provided by the clib-support library:
 * https://github.com/cypresssemiconductorco/clib-support
 *
 * ARM/IAR toolchains: the application must provide the reent.h header to adapt
 * FreeRTOS's configUSE_NEWLIB_REENTRANT to work with the toolchain-specific C library.
 * The compatible implementations are also provided by the clib-support library.
 */
#define configUSE_NEWLIB_REENTRANT              1

#endif /* FREERTOS_CONFIG_H */
