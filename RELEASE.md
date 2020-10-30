# FreeRTOS 10.3.1

### What's Included?
* FreeRTOS kernel 10.3.1
* Ports for GCC, IAR, ARM compilers and Cortex M0, Cortex M0+, Cortex M4 CPUs.
* FreeRTOSConfig.h template with the recommended configuration options.

For a complete description of FreeRTOS, refer to [README.md](./README.md).

### What Changed?
#### v10.3.1
* Updated to match the upstream FreeRTOS kernel version 10.3.1.
* Added ports for the Cortex M0 and Cortex M0+ CPUs.
* Added ports for the ARM Compiler 6 (based on GCC port files).
* Updated FreeRTOSConfig.h template to enable configuration options required by the RTOS support libraries:

        #define configUSE_MUTEXES                       1
        #define configUSE_RECURSIVE_MUTEXES             1
        #define configUSE_COUNTING_SEMAPHORES           1
        #define configSUPPORT_STATIC_ALLOCATION         1
        #define configUSE_TRACE_FACILITY                1
        #define INCLUDE_xTimerPendFunctionCall          1

* Enabled allocation of thread reentrancy structures for each task:

        #define configUSE_NEWLIB_REENTRANT              1

* Automatically enable low power tickless mode based on ModusToolbox Device Configurator selection:

        #if defined(CY_CFG_PWR_SYS_IDLE_MODE) && \
            ((CY_CFG_PWR_SYS_IDLE_MODE == CY_CFG_PWR_MODE_SLEEP) || \
             (CY_CFG_PWR_SYS_IDLE_MODE == CY_CFG_PWR_MODE_DEEPSLEEP))
        extern void vApplicationSleep( uint32_t xExpectedIdleTime );
        #define portSUPPRESS_TICKS_AND_SLEEP( xIdleTime ) vApplicationSleep( xIdleTime )
        #define configUSE_TICKLESS_IDLE                 2
        #else
        #define configUSE_TICKLESS_IDLE                 0
        #endif

* Automatically configure the deep sleep latency based on ModusToolbox Device Configurator selection:

        #if( CY_CFG_PWR_DEEPSLEEP_LATENCY > 0 )
        #define configEXPECTED_IDLE_TIME_BEFORE_SLEEP   CY_CFG_PWR_DEEPSLEEP_LATENCY
        #endif

* Added dependency on the [CLib FreeRTOS Support](https://github.com/cypresssemiconductorco/clib-support) and [RTOS Abstraction](https://github.com/cypresssemiconductorco/abstraction-rtos) libraries.

### Supported Software and Tools
This version of FreeRTOS was validated for compatibility with the following Software and Tools:

| Software and Tools                                      | Version |
| :---                                                    | :----:  |
| ModusToolbox Software Environment                       | 2.2     |
| Peripheral Driver Library (mtb-pdl-cat1)                | 2.0.0   |
| GCC Compiler                                            | 9.3.1   |
| IAR Compiler                                            | 8.42.2  |
| ARM Compiler 6                                          | 6.13    |

### Known Issues
Usage of the ARM Compiler 6 with enabled LTO (Link Time Optimization) option
can result in an incorrect behavior of the application in some scenarios.
It is recommended to avoid enabling LTO compiler/linker flags (`-flto`)
when using ARM Compiler 6 with FreeRTOS applications.

### More information
The following resources contain more information:
* [FreeRTOS README.md](./README.md)
* [FreeRTOS API Documentation](http://www.freertos.org/a00106.html)
* [FreeRTOS Home Page](https://www.freertos.org/index.html)
* [ModusToolbox Software Environment, Quick Start Guide, Documentation, and Videos](https://www.cypress.com/products/modustoolbox-software-environment)
* [PSoC 6 Technical Reference Manuals](https://www.cypress.com/search/all?f%5B0%5D=meta_type%3Atechnical_documents&f%5B1%5D=resource_meta_type%3A583&f%5B2%5D=field_related_products%3A114026)
* [PSoC 6 Programmable System-on-Chip datasheets](https://www.cypress.com/search/all?f%5B0%5D=meta_type%3Atechnical_documents&f%5B1%5D=field_related_products%3A114026&f%5B2%5D=resource_meta_type%3A575)
* [Cypress Semiconductor](http://www.cypress.com)
---
© Cypress Semiconductor Corporation, 2019-2020.
