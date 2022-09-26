/*
 * Copyright (C) 2010-2021 Arm Limited or its affiliates. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/* ----------------------------------------------------------------------
 * Project:      CMSIS NN Library
 * Title:        arm_nn_mat_mult_kernel_q7_q15_reordered.c
 * Description:  Matrix-multiplication function for convolution with reordered columns
 *
 * $Date:        January 26, 2021
 * $Revision:    V.1.0.2
 *
 * Target Processor:  Cortex-M cores
 * -------------------------------------------------------------------- */

#include "edge-impulse-sdk/CMSIS/NN/Include/arm_nnfunctions.h"
#include "edge-impulse-sdk/CMSIS/NN/Include/arm_nnsupportfunctions.h"

/**
 * @brief Matrix-multiplication function for convolution with re-ordered input.
 *
 * @details Refer to header file for details.
 *
 */

q7_t *arm_nn_mat_mult_kernel_q7_q15_reordered(const q7_t *pA,
                                              const q15_t *pInBuffer,
                                              const uint16_t ch_im_out,
                                              const uint16_t numCol_A,
                                              const uint16_t bias_shift,
                                              const uint16_t out_shift,
                                              const q7_t *bias,
                                              q7_t *pOut)
{

#if defined(ARM_MATH_DSP)
    /* set up the second output pointers */
    q7_t *pOut2 = pOut + ch_im_out;
    int i;

    /* this loop over rows in A */
    for (i = 0; i < ch_im_out; i += 2)
    {
        /* setup pointers for B */
        const q15_t *pB = pInBuffer;
        const q15_t *pB2 = pB + numCol_A;

        /* align the second pointer for A */
        const q7_t *pA2 = pA + numCol_A;

        /* init the sum with bias */
        q31_t sum = ((q31_t)(bias[i]) << bias_shift) + NN_ROUND(out_shift);
        q31_t sum2 = ((q31_t)(bias[i]) << bias_shift) + NN_ROUND(out_shift);
        q31_t sum3 = ((q31_t)(bias[i + 1]) << bias_shift) + NN_ROUND(out_shift);
        q31_t sum4 = ((q31_t)(bias[i + 1]) << bias_shift) + NN_ROUND(out_shift);

        uint16_t colCnt = numCol_A >> 2;
        /* accumulate over the vector */
        while (colCnt)
        {
            q31_t inA11, inA12, inA21, inA22;

            q31_t inB1 = arm_nn_read_q15x2_ia(&pB);
            q31_t inB2 = arm_nn_read_q15x2_ia(&pB2);

            pA = read_and_pad_reordered(pA, &inA11, &inA12);
            pA2 = read_and_pad_reordered(pA2, &inA21, &inA22);

            sum = __SMLAD(inA11, inB1, sum);
            sum2 = __SMLAD(inA11, inB2, sum2);
            sum3 = __SMLAD(inA21, inB1, sum3);
            sum4 = __SMLAD(inA21, inB2, sum4);

            inB1 = arm_nn_read_q15x2_ia(&pB);
            inB2 = arm_nn_read_q15x2_ia(&pB2);

            sum = __SMLAD(inA12, inB1, sum);
            sum2 = __SMLAD(inA12, inB2, sum2);
            sum3 = __SMLAD(inA22, inB1, sum3);
            sum4 = __SMLAD(inA22, inB2, sum4);

            colCnt--;
        } /* while over colCnt */
        colCnt = numCol_A & 0x3;
        while (colCnt)
        {
            q7_t inA1 = *pA++;
            q15_t inB1 = *pB++;
            q7_t inA2 = *pA2++;
            q15_t inB2 = *pB2++;

            sum += inA1 * inB1;
            sum2 += inA1 * inB2;
            sum3 += inA2 * inB1;
            sum4 += inA2 * inB2;
            colCnt--;
        } /* while over colCnt */
        *pOut++ = (q7_t)__SSAT((sum >> out_shift), 8);
        *pOut++ = (q7_t)__SSAT((sum3 >> out_shift), 8);
        *pOut2++ = (q7_t)__SSAT((sum2 >> out_shift), 8);
        *pOut2++ = (q7_t)__SSAT((sum4 >> out_shift), 8);

        /* skip the row computed with A2 */
        pA += numCol_A;
    } /* for over ch_im_out */

    pOut += ch_im_out;

    /* return the new output pointer with offset */
    return pOut;
#else
    (void)pA;
    (void)pInBuffer;
    (void)ch_im_out;
    (void)numCol_A;
    (void)bias_shift;
    (void)out_shift;
    (void)bias;
    (void)pOut;
    /* To be completed */
    return NULL;
#endif /* ARM_MATH_DSP */
}
