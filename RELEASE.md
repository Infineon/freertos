# FreeRTOS for Infineon MCUs

## What's included?

- FreeRTOS kernel 10.6.2
- Ports for GCC, IAR, LLVM, Arm&reg; compilers 
- Cores: Cortex&reg;-M0 (CM0), Cortex&reg;-M0+ (CM0P), Cortex&reg;-M4 (CM4), Cortex&reg;-M33 (CM33), Cortex&reg;-M55 (CM55), Cortex&reg;-R4 (CR4) and Cortex&reg;-M7 (CM7)
- *FreeRTOSConfig.h* template with the recommended configuration options
- Cortex&reg;-R4 (CR4) is currently supported only on GCC_ARM
- Add Arm&reg; TrustZone NSC functionality support for CM33 core.

See [README.md](./README.md) for a complete description of FreeRTOS.


## Changelog

### v10.6.2

- Updated to upstream FreeRTOS kernel version 10.6.2
- Enabled support for LLVM_ARM toolchain and Updated the *FreeRTOSConfig.h* template to enable LLVM configuration option required by RTOS support libraries:

   ```
   #define configUSE_PICOLIBC_TLS                  1
   ```
- Enabled support for CM33 TrustZone NSC functionality.
   
### v10.5.004

- Updated CM33 GCC_ARM port file to fix build issues with -flto linker option.

### v10.5.003

- Updated the Cortex&reg;-M33 *FreeRTOSConfig.h* template to support FreeRTOS on secure devices.
- Setting Cortex&reg;-M33 Deep Sleep Latency Configuration to 2 ticks to allow vApplicationSleep function to handle different kind of sleep modes, like simple MCU sleep without tickless.
- Added support for D-cache enablement on XMC7200 devices (CM7).

### v10.5.002

- Added support of DSRAM feature for 20829B0 kit (CM33) on IAR compiler

### v10.5.001

- Added support for 20829B0 kit (CM33) on IAR compiler

### v10.5.0

- Updated to upstream FreeRTOS kernel version 10.5.0

### v10.4.306

- Modified the FreeRTOSConfig.h of CR4 port to fix the issue with debugger
- Fixed MISRA warnings
- DSRAM support added for CM33 ARM port

### v10.4.305

- Added functions to save and restore context before sleep for CM33

### v10.4.304

- Created a common section for frequently called freertos functions

### v10.4.303

- Added Cortex&reg;-M7 (CM7) support based on upstream FreeRTOS kernel version 10.4.3 LTS Patch 2

### v10.4.302

- Updated to upstream FreeRTOS kernel version 10.4.3 LTS Patch 2

### v10.4.4

- Added Cortex&reg;-R4 (CR4) support based on upstream FreeRTOS kernel version 10.4.3 LTS Patch 1

### v10.4.3

- Updated to match the upstream FreeRTOS kernel version 10.4.3
- Updated to match the upstream FreeRTOS kernel version 10.4.3 LTS Patch 1

### v10.3.1

- Updated to match the upstream FreeRTOS kernel version 10.3.1
- Added ports for Cortex&reg;-M0 and Cortex&reg;-M0+ CPUs.
- Added ports for the ARM Compiler 6 (based on GCC port files).
- Updated the *FreeRTOSConfig.h* template to enable configuration options required by RTOS support libraries:

   ```
   #define configUSE_MUTEXES                       1
   #define configUSE_RECURSIVE_MUTEXES             1
   #define configUSE_COUNTING_SEMAPHORES           1
   #define configSUPPORT_STATIC_ALLOCATION         1
   #define configUSE_TRACE_FACILITY                1
   #define INCLUDE_xTimerPendFunctionCall          1
   ```

- Enabled allocation of thread reentrancy structures for each task:

   ```
   #define configUSE_NEWLIB_REENTRANT              1
   ```

