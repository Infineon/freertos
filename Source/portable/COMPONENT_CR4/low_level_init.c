/*
 * Copyright 2024, Cypress Semiconductor Corporation or a subsidiary of
 * Cypress Semiconductor Corporation. All Rights Reserved.
 *
 * This software, associated documentation and materials ("Software"),
 * is owned by Cypress Semiconductor Corporation
 * or one of its subsidiaries ("Cypress") and is protected by and subject to
 * worldwide patent protection (United States and foreign),
 * United States copyright laws and international treaty provisions.
 * Therefore, you may use this Software only as provided in the license
 * agreement accompanying the software package from which you
 * obtained this Software ("EULA").
 * If no EULA applies, Cypress hereby grants you a personal, non-exclusive,
 * non-transferable license to copy, modify, and compile the Software
 * source code solely for use in connection with Cypress's
 * integrated circuit products. Any reproduction, modification, translation,
 * compilation, or representation of this Software except as specified
 * above is prohibited without the express written permission of Cypress.
 *
 * Disclaimer: THIS SOFTWARE IS PROVIDED AS-IS, WITH NO WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, NONINFRINGEMENT, IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. Cypress
 * reserves the right to make changes to the Software without notice. Cypress
 * does not assume any liability arising out of the application or use of the
 * Software or any product or circuit described in the Software. Cypress does
 * not authorize its products for use in any products where a malfunction or
 * failure of the Cypress product may reasonably be expected to result in
 * significant property damage, injury or death ("High Risk Product"). By
 * including Cypress's product in a High Risk Product, the manufacturer
 * of such system or application assumes all risk of such use and in doing
 * so agrees to indemnify Cypress against all liability.
 */
#include "FreeRTOS.h"
#include "platform_isr.h"
#include "portmacro.h"
#include "cyhal_timer.h"

extern void _cyhal_timer_irq_handler(void);
extern void platform_irq_demuxer(void);
/** @file
 * Implementation of IRQ handling functions for CR4.
 */

/******************************************************
 *                      Macros
 ******************************************************/

/******************************************************
 *                    Constants
 ******************************************************/

/******************************************************
 *                   Enumerations
 ******************************************************/

/******************************************************
 *                 Type Definitions
 ******************************************************/

/******************************************************
 *                    Structures
 ******************************************************/

/******************************************************
 *               Static Function Declarations
 ******************************************************/
void irq_vector_external_interrupt( void ) __attribute__((naked));
void irq_vector_software_interrupt( void ) __attribute__((naked));

/******************************************************
 *               Variable Definitions
 ******************************************************/

/******************************************************
 *               Function Definitions
 ******************************************************/

/* Initial entry point for external interrupts */
void irq_vector_external_interrupt( void )
{
    __asm volatile (
        "B    FreeRTOS_IRQ_Handler    \t\n"
    );
}

/* Initial entry point for software interrupts */
void irq_vector_software_interrupt( void )
{
    __asm volatile (
        "B    FreeRTOS_SVC_Handler    \t\n"
    );
}

static void timer_isr(void *callback_arg, cyhal_timer_event_t event)
{
    (void) (callback_arg);
    (void) (event);
    FreeRTOS_Tick_Handler( );
}

extern void platform_tick_start()
{
    cy_rslt_t result;
    static bool tick_timer_initialized = false;
    static cyhal_timer_t timer;
    const cyhal_timer_cfg_t TimerCfg = {true, CYHAL_TIMER_DIR_UP, true, configCPU_CLOCK_HZ/configTICK_RATE_HZ, (configCPU_CLOCK_HZ/configTICK_RATE_HZ)/2, 0};
    (void) result;
    if( !tick_timer_initialized )
    {
        result = cyhal_timer_init(&timer, 0, NULL);
        CY_ASSERT(result == CY_RSLT_SUCCESS);

        result = cyhal_timer_set_frequency(&timer, configCPU_CLOCK_HZ);
        CY_ASSERT(result == CY_RSLT_SUCCESS);
        
        cyhal_timer_configure(&timer, &TimerCfg);

        cyhal_timer_register_callback(&timer, timer_isr, NULL);
        cyhal_timer_enable_event(&timer, CYHAL_TIMER_IRQ_CAPTURE_COMPARE, 0, true);

        result = cyhal_timer_start(&timer);
        CY_ASSERT(result == CY_RSLT_SUCCESS);

        tick_timer_initialized = true;
    }
}

PLATFORM_DEFINE_ISR(platform_irq_demuxer_wrapper)
{
    platform_irq_demuxer();
}

PLATFORM_MAP_ISR(platform_irq_demuxer_wrapper, vApplicationIRQHandler);

/* This empty function is needed just to allow the linker to discard the symbol from the binary. Without this, the weak function platform_irq_demuxer_default defined in BSP will not be discarded by the linker. */
void platform_irq_demuxer_default(void)
{
    return;
}
