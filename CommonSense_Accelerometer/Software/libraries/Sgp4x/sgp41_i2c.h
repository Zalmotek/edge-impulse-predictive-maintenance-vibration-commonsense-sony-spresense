/*
 * THIS FILE IS AUTOMATICALLY GENERATED
 *
 * I2C-Generator: 0.3.0
 * Yaml Version: 0.1.0
 * Template Version: 0.7.0-62-g3d691f9
 */
/*
 * Copyright (c) 2021, Sensirion AG
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 *
 * * Neither the name of Sensirion AG nor the names of its
 *   contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef SGP41_I2C_H
#define SGP41_I2C_H

#ifdef __cplusplus
extern "C" {
#endif

#include "sensirion_config.h"

/**
 * sgp41_execute_conditioning() - This command starts the conditioning, i.e.,
 * the VOC pixel will be operated at the same temperature as it is by calling
 * the sgp41_measure_raw command while the NOx pixel will be operated at a
 * different temperature for conditioning. This command returns only the
 * measured raw signal of the VOC pixel SRAW_VOC as 2 bytes (+ 1 CRC byte).
 *
 * @param default_rh Default conditions for relative humidty.
 *
 * @param default_t Default conditions for temperature.
 *
 * @param sraw_voc u16 unsigned integer directly provides the raw signal
 * SRAW_VOC in ticks which is proportional to the logarithm of the resistance of
 * the sensing element.
 *
 * @return 0 on success, an error code otherwise
 */
int16_t sgp41_execute_conditioning(uint16_t default_rh, uint16_t default_t, uint16_t* sraw_voc);

/**
 * sgp41_measure_raw_signals() - This command starts/continues the VOC+NOx
 * measurement mode
 *
 * @param relative_humidity Leaves humidity compensation disabled by sending the
 * default value 0x8000 (50%RH) or enables humidity compensation when sending
 * the relative humidity in ticks (ticks = %RH * 65535 / 100)
 *
 * @param temperature Leaves humidity compensation disabled by sending the
 * default value 0x6666 (25 degC) or enables humidity compensation when sending
 * the temperature in ticks (ticks = (degC + 45) * 65535 / 175)
 *
 * @param sraw_voc u16 unsigned integer directly provides the raw signal
 * SRAW_VOC in ticks which is proportional to the logarithm of the resistance of
 * the sensing element.
 *
 * @param sraw_nox u16 unsigned integer directly provides the raw signal
 * SRAW_NOX in ticks which is proportional to the logarithm of the resistance of
 * the sensing element.
 *
 * @return 0 on success, an error code otherwise
 */
int16_t sgp41_measure_raw_signals(uint16_t relative_humidity, uint16_t temperature, uint16_t* sraw_voc, uint16_t* sraw_nox);

/**
 * sgp41_execute_self_test() - This command triggers the built-in self-test
 * checking for integrity of both hotplate and MOX material and returns the
 * result of this test as 2 bytes
 *
 * @param test_result 0xXX 0xYY: ignore most significant byte 0xXX. The four
 * least significant bits of the least significant byte 0xYY provide information
 * if the self-test has or has not passed for each individual pixel. All zero
 * mean all tests passed successfully. Check the datasheet for more detailed
 * information.
 *
 * @return 0 on success, an error code otherwise
 */
int16_t sgp41_execute_self_test(uint16_t* test_result);

/**
 * sgp41_turn_heater_off() - This command turns the hotplate off and stops the
 * measurement. Subsequently, the sensor enters the idle mode.
 *
 * @return 0 on success, an error code otherwise
 */
int16_t sgp41_turn_heater_off(void);

/**
 * sgp41_get_serial_number() - This command provides the decimal serial number
 * of the SGP41 chip by returning 3x2 bytes.
 *
 * @param serial_number 48-bit unique serial number
 *
 * @return 0 on success, an error code otherwise
 */
int16_t sgp41_get_serial_number(uint16_t* serial_number, uint8_t serial_number_size);

#ifdef __cplusplus
}
#endif

#endif /* SGP41_I2C_H */