- Automatically enable low-power tickless mode based on ModusToolbox&trade; device configurator selection:

   ```
   #if defined(CY_CFG_PWR_SYS_IDLE_MODE) && \
      ((CY_CFG_PWR_SYS_IDLE_MODE == CY_CFG_PWR_MODE_SLEEP) || \
      (CY_CFG_PWR_SYS_IDLE_MODE == CY_CFG_PWR_MODE_DEEPSLEEP))
      extern void vApplicationSleep( uint32_t xExpectedIdleTime );
      #define portSUPPRESS_TICKS_AND_SLEEP( xIdleTime ) vApplicationSleep( xIdleTime )
      #define configUSE_TICKLESS_IDLE                 2
      #else
      #define configUSE_TICKLESS_IDLE                 0
      #endif
   ```

- Automatically configure the deep sleep latency based on ModusToolbox&trade; device configurator selection:

   ```
   #if( CY_CFG_PWR_DEEPSLEEP_LATENCY > 0 )
        #define configEXPECTED_IDLE_TIME_BEFORE_SLEEP   CY_CFG_PWR_DEEPSLEEP_LATENCY
        #endif
   ```

- Added dependency on the [CLib FreeRTOS support](https://github.com/Infineon/clib-support) and [RTOS abstraction](https://github.com/Infineon/abstraction-rtos) libraries.


## Supported software and tools

This version of FreeRTOS was validated for compatibility with the following software and tools:

| Software and tools                                      | Version |
| :---                                                    | :----:  |
| ModusToolbox&trade; software environment                | 3.6     |
| GCC compiler                                            | 14.2.1  |
| IAR compiler                                            | 9.50.2  |
| Arm&reg; compiler 6                                     | 6.22    |
| LLVM ARM Compiler                                       | 19.1.5  |

## Known Issues
| Problem | Workaround |
| ------- | ---------- |
| In the ModusToolbox&trade Eclipse IDE, the call stack does not show the debugging of multithreaded applications with FreeRTOS | <p> To see the call stack properly, do one of the following options:</p> <p>1. Disable the compiler optimization. To disable the compiler optimization on ARMC6 & GCC_ARM toolchains, add <b>CFLAGS+=-O0</b> to the application Makefile</p><p>2. Add the fno-omit-frame-pointer to the application Makefile by adding <b>CFLAGS+=-fno-omit-frame-pointer</b>.</p><p> 3. The above two options increase the code size. If there is a memory constraint, use the Visual Studio Code IDE for debugging. To use, Visual Studio Code for ModusToolbox™ Eclipse IDE, refer to the section user guide section at [ModusToolbox&trade; Software page](https://www.infineon.com/cms/en/design-support/tools/sdk/modustoolbox-software/?gad_source=1&gclid=CjwKCAjw17qvBhBrEiwA1rU9w5xTYwo11nLYqYNEXBi1i4tmOyH5sadgBM1QXq5VSh7eOgmbzeGi-hoCtacQAvD_BwE&gclsrc=aw.ds). </p> |
| The usage of the Arm® compiler 6 with enabled LTO | enabled LTO (link-time optimization) option can result in the application incorrect behavior in some scenarios. Recommended: avoid enabling LTO compiler/linker flags (`-flto`) when using Arm® Compiler 6 with FreeRTOS applications.|

### More information

- [FreeRTOS README.md](./README.md)
- [FreeRTOS API documentation](http://www.freertos.org/a00106.html)
- [FreeRTOS port documentation](https://infineon.github.io/freertos/html/index.html)
- [FreeRTOS homepage](https://www.freertos.org/index.html)
- [ModusToolbox&trade; software environment, quick start guide, documentation, and videos](https://www.infineon.com/design-resources/development-tools/sdk/modustoolbox-software)
- [Code Examples for ModusToolbox Software](https://github.com/Infineon/Code-Examples-for-ModusToolbox-Software)
- [ModusToolbox Device Configurator Tool Guide](https://www.infineon.com/ModusToolboxDeviceConfig)
- [Infineon Technologies AG](https://www.infineon.com)

---
© Cypress Semiconductor Corporation (an Infineon company), 2019-2025.

