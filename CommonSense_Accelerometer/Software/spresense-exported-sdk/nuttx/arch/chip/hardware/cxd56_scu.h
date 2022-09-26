/****************************************************************************
 * arch/arm/src/cxd56xx/hardware/cxd56_scu.h
 *
 *   Copyright 2018 Sony Semiconductor Solutions Corporation
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
 * 3. Neither the name of Sony Semiconductor Solutions Corporation nor
 *    the names of its contributors may be used to endorse or promote
 *    products derived from this software without specific prior written
 *    permission.
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

#ifndef __ARCH_ARM_SRC_CXD56XX_HARDWARE_CXD56_SCU_H
#define __ARCH_ARM_SRC_CXD56XX_HARDWARE_CXD56_SCU_H

#define SCU_DEBUG_I2C0			(CXD56_SCU_BASE + 0x0018)
#define SCU_DEBUG_I2C1			(CXD56_SCU_BASE + 0x001c)
#define SCU_SEQ_ENABLE_ALL		(CXD56_SCU_BASE + 0x0020)
#define SCU_SEQ_ACCESS_INHIBIT		(CXD56_SCU_BASE + 0x0024)
#define SCU_START_CTRL_COMMON		(CXD56_SCU_BASE + 0x0028)
#define SCU_START_MODE0			(CXD56_SCU_BASE + 0x002c)
#define SCU_START_MODE1			(CXD56_SCU_BASE + 0x0030)
#define SCU_START_INTERVAL3_0		(CXD56_SCU_BASE + 0x0034)
#define SCU_START_INTERVAL7_4		(CXD56_SCU_BASE + 0x0038)
#define SCU_START_INTERVAL9_8		(CXD56_SCU_BASE + 0x003c)
#define SCU_START_PHASE1_0		(CXD56_SCU_BASE + 0x0040)
#define SCU_START_PHASE3_2		(CXD56_SCU_BASE + 0x0044)
#define SCU_START_PHASE5_4		(CXD56_SCU_BASE + 0x0048)
#define SCU_START_PHASE7_6		(CXD56_SCU_BASE + 0x004c)
#define SCU_START_PHASE9_8		(CXD56_SCU_BASE + 0x0050)
#define SCU_SINGLE_EXE			(CXD56_SCU_BASE + 0x0054)
#define SCU_START_CTRL_STT0		(CXD56_SCU_BASE + 0x0058)
#define SCU_START_CTRL_STT1		(CXD56_SCU_BASE + 0x005c)
#define SCU_DEBUG_CTRL			(CXD56_SCU_BASE + 0x0060)
#define SCU_OFST_GAIN_EN		(CXD56_SCU_BASE + 0x0070)
#define SCU_UNSIGNED_TO_SIGNED		(CXD56_SCU_BASE + 0x0074)
#define SCU_DEC_CLR			(CXD56_SCU_BASE + 0x0078)
#define SCU_MATHFUNC_CLR		(CXD56_SCU_BASE + 0x007c)
#define SCU_EVENT_STT			(CXD56_SCU_BASE + 0x0080)
#define SCU_DECIMATION_PARAM0		(CXD56_SCU_BASE + 0x0084)
#define SCU_DECIMATION_PARAM1		(CXD56_SCU_BASE + 0x0088)
#define SCU_MATHFUNC_SEL		(CXD56_SCU_BASE + 0x008c)
#define SCU_MATHFUNC_POS0		(CXD56_SCU_BASE + 0x0090)
#define SCU_MATHFUNC_POS1		(CXD56_SCU_BASE + 0x0094)
#define SCU_MATHFUNC_POS2		(CXD56_SCU_BASE + 0x0098)
#define SCU_MATHFUNC_POS(n)		(SCU_MATHFUNC_POS0 + (n * 4))
#define SCU_MATHFUNC_PARAM_0_0		(CXD56_SCU_BASE + 0x00a0)
#define SCU_MATHFUNC_PARAM_C0_0_0_MSB	(CXD56_SCU_BASE + 0x00a4)
#define SCU_MATHFUNC_PARAM_C0_0_0_LSB	(CXD56_SCU_BASE + 0x00a8)
#define SCU_MATHFUNC_PARAM_C1_0_0_MSB	(CXD56_SCU_BASE + 0x00b0)
#define SCU_MATHFUNC_PARAM_C1_0_0_LSB	(CXD56_SCU_BASE + 0x00b4)
#define SCU_MATHFUNC_PARAM_C2_0_0_MSB	(CXD56_SCU_BASE + 0x00b8)
#define SCU_MATHFUNC_PARAM_C2_0_0_LSB	(CXD56_SCU_BASE + 0x00bc)
#define SCU_MATHFUNC_PARAM_C3_0_0_MSB	(CXD56_SCU_BASE + 0x00d0)
#define SCU_MATHFUNC_PARAM_C3_0_0_LSB	(CXD56_SCU_BASE + 0x00d4)
#define SCU_MATHFUNC_PARAM_C4_0_0_MSB	(CXD56_SCU_BASE + 0x00d8)
#define SCU_MATHFUNC_PARAM_C4_0_0_LSB	(CXD56_SCU_BASE + 0x00dc)
#define SCU_MATHFUNC_PARAM_0_1		(CXD56_SCU_BASE + 0x00e0)
#define SCU_MATHFUNC_PARAM_C0_0_1_MSB	(CXD56_SCU_BASE + 0x00e4)
#define SCU_MATHFUNC_PARAM_C0_0_1_LSB	(CXD56_SCU_BASE + 0x00e8)
#define SCU_MATHFUNC_PARAM_C1_0_1_MSB	(CXD56_SCU_BASE + 0x00ec)
#define SCU_MATHFUNC_PARAM_C1_0_1_LSB	(CXD56_SCU_BASE + 0x00f0)
#define SCU_MATHFUNC_PARAM_C2_0_1_MSB	(CXD56_SCU_BASE + 0x00f4)
#define SCU_MATHFUNC_PARAM_C2_0_1_LSB	(CXD56_SCU_BASE + 0x00f8)
#define SCU_MATHFUNC_PARAM_C3_0_1_MSB	(CXD56_SCU_BASE + 0x00fc)
#define SCU_MATHFUNC_PARAM_C3_0_1_LSB	(CXD56_SCU_BASE + 0x0100)
#define SCU_MATHFUNC_PARAM_C4_0_1_MSB	(CXD56_SCU_BASE + 0x0104)
#define SCU_MATHFUNC_PARAM_C4_0_1_LSB	(CXD56_SCU_BASE + 0x0108)
#define SCU_MATHFUNC_PARAM_1_0		(CXD56_SCU_BASE + 0x010c)
#define SCU_MATHFUNC_PARAM_C0_1_0_MSB	(CXD56_SCU_BASE + 0x0110)
#define SCU_MATHFUNC_PARAM_C0_1_0_LSB	(CXD56_SCU_BASE + 0x0114)
#define SCU_MATHFUNC_PARAM_C1_1_0_MSB	(CXD56_SCU_BASE + 0x0118)
#define SCU_MATHFUNC_PARAM_C1_1_0_LSB	(CXD56_SCU_BASE + 0x011c)
#define SCU_MATHFUNC_PARAM_C2_1_0_MSB	(CXD56_SCU_BASE + 0x0120)
#define SCU_MATHFUNC_PARAM_C2_1_0_LSB	(CXD56_SCU_BASE + 0x0124)
#define SCU_MATHFUNC_PARAM_C3_1_0_MSB	(CXD56_SCU_BASE + 0x0128)
#define SCU_MATHFUNC_PARAM_C3_1_0_LSB	(CXD56_SCU_BASE + 0x012c)
#define SCU_MATHFUNC_PARAM_C4_1_0_MSB	(CXD56_SCU_BASE + 0x0130)
#define SCU_MATHFUNC_PARAM_C4_1_0_LSB	(CXD56_SCU_BASE + 0x0134)
#define SCU_MATHFUNC_PARAM_1_1		(CXD56_SCU_BASE + 0x0138)
#define SCU_MATHFUNC_PARAM_C0_1_1_MSB	(CXD56_SCU_BASE + 0x013c)
#define SCU_MATHFUNC_PARAM_C0_1_1_LSB	(CXD56_SCU_BASE + 0x0140)
#define SCU_MATHFUNC_PARAM_C1_1_1_MSB	(CXD56_SCU_BASE + 0x0144)
#define SCU_MATHFUNC_PARAM_C1_1_1_LSB	(CXD56_SCU_BASE + 0x0148)
#define SCU_MATHFUNC_PARAM_C2_1_1_MSB	(CXD56_SCU_BASE + 0x014c)
#define SCU_MATHFUNC_PARAM_C2_1_1_LSB	(CXD56_SCU_BASE + 0x0150)
#define SCU_MATHFUNC_PARAM_C3_1_1_MSB	(CXD56_SCU_BASE + 0x0154)
#define SCU_MATHFUNC_PARAM_C3_1_1_LSB	(CXD56_SCU_BASE + 0x0158)
#define SCU_MATHFUNC_PARAM_C4_1_1_MSB	(CXD56_SCU_BASE + 0x015c)
#define SCU_MATHFUNC_PARAM_C4_1_1_LSB	(CXD56_SCU_BASE + 0x0160)
#define SCU_MATHFUNC_PARAM_2_0		(CXD56_SCU_BASE + 0x0164)
#define SCU_MATHFUNC_PARAM_C0_2_0_MSB	(CXD56_SCU_BASE + 0x0168)
#define SCU_MATHFUNC_PARAM_C0_2_0_LSB	(CXD56_SCU_BASE + 0x016c)
#define SCU_MATHFUNC_PARAM_C1_2_0_MSB	(CXD56_SCU_BASE + 0x0170)
#define SCU_MATHFUNC_PARAM_C1_2_0_LSB	(CXD56_SCU_BASE + 0x0174)
#define SCU_MATHFUNC_PARAM_C2_2_0_MSB	(CXD56_SCU_BASE + 0x0178)
#define SCU_MATHFUNC_PARAM_C2_2_0_LSB	(CXD56_SCU_BASE + 0x017c)
#define SCU_MATHFUNC_PARAM_C3_2_0_MSB	(CXD56_SCU_BASE + 0x0180)
#define SCU_MATHFUNC_PARAM_C3_2_0_LSB	(CXD56_SCU_BASE + 0x0184)
#define SCU_MATHFUNC_PARAM_C4_2_0_MSB	(CXD56_SCU_BASE + 0x0188)
#define SCU_MATHFUNC_PARAM_C4_2_0_LSB	(CXD56_SCU_BASE + 0x018c)
#define SCU_MATHFUNC_PARAM_2_1		(CXD56_SCU_BASE + 0x0190)
#define SCU_MATHFUNC_PARAM_C0_2_1_MSB	(CXD56_SCU_BASE + 0x0194)
#define SCU_MATHFUNC_PARAM_C0_2_1_LSB	(CXD56_SCU_BASE + 0x0198)
#define SCU_MATHFUNC_PARAM_C1_2_1_MSB	(CXD56_SCU_BASE + 0x019c)
#define SCU_MATHFUNC_PARAM_C1_2_1_LSB	(CXD56_SCU_BASE + 0x01a0)
#define SCU_MATHFUNC_PARAM_C2_2_1_MSB	(CXD56_SCU_BASE + 0x01a4)
#define SCU_MATHFUNC_PARAM_C2_2_1_LSB	(CXD56_SCU_BASE + 0x01a8)
#define SCU_MATHFUNC_PARAM_C3_2_1_MSB	(CXD56_SCU_BASE + 0x01ac)
#define SCU_MATHFUNC_PARAM_C3_2_1_LSB	(CXD56_SCU_BASE + 0x01b0)
#define SCU_MATHFUNC_PARAM_C4_2_1_MSB	(CXD56_SCU_BASE + 0x01b4)
#define SCU_MATHFUNC_PARAM_C4_2_1_LSB	(CXD56_SCU_BASE + 0x01b8)
#define SCU_EVENT_PARAM_THRESH(n)	(SCU_EVENT_PARAM0_THRESH + ((n) * 0x10))
#define SCU_EVENT_PARAM_COUNT0(n)	(SCU_EVENT_PARAM0_COUNT0 + ((n) * 0x10))
#define SCU_EVENT_PARAM_COUNT1(n)	(SCU_EVENT_PARAM0_COUNT1 + ((n) * 0x10))
#define SCU_EVENT_INTR_ENABLE(n) 	(SCU_EVENT_PARAM0_COUNT2 + ((n) * 0x10))
#define SCU_EVENT_PARAM_DELAY_SAMPLE(n)	(SCU_EVENT_PARAM0_DELAY_SAMPLE + ((n) * 0x4))
#define SCU_EVENT_PARAM0_THRESH		(CXD56_SCU_BASE + 0x01bc)
#define SCU_EVENT_PARAM0_COUNT0		(CXD56_SCU_BASE + 0x01c0)
#define SCU_EVENT_PARAM0_COUNT1		(CXD56_SCU_BASE + 0x01c4)
#define SCU_EVENT_PARAM0_COUNT2		(CXD56_SCU_BASE + 0x01c8)
#define SCU_EVENT_PARAM1_THRESH		(CXD56_SCU_BASE + 0x01cc)
#define SCU_EVENT_PARAM1_COUNT0		(CXD56_SCU_BASE + 0x01d0)
#define SCU_EVENT_PARAM1_COUNT1		(CXD56_SCU_BASE + 0x01d4)
#define SCU_EVENT_PARAM1_COUNT2		(CXD56_SCU_BASE + 0x01d8)
#define SCU_EVENT_PARAM2_THRESH		(CXD56_SCU_BASE + 0x01dc)
#define SCU_EVENT_PARAM2_COUNT0		(CXD56_SCU_BASE + 0x01e0)
#define SCU_EVENT_PARAM2_COUNT1		(CXD56_SCU_BASE + 0x01e4)
#define SCU_EVENT_PARAM2_COUNT2		(CXD56_SCU_BASE + 0x01e8)
#define SCU_EVENT_PARAM0_DELAY_SAMPLE	(CXD56_SCU_BASE + 0x01ec)
#define SCU_EVENT_PARAM1_DELAY_SAMPLE	(CXD56_SCU_BASE + 0x01f0)
#define SCU_EVENT_PARAM2_DELAY_SAMPLE	(CXD56_SCU_BASE + 0x01f4)
#define SCU_EVENT_TIMESTAMP0_R_MSB	(CXD56_SCU_BASE + 0x0200)
#define SCU_EVENT_TIMESTAMP0_R_LSB	(CXD56_SCU_BASE + 0x0204)
#define SCU_EVENT_TIMESTAMP1_R_MSB	(CXD56_SCU_BASE + 0x0208)
#define SCU_EVENT_TIMESTAMP1_R_LSB	(CXD56_SCU_BASE + 0x020c)
#define SCU_EVENT_TIMESTAMP2_R_MSB	(CXD56_SCU_BASE + 0x0210)
#define SCU_EVENT_TIMESTAMP2_R_LSB	(CXD56_SCU_BASE + 0x0214)
#define SCU_EVENT_TIMESTAMP0_F_MSB	(CXD56_SCU_BASE + 0x0218)
#define SCU_EVENT_TIMESTAMP0_F_LSB	(CXD56_SCU_BASE + 0x021c)
#define SCU_EVENT_TIMESTAMP1_F_MSB	(CXD56_SCU_BASE + 0x0220)
#define SCU_EVENT_TIMESTAMP1_F_LSB	(CXD56_SCU_BASE + 0x0224)
#define SCU_EVENT_TIMESTAMP2_F_MSB	(CXD56_SCU_BASE + 0x0228)
#define SCU_EVENT_TIMESTAMP2_F_LSB	(CXD56_SCU_BASE + 0x022c)
#define SCU_FIFO_WRITE_CTRL		(CXD56_SCU_BASE + 0x0230)
#define SCU_DMA0_SEL			(CXD56_SCU_BASE + 0x0300)
#define SCU_INT_ENABLE_MAIN		(CXD56_SCU_BASE + 0x0400)
#define SCU_INT_DISABLE_MAIN		(CXD56_SCU_BASE + 0x0404)
#define SCU_INT_CLEAR_MAIN		(CXD56_SCU_BASE + 0x0408)
#define SCU_LEVEL_SEL_MAIN		(CXD56_SCU_BASE + 0x040c)
#define SCU_INT_RAW_STT_MAIN		(CXD56_SCU_BASE + 0x0410)
#define SCU_INT_MASKED_STT_MAIN		(CXD56_SCU_BASE + 0x0414)
#define SCU_INT_ENABLE_ERR_0		(CXD56_SCU_BASE + 0x0420)
#define SCU_INT_DISABLE_ERR_0		(CXD56_SCU_BASE + 0x0424)
#define SCU_INT_CLEAR_ERR_0		(CXD56_SCU_BASE + 0x0428)
#define SCU_INT_RAW_STT_ERR_0		(CXD56_SCU_BASE + 0x042c)
#define SCU_INT_MASKED_STT_ERR_0	(CXD56_SCU_BASE + 0x0430)
#define SCU_INT_ENABLE_ERR_1		(CXD56_SCU_BASE + 0x0440)
#define SCU_INT_DISABLE_ERR_1		(CXD56_SCU_BASE + 0x0444)
#define SCU_INT_CLEAR_ERR_1		(CXD56_SCU_BASE + 0x0448)
#define SCU_INT_RAW_STT_ERR_1		(CXD56_SCU_BASE + 0x044c)
#define SCU_INT_MASKED_STT_ERR_1	(CXD56_SCU_BASE + 0x0450)
#define SCU_INT_ENABLE_ERR_2		(CXD56_SCU_BASE + 0x0460)
#define SCU_INT_DISABLE_ERR_2		(CXD56_SCU_BASE + 0x0464)
#define SCU_INT_CLEAR_ERR_2		(CXD56_SCU_BASE + 0x0468)
#define SCU_INT_RAW_STT_ERR_2		(CXD56_SCU_BASE + 0x046c)
#define SCU_INT_MASKED_STT_ERR_2	(CXD56_SCU_BASE + 0x0470)
#define SCU_RAM_TEST			(CXD56_SCU_BASE + 0x0500)
#define SCU_POWER			(CXD56_SCU_BASE + 0x0510)
#define SCU_INT_ENABLE_MAIN_AD		(CXD56_SCU_BASE + 0x0520)
#define SCU_INT_DISABLE_MAIN_AD		(CXD56_SCU_BASE + 0x0524)
#define SCU_INT_CLEAR_MAIN_AD		(CXD56_SCU_BASE + 0x0528)
#define SCU_INT_LEVEL_SEL_MAIN_AD	(CXD56_SCU_BASE + 0x052c)
#define SCU_INT_RAW_STT_MAIN_AD		(CXD56_SCU_BASE + 0x0530)
#define SCU_INT_MASKED_STT_MAIN_AD	(CXD56_SCU_BASE + 0x0534)

#endif /* __ARCH_ARM_SRC_CXD56XX_HARDWARE_CXD56_SCU_H */
