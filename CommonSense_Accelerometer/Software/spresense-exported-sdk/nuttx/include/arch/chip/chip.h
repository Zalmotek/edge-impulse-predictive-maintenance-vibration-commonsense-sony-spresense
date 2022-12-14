/*****************************************************************************
 * arch/arm/include/cxd56xx/chip.h
 *
 *   Copyright 2018 Sony Semiconductor Solutions Corporation
 *
 *   Copyright (C) 2012-2013 Gregory Nutt. All rights reserved.
 *   Author: Gregory Nutt <gnutt@nuttx.org>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name NuttX nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/

#ifndef __ARCH_ARM_INCLUDE_CXD56XX_CHIP_H
#define __ARCH_ARM_INCLUDE_CXD56XX_CHIP_H

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#define CXD56M4_SYSH_PRIORITY_MIN     0xe0 /* All bits[7:5] set is minimum priority */
#define CXD56M4_SYSH_PRIORITY_DEFAULT 0x80 /* Midpoint is the default */
#define CXD56M4_SYSH_PRIORITY_MAX     0x00 /* Zero is maximum priority */
#define CXD56M4_SYSH_PRIORITY_STEP    0x20 /* Steps between priorities */

#define NVIC_SYSH_PRIORITY_MIN        CXD56M4_SYSH_PRIORITY_MIN
#define NVIC_SYSH_PRIORITY_DEFAULT    CXD56M4_SYSH_PRIORITY_DEFAULT
#define NVIC_SYSH_PRIORITY_MAX        CXD56M4_SYSH_PRIORITY_MAX
#define NVIC_SYSH_PRIORITY_STEP       CXD56M4_SYSH_PRIORITY_STEP

#endif /* __ARCH_ARM_INCLUDE_CXD56XX_CHIP_H */
