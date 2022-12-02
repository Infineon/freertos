# FreeRTOS for Infineon MCUs

## What's included?

- FreeRTOS kernel 10.4.3
- Ports for GCC, IAR, Arm&reg; compilers and Cortex&reg;-M0 (CM0), Cortex&reg;-M0+ (CM0P), Cortex&reg;-M4 (CM4), Cortex&reg;-M33 (CM33), Cortex&reg;-R4 (CR4) and Cortex&reg;-M7 (CM7) CPUs
- *FreeRTOSConfig.h* template with the recommended configuration options
- Cortex&reg;-R4 (CR4) is currently supported only on GCC_ARM

See [README.md](./README.md) for a complete description of FreeRTOS.


## Changelog

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
| ModusToolbox&trade; software environment                       | 3.0     |
| Peripheral driver library (`mtb-pdl-cat1`)                | 3.0.0   |
| GCC compiler                                            | 10.3.1  |
| IAR compiler                                            | 9.30    |
| Arm&reg; compiler 6                                          | 6.16    |


## Known issues

Usage of the Arm&reg; compiler 6 with enabled LTO (link-time optimization) option can result in an incorrect behavior of the application in some scenarios. It is recommended to avoid enabling LTO compiler/linker flags (`-flto`)
when using Arm&reg; Compiler 6 with FreeRTOS applications.

### More information

- [FreeRTOS README.md](./README.md)
- [FreeRTOS API documentation](http://www.freertos.org/a00106.html)
- [FreeRTOS homepage](https://www.freertos.org/index.html)
- [ModusToolbox&trade; software environment, quick start guide, documentation, and videos](https://www.cypress.com/products/modustoolbox-software-environment)
- [PSoC&trade; 6 technical reference manuals](https://www.cypress.com/search/all?f%5B0%5D=meta_type%3Atechnical_documents&f%5B1%5D=resource_meta_type%3A583&f%5B2%5D=field_related_products%3A114026)
- [PSoC&trade; 6 datasheets](https://www.cypress.com/search/all?f%5B0%5D=meta_type%3Atechnical_documents&f%5B1%5D=field_related_products%3A114026&f%5B2%5D=resource_meta_type%3A575)
- [Cypress semiconductor](http://www.cypress.com) website
---
© Cypress Semiconductor Corporation, 2019-2021
