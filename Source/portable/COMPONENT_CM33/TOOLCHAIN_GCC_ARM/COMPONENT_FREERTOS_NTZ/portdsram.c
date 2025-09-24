/*
 * FreeRTOS Kernel V10.6.2
 * Copyright (C) 2021 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
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
 * https://www.FreeRTOS.org
 * https://github.com/FreeRTOS
 *
 * 1 tab == 4 spaces!
 */

/* Standard includes. */
#include "portdsram.h"
/* Defining MPU_WRAPPERS_INCLUDED_FROM_API_FILE ensures that PRIVILEGED_FUNCTION
 * is defined correctly and privileged functions are placed in correct sections. */
#define MPU_WRAPPERS_INCLUDED_FROM_API_FILE

/* Portasm includes. */
#include "portasm.h"

/* MPU_WRAPPERS_INCLUDED_FROM_API_FILE is needed to be defined only for the
 * header files. */
#undef MPU_WRAPPERS_INCLUDED_FROM_API_FILE

FREERTOS_COMMON_SECTION_BEGIN
void vStoreDSRAMContextWithWFI( void ) /* __attribute__ (( naked )) PRIVILEGED_FUNCTION */
{
      __asm volatile
        (
         " .syntax unified                         \n"
         " stmdb sp!, {r1-r11, lr}                 \n" /* Store reg on the stack*/
         " mrs r1, psplim                          \n" /* r1 = PSPLIM. */
         " mrs r2, control                         \n" /* r2 = Control. */
         " adr r3,wfi_exit + 1                     \n"
         " stmdb sp!, {r1-r3}                      \n" /* Store on the stack - PSPLIM, Control Return Address */
         "                                         \n"
         " ldr r2, =pxCurrentTCB                   \n" /* Read the location of pxCurrentTCB i.e. &( pxCurrentTCB ). */
         " ldr r1, [r2]                            \n" /* Read pxCurrentTCB. */
         " mrs r0, psp                             \n" /* Read PSP in r0. */
         " str r0, [r1]                            \n" /* Save the new top of stack in TCB. */
         " wfi                                     \n"
         " isb                                     \n"
         " add  sp, sp, #12                        \n" /* We we do not  enter DSRAM pop-out PSPLIM, Control Return Address*/
         "wfi_exit:     dsb                        \n"
         " ldmia sp!, {r1-r11, pc}                 \n"
         );
}
FREERTOS_COMMON_SECTION_END

FREERTOS_COMMON_SECTION_BEGIN
void vRestoreDSRAMContext( void )
{
      __asm volatile
        (
         " ldr r2, pxCurrentTCBConstE1             \n" /* Read the location of pxCurrentTCB i.e. &( pxCurrentTCB ). */
         " ldr r1, [r2]                            \n" /* Read pxCurrentTCB. */
         " ldr r0, [r1]                            \n" /* The first item in pxCurrentTCB is the task top of stack. r0 now points to the top of stack. */
         "                                         \n"
         " ldmia r0!, {r1-r3}                      \n" /* Read from stack - r1 = PSPLIM, r2 = CONTROL, r3 = LR */
         "                                         \n"
         " msr psplim, r1                          \n" /* Restore the PSPLIM register value for the task. */
         " msr psp, r0                             \n" /* restore stacks */
         " ldr r0, MSPStackTop                     \n"
         " msr msp, r0                             \n"
         " msr CONTROL, r2                         \n"
         " isb                                     \n"
         " bx r3                                   \n"
         "                                         \n"
         " .align 4                                \n"
         "pxCurrentTCBConstE1: .word pxCurrentTCB  \n"
         "MSPStackTop: .word __StackTop            \n"
        );
}
FREERTOS_COMMON_SECTION_END
/*-----------------------------------------------------------*/
