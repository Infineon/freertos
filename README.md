# FreeRTOS

### Overview
FreeRTOS is a small operating system for microcontrollers.
It is supplied as standard C source files
built along with the other C files in your project.
This repository contains a port of FreeRTOS kernel for PSoC 6 and PSoC 4 MCUs.

FreeRTOS API Documentation is available at
<a href="https://www.freertos.org/a00106.html" target="_blank">FreeRTOS web page</a>.

### Features
* Simple, small, and easy to use.
   A typical RTOS kernel binary image is in the region 6K-12K bytes.
* Flexible scheduler configuration – preemptive, cooperative, and hybrid options with optional time-slicing.
* RTOS objects (tasks, queues, semaphores, software timers, mutexes, and event groups)
    can be created using either dynamically or statically allocated RAM.
* Tickless mode for low-power applications.
* Mutexes with priority inheritance.
* Supports both real time tasks and co-routines.
* Efficient software timers.
* Stack overflow-detection options.
* Royalty-free.

### Quick Start
The quick start guide provides steps to create a simple blinking LED project
with a single task using FreeRTOS.

FreeRTOS can be used in various Development Environments such as ModusToolbox or others.
Refer to [FreeRTOS Release.md](./RELEASE.md) for details.
The quickest way to get started is using the Code Examples.
Cypress Semiconductor continuously extends its portfolio of the code examples at
the <a href="http://www.cypress.com">Cypress Semiconductor website</a>
and at the <a href="https://github.com/cypresssemiconductorco">
Cypress Semiconductor GitHub</a>.

This quick start guide assumes that the environment is configured to use the
PSoC 6 Board Support Package for your kit and the
PSoC 6 Board Support Package is included in the project.

To create a simple FreeRTOS application:

1. Add FreeRTOS to the project. For ModusToolbox, enable FreeRTOS using Library Manager.
2. Add the RTOS support libraries to the project:
    * [clib-support](https://github.com/cypresssemiconductorco/clib-support)
    * [abstraction-rtos](https://github.com/cypresssemiconductorco/abstraction-rtos)

    Note: ModusToolbox Library Manager automatically pulls the dependent libraries
    once the FreeRTOS library is selected.
3. Add `FREERTOS` to the `COMPONENTS` variable defined in the ModusToolbox application Makefile:
    ```
    COMPONENTS+=FREERTOS
    ```
3. Copy the FreeRTOSConfig.h file from the freertos/Source/portable/COMPONENT_$(CORE)
   folder to your project, where $(CORE) is the target ARM Cortex-M CPU core (CM0, CM0P or CM4).
4. Open the copied FreeRTOSConfig.h file and remove the `#warning This is a template.` line.
5. In the main.c file:
    * Include the required header files:
        ```c
        #include "cyhal.h"
        #include "cybsp.h"
        #include "FreeRTOS.h"
        #include "task.h"
        ```

    * Specify GPIO port and pin for the LED:
        ```c
        #define LED_GPIO (CYBSP_USER_LED)
        ```

    * Add the function to toggle LED:
        ```c
        void blinky(void * arg)
        {
            (void)arg;

            /* Initialize the LED GPIO pin */
            cyhal_gpio_init(LED_GPIO, CYHAL_GPIO_DIR_OUTPUT,
                            CYHAL_GPIO_DRIVE_STRONG, CYBSP_LED_STATE_OFF);

            for(;;)
            {
                /* Toggle the LED periodically */
                cyhal_gpio_toggle(LED_GPIO);
                vTaskDelay(500);
            }
        }
        ```

    * Create a task using the previously added LED toggle function and start the task scheduler:
        ```c
        UBaseType_t __attribute__((used)) uxTopUsedPriority;

        int main(void)
        {
            BaseType_t retval;
            cy_rslt_t result;

            /* This enables RTOS aware debugging in OpenOCD. */
            uxTopUsedPriority = configMAX_PRIORITIES - 1;

            /* Initialize the device and board peripherals */
            result = cybsp_init() ;
            if (result != CY_RSLT_SUCCESS)
            {
                CY_ASSERT(0);
            }

            __enable_irq();

            retval = xTaskCreate(blinky, "blinky", configMINIMAL_STACK_SIZE, NULL, 5, NULL);

            if (pdPASS == retval)
            {
                vTaskStartScheduler();
            }

            for (;;)
            {
                /* vTaskStartScheduler never returns */
            }
        }
        ```

        Note: defining `uxTopUsedPriority` variable is recommended for correct
        RTOS support in the OpenOCD Debugger (enable thread awareness).

6. Build the project and program it into the target kit.
7. Observe LED blinking on the kit.

### Configuration Considerations
To configure FreeRTOS, copy the pre-configured FreeRTOSConfig.h file from
the freertos/Source/portable folder to your project and modify the copied configuration file as needed.
The <a href="http://www.freertos.org/a00110.html" target="_blank">Customization</a>
documentation describes the configuration options available in the FreeRTOSConfig.h file.

The following configuration values are especially important to PSoC6 / PSoC 4 MCU FreeRTOS ports:

* [configCPU_CLOCK_HZ](#configcpu_clock_hz)
* [configMAX_SYSCALL_INTERRUPT_PRIORITY](#configmax_syscall_interrupt_priority)
* [configHEAP_ALLOCATION_SCHEME](#configheap_allocation_scheme)
* [configTOTAL_HEAP_SIZE](#configtotal_heap_size)
* [configMINIMAL_STACK_SIZE](#configminimal_stack_size)
* [configUSE_TICKLESS_IDLE](#configuse_tickless_idle)
* [configUSE_NEWLIB_REENTRANT](#configuse_newlib_reentrant)

#### configCPU_CLOCK_HZ
This parameter passes to the FreeRTOS frequency of the MCU core required for
FreeRTOS system timer configuration.
The pre-configured FreeRTOSConfig.h file provided with this FreeRTOS package sets
the configCPU_CLOCK_HZ value as SystemCoreClock calculated by the
PSoC MCU system startup code.

#### configMAX_SYSCALL_INTERRUPT_PRIORITY
This parameter sets the highest interrupt priority to call interrupt-safe
FreeRTOS functions from.
Calling the FreeRTOS functions from an interrupt with a priority higher than
configMAX_SYSCALL_INTERRUPT_PRIORITY causes FreeRTOS to generate an exception.
To avoid this problem:
1. Reduce all interrupt priorities to configMAX_SYSCALL_INTERRUPT_PRIORITY or
    lower.
2. Trigger an interrupt with a priority less or equal to
    configMAX_SYSCALL_INTERRUPT_PRIORITY and call the FreeRTOS functions
    from this interrupt handler.
3. Call the FreeRTOS functions from the traceTASKSWITCHEDOUT() macro.
    [see FreeRTOS support](https://www.freertos.org/FreeRTOS_Support_Forum_Archive/October_2014/freertos_How_to_wake_up_a_FreeRtos_task_from_a_high_priority_ISR_140cde98j.html).

If your system pipe (IPC) interrupt priority is less than or equal to
configMAX_SYSCALL_INTERRUPT_PRIORITY, be careful with code that writes
to  flash (including the Flash/BLE/Emulated EEPROM/DFU). The
duration of critical sections must be kept short. For details, see
the psoc6pdl flash driver documentation.

The interrupt priority parameters are ignored by the Cortex M0/M0+ ports
as these CPUs use a simpler Nested Vectored Interrupt Controller (NVIC):

* configKERNEL_INTERRUPT_PRIORITY
* configMAX_SYSCALL_INTERRUPT_PRIORITY
* configMAX_API_CALL_INTERRUPT_PRIORITY

#### configHEAP_ALLOCATION_SCHEME
This parameter specifies the memory management scheme.
The FreeRTOS kernel requires RAM for each created task, queue, mutex, software timer,
semaphore, or event group.
To manage used memory, FreeRTOS implements several different memory
management schemes to choose the most suitable for your application.

Each scheme is documented in the FreeRTOS
[Memory Management](http://www.freertos.org/a00111.html) topic.
The memory management implementation files are in the freertos/Source/portable/MemMang
directory.

The available memory management schemes:

- HEAP_ALLOCATION_TYPE1 ([heap_1](http://www.freertos.org/a00111.html#heap_1)) -
  The simplest, does not permit memory to be freed. Active by default.
- HEAP_ALLOCATION_TYPE2 ([heap_2](http://www.freertos.org/a00111.html#heap_2)) -
  Permits memory to be freed, but not does coalesce adjacent free blocks.
- HEAP_ALLOCATION_TYPE3 ([heap_3](http://www.freertos.org/a00111.html#heap_3)) -
  Simply wraps a standard malloc() and free() for thread safety.
- HEAP_ALLOCATION_TYPE4 ([heap_4](http://www.freertos.org/a00111.html#heap_4)) -
  Coalesces adjacent free blocks to avoid fragmentation. Includes the absolute address-placement option.
- HEAP_ALLOCATION_TYPE5 ([heap_5](http://www.freertos.org/a00111.html#heap_5)) -
  As per heap_4, with the ability to span the heap across multiple non-adjacent memory areas.
- NO_HEAP_ALLOCATION -
  Disables the memory management, used for applications with static memory allocation.

In the heap_3 memory scheme, your linker file must specify a sufficient size of
heap and stack, and your firmware must implement and use malloc() and free()
to allocate and release memory.

In the other memory schemes, the RTOS itself allocates a stack and heap.
For these schemes, the stack defined in the Board Support Package linker file
is used only by the main() function and the functions it calls.

HEAP_ALLOCATION_TYPE3 is the default memory management scheme enabled in FreeRTOSConfig.h.
It is suggested for use with the applications that rely on malloc() and free()
implementations provided by the standard C library (for example, GCC + newlib).
To avoid a possible corruption of the heap storage pool due to simultaneous
calls to malloc from multiple threads, it is strongly recommended to enable
[configUSE_NEWLIB_REENTRANT](#configuse_newlib_reentrant) configuration parameter,
and include [CLib FreeRTOS Support Library](https://github.com/cypresssemiconductorco/clib-support)
in the application.
Note: In ModusToolbox, clib-support has been added as a dependency to this
FreeRTOS library so that you do not need to import it separately.

For more details on Memory Management schemes, see
the [FreeRTOS Documentation](http://www.freertos.org/a00111.html) page.

#### configTOTAL_HEAP_SIZE
This parameter specifies a total amount of RAM available for the FreRTOS heap.
This parameter ignored when heap_3 memory scheme is used.

#### configMINIMAL_STACK_SIZE
This parameter specifies the size of the stack used by the idle task.
It is not recommended to reduce the default parameter value.

#### configUSE_TICKLESS_IDLE
This parameter configures tickless idle functionality required for low power mode support.

The tickless mode is enabled automatically in case the ModusToolbox Device Configurator
Power personality parameter "System Idle Power Mode" is set to either "CPU Sleep"
or "System Deep Sleep". The correct operation of the tickless mode depends
on the vApplicationSleep hook provided by the application.
The [RTOS Abstraction library](https://github.com/cypresssemiconductorco/abstraction-rtos#freertos)
provides the compatible implementation of the vApplicationSleep hook.
The [Low Power Assistant library](https://github.com/cypresssemiconductorco/lpa)
provides additional portable configuration layer for low-power features supported
by the PSoC 6 devices.

For more details on Low Power support, refer to the below resources:
* [Tickless Idle Mode](https://www.freertos.org/low-power-tickless-rtos.html)
* [Low Power RTOS For ARM Cortex-M MCUs](https://www.freertos.org/low-power-ARM-cortex-rtos.html)
* [LPA Middleware API Reference Guide](https://cypresssemiconductorco.github.io/lpa/lpa_api_reference_manual/html/index.html)

#### configUSE_NEWLIB_REENTRANT
This parameter enables allocation of newlib reentrancy structures for each RTOS task.

The system behavior is toolchain-specific.

* GCC toolchain: the application must provide the implementation for the required
  newlib hook functions: __malloc_lock, __malloc_unlock, __env_lock, __env_unlock.
  FreeRTOS-compatible implementation is provided by the clib-support library:
  https://github.com/cypresssemiconductorco/clib-support

*  ARM/IAR toolchains: the application must provide the reent.h header to adapt
  FreeRTOS's configUSE_NEWLIB_REENTRANT to work with the toolchain-specific C library.
  The compatible implementations are provided by the clib-support library.

For more details on the newlib reentrancy support, refer to the below resources:
* [FreeRTOS Customization](https://www.freertos.org/a00110.html#configUSE_NEWLIB_REENTRANT)
* [Newlib reentrancy](https://sourceware.org/newlib/libc.html#Reentrancy)
* [CLib FreeRTOS Support Library](https://github.com/cypresssemiconductorco/clib-support)
* [newlib and FreeRTOS configuration hints](http://www.nadler.com/embedded/newlibAndFreeRTOS.html)

### More information
The following resources contain more information:
* [FreeRTOS RELEASE.md](./RELEASE.md)
* [FreeRTOS API Documentation](http://www.freertos.org/a00106.html)
* [FreeRTOS Home Page](https://www.freertos.org/index.html)
* [ModusToolbox Software Environment, Quick Start Guide, Documentation, and Videos](https://www.cypress.com/products/modustoolbox-software-environment)
* [PSoC 6 Technical Reference Manuals](https://www.cypress.com/search/all?f%5B0%5D=meta_type%3Atechnical_documents&f%5B1%5D=resource_meta_type%3A583&f%5B2%5D=field_related_products%3A114026)
* [PSoC 6 Programmable System-on-Chip datasheets](https://www.cypress.com/search/all?f%5B0%5D=meta_type%3Atechnical_documents&f%5B1%5D=field_related_products%3A114026&f%5B2%5D=resource_meta_type%3A575)
* [Cypress Semiconductor](http://www.cypress.com)
---
© Cypress Semiconductor Corporation, 2019-2020.