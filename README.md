# FreeRTOS for Infineon MCUs

## Overview

FreeRTOS is supplied as standard C source files built along with the other C files in your project. This repository contains a port of the FreeRTOS kernel for Infineon MCUs based on Arm&reg; Cortex&reg;-M0 (CM0), Cortex&reg;-M0+ (CM0P), Cortex&reg;-M4 (CM4), Cortex&reg;-M33 (CM33), Cortex&reg;-M55 (CM55), Cortex&reg;-R4 (CR4) and Cortex&reg;-M7 (CM7) cores.

**Note:** Cortex&reg;-R4 (CR4) is currently supported only on GCC_ARM

See the FreeRTOS API documentation at <a href="https://www.freertos.org/a00106.html" target="_blank">FreeRTOS web page</a>.

## Features

- Simple, small, and easy to use – a typical RTOS kernel binary image is in the region of 6K–12K bytes
- Flexible scheduler configuration – preemptive, cooperative, and hybrid options with optional time-slicing
- RTOS objects (tasks, queues, semaphores, software timers, mutexes, and event groups) can be created using either dynamically or statically allocated RAM
- Tickless mode for low-power applications
- Mutexes with priority inheritance
- Supports both real-time tasks and co-routines
- Efficient software timers
- Stack overflow detection options
- Royalty-free

## Configuration considerations

To configure FreeRTOS, copy the pre-configured *FreeRTOSConfig.h* file from the *freertos/Source/portable* folder to your project and modify the copied configuration file as needed.

See the <a href="http://www.freertos.org/a00110.html" target="_blank">Customization</a> documentation for configuration options available in the *FreeRTOSConfig.h* file.

The following configuration values are especially important to CM0, CM0P, CM4, CM33, CM55, CR4 and CM7 FreeRTOS ports:

