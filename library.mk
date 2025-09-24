################################################################################
# \file library.mk
#
# \brief
# Makefile to identify the presence of MTB_HAL
#
################################################################################
# \copyright
# Copyright 2022-2024 Cypress Semiconductor Corporation (an Infineon company) or
# an affiliate of Cypress Semiconductor Corporation
#
# SPDX-License-Identifier: Apache-2.0
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
################################################################################

# TrustZone flow
ifneq (,$(filter $(COMPONENTS),FREERTOS_TZ))

  # TZ Secure side
  ifneq (,$(filter $(COMPONENTS), SECURE_DEVICE))
    CY_IGNORE+= $(SEARCH_freertos)/Source 
    CY_IGNORE+= $(SEARCH_abstraction-rtos)
    CY_IGNORE+= $(SEARCH_clib-support)
  endif

else # Non TrustZone flow for all cores

  COMPONENTS+= FREERTOS_NTZ

endif