- [`configCPU_CLOCK_HZ`](#configcpu_clock_hz)
- [`configMAX_SYSCALL_INTERRUPT_PRIORITY`](#configmax_syscall_interrupt_priority)
- [`configHEAP_ALLOCATION_SCHEME`](#configheap_allocation_scheme)
- [`configTOTAL_HEAP_SIZE`](#configtotal_heap_size)
- [`configMINIMAL_STACK_SIZE`](#configminimal_stack_size)
- [`configMINIMAL_SECURE_STACK_SIZE`](#configminimal_secure_stack_size)(only for CM33 NSC)
- [`configUSE_TICKLESS_IDLE`](#configuse_tickless_idle)
- [`configUSE_NEWLIB_REENTRANT`](#configuse_newlib_reentrant)
- [`configMAX_PRIORITIES`](#configmax_priorities)


### `configCPU_CLOCK_HZ`

This parameter passes the frequency of the MCU core required for the FreeRTOS system timer configuration to FreeRTOS. The pre-configured *FreeRTOSConfig.h* file provided with this FreeRTOS package sets the `configCPU_CLOCK_HZ` value as `SystemCoreClock` calculated by the MCU system startup code.

### `configMAX_SYSCALL_INTERRUPT_PRIORITY`

This parameter sets the highest interrupt priority to call interrupt-safe FreeRTOS functions from. Calling the FreeRTOS functions from an interrupt with a priority higher than `configMAX_SYSCALL_INTERRUPT_PRIORITY`causes FreeRTOS to generate an exception.

Do the following to avoid this problem:

1. Reduce all interrupt priorities to `configMAX_SYSCALL_INTERRUPT_PRIORITY` or lower.

2. Trigger an interrupt with a priority less than or equal to
    `configMAX_SYSCALL_INTERRUPT_PRIORITY` and call the FreeRTOS functions from this interrupt handler.

3. Call the FreeRTOS functions from the `traceTASKSWITCHEDOUT()` macro. See [FreeRTOS support](https://www.freertos.org/FreeRTOS_Support_Forum_Archive/October_2014/freertos_How_to_wake_up_a_FreeRtos_task_from_a_high_priority_ISR_140cde98j.html).

**Note:** If your system pipe (IPC) interrupt priority is less than or equal to `configMAX_SYSCALL_INTERRUPT_PRIORITY`, be careful with code that writes to the flash (including the flash/Bluetooth&reg; LE/Emulated EEPROM/DFU). The duration of critical sections must be kept short. For details, see
the flash driver section of the PDL. For more details on PDL drivers, refer to the [PDL section of ModusToolbox&trade; tools package user guide](https://www.infineon.com/assets/row/public/documents/30/68/infineon-modustoolbox-tools-package-user-guide-gettingstarted-en.pdf).

The interrupt priority parameters are ignored by the Cortex&reg; M0/M0P ports because these CPUs use a simpler nested vectored interrupt controller (NVIC):

- `configKERNEL_INTERRUPT_PRIORITY`
- `configMAX_SYSCALL_INTERRUPT_PRIORITY`
- `configMAX_API_CALL_INTERRUPT_PRIORITY`

See [Link](https://www.freertos.org/RTOS-Cortex-M3-M4.html) for more information on interrupt priority.

### configHEAP_ALLOCATION_SCHEME

This parameter specifies the heap memory management scheme.

Each scheme is documented in the FreeRTOS
[heap memory management](http://www.freertos.org/a00111.html) topic. The heap memory management implementation files are in the *mtb_shared/freertos/release-vX.Y.Z/Source/portable/MemMang* directory.

The following heap memory management schemes are available:

|Function | Description |
|---------|-------------|
|`HEAP_ALLOCATION_TYPE1` ([heap_1](http://www.freertos.org/a00111.html#heap_1c)) | The simplest; does not permit memory to be freed. Active by default |
|`HEAP_ALLOCATION_TYPE2` ([heap_2](http://www.freertos.org/a00111.html#heap_2c)) | Permits memory to be freed, but does not coalesce adjacent free blocks |
|`HEAP_ALLOCATION_TYPE3` ([heap_3](http://www.freertos.org/a00111.html#heap_3c))| Simply wraps a standard `malloc()` and `free()` for thread safety |
|`HEAP_ALLOCATION_TYPE4` ([heap_4](http://www.freertos.org/a00111.html#heap_4c)) | Coalesces adjacent free blocks to avoid fragmentation. Includes the absolute address-placement option |
|`HEAP_ALLOCATION_TYPE5` ([heap_5](http://www.freertos.org/a00111.html#heap_5c)) | As per `heap_4`, with the ability to span the heap across multiple non-adjacent memory areas |
|`NO_HEAP_ALLOCATION` | Disables heap memory management; used for applications with static memory allocation |

In the heap_3 memory scheme, your linker file must specify a sufficient size of the heap and stack, and your firmware must implement and use `malloc()` and `free()` to allocate and release memory.

In the other memory schemes, the RTOS itself allocates a stack and heap. For these schemes, the stack defined in the BSP linker file is used only by the `main()` function and the functions it calls.

`HEAP_ALLOCATION_TYPE3` is the default heap memory management scheme enabled in *FreeRTOSConfig.h*. It is suggested for use with the applications that rely on `malloc()` and `free()`
implementations provided by the standard C library (for example, GCC + newlib).

To avoid a possible corruption of the heap storage pool due to simultaneous calls to malloc from multiple threads, it is strongly recommended to enable the [configUSE_NEWLIB_REENTRANT](#configuse_newlib_reentrant) configuration parameter, and include the [CLib FreeRTOS support library](https://github.com/Infineon/clib-support)
in the application.

**Note:** In ModusToolbox&trade; software, `clib-support` has been added as a dependency to this FreeRTOS library so that you do not need to import it separately.

For more details on heap memory management schemes, see
the [FreeRTOS documentation](http://www.freertos.org/a00111.html).


### `configTOTAL_HEAP_SIZE`

This parameter specifies the total amount of RAM available for the FreRTOS heap. This parameter ignored when the heap_3 memory scheme is used.


### `configMINIMAL_STACK_SIZE`

This parameter specifies the size of the stack used by the idle task. It is not recommended to reduce the default parameter value. Please note that when DS-RAM feature is enabled, the minimum value of this parameter should be 256.

### `configMINIMAL_SECURE_STACK_SIZE`

This parameter specifies the size of the Secure context for nonSecure task while use NSC. The minimum value of this parameter must be 256.

### `configUSE_TICKLESS_IDLE`

This parameter configures the tickless idle functionality required for low-power mode support.

The tickless mode is enabled automatically if the ModusToolbox&trade; device configurator power personality parameter "System idle power mode" is set to either "CPU Sleep" or "System Deep Sleep". The correct operation of the tickless mode depends on the `vApplicationSleep` hook provided by the application.
See the [RTOS abstraction library](https://github.com/Infineon/abstraction-rtos#freertos)
for the compatible implementation of the vApplicationSleep hook.
The [low power assistant library](https://github.com/Infineon/lpa) provides additional portable configuration layer for low-power features supported by PSoC&trade; 6 MCU devices.

See the following resources for more details on low-power support:

- [Tickless idle mode](https://www.freertos.org/low-power-tickless-rtos.html)
- [Low-power RTOS for Arm&reg; Cortex&reg;-M MCUs](https://www.freertos.org/low-power-ARM-cortex-rtos.html)
- [LPA middleware API reference guide](https://infineon.github.io/lpa/api_reference_manual/html/index.html)


### `configUSE_NEWLIB_REENTRANT`

This parameter enables the allocation of newlib reentrancy structures for each RTOS task. The system behavior is toolchain-specific.

- **GCC toolchain:** The application must provide the implementation for the required newlib hook functions: `__malloc_lock`, `__malloc_unlock`, `__env_lock`, and `__env_unlock`.
  FreeRTOS-compatible implementation is provided by the [clib-support library](https://github.com/Infineon/clib-support).

- **Arm&reg;/IAR/LLVM toolchains:** The application must provide the *reent.h* header file to adapt FreeRTOS's `configUSE_NEWLIB_REENTRANT` to work with the toolchain-specific C library. The compatible implementations are provided by the `clib-support` library.

See the following resources for more details on newlib reentrancy support:

- [FreeRTOS customization](https://www.freertos.org/a00110.html#configUSE_NEWLIB_REENTRANT)
- [newlib reentrancy](https://sourceware.org/newlib/libc.html#Reentrancy)
- [CLib FreeRTOS support library](https://github.com/Infineon/clib-support)
- [newlib and FreeRTOS configuration hints](http://www.nadler.com/embedded/newlibAndFreeRTOS.html)


### `configMAX_PRIORITIES`

This parameter specifies the maximum number of discrete priorities for RTOS tasks, which are numbered 0 to configMAX_PRIORITIES-1.

1. If timer functionality is enabled, `configTIMER_TASK_PRIORITY` parameter in `FreeRTOSConfig.h` should have value in the range of 0 to configMAX_PRIORITIES-1.

2. Any user created RTOS-tasks should have priority value from 0 to configMAX_PRIORITIES-1.

**Note:** If the task priority is defined equal to or greater than configMAX_PRIORITIES then in case of Release mode the priority will be adjusted to configMAX_PRIORITIES - 1, in case of Debug mode the application will assert.

### `configENABLE_FPU`

This parameter enables or disables the Floating Point Unit (FPU) support in the system.

## Quick Start

The [Quick Start section of the FreeRTOS Middleware API Reference Guide](https://infineon.github.io/freertos/html/index.html#section_freertos_quick_start)
describes step-by-step instructions to set up a FreeRTOS application.

## More information

- [FreeRTOS RELEASE.md](./RELEASE.md)
- [FreeRTOS API documentation](http://www.freertos.org/a00106.html)
- [FreeRTOS port documentation](https://infineon.github.io/freertos/html/index.html)
- [FreeRTOS homepage](https://www.freertos.org/index.html)
- [ModusToolbox&trade; software environment, quick start guide, documentation, and videos](https://www.infineon.com/design-resources/development-tools/sdk/modustoolbox-software)
- [Code Examples for ModusToolbox Software](https://github.com/Infineon/Code-Examples-for-ModusToolbox-Software)
- [ModusToolbox Device Configurator Tool Guide](https://www.infineon.com/ModusToolboxDeviceConfig)
- [Infineon Technologies AG](https://www.infineon.com)

---
© Cypress Semiconductor Corporation (an Infineon company), 2019-2025.
