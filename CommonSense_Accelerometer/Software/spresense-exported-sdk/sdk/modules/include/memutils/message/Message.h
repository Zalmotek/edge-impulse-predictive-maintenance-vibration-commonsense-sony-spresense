/****************************************************************************
 * modules/include/memutils/message/Message.h
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
/**
 * @defgroup memutils Memory Utility Libraries
 * @{
 */

#ifndef __SONY_APPS_INCLUDE_MEMUTILS_MESSAGE_MESSAGE_H
#define __SONY_APPS_INCLUDE_MEMUTILS_MESSAGE_MESSAGE_H

/**
 * @defgroup memutils_message Message Library (for class object send)
 * @{
 *
 * @file   Message.h
 * @brief  Message Library API
 * @author CXD5602 Media SW Team
 */

#include "memutils/common_utils/common_errcode.h"
#include "memutils/message/message_type.h"
#include "memutils/message/MsgQueBlock.h"

#define MSG_LIB_NAME  "MsgLib"
#define MSG_LIB_VER   "2.03"
#define MSG_QUE_NULL  0

/*****************************************************************
 * Types of data generated by the message queue layout generation tool
 *****************************************************************
 */
struct MsgQueDef {
  drm_t     n_drm;
  uint16_t  n_size;
  uint16_t  n_num;
  drm_t     h_drm;
  uint16_t  h_size;
  uint16_t  h_num;
  MsgCpuId  owner;
#ifdef USE_MULTI_CORE
  InterCpuLock::SpinLockId  spinlock;
#else
  uint16_t  spinlock;
#endif
}; /* MsgQueDef */

/*****************************************************************
 * Message library class
 *****************************************************************
 */

/**
 * @class MsgLib
 * @brief Message Library Class.
 */
class MsgLib {
private:
  static uint32_t num_msg_pools;
  static uint32_t msgq_top_drm;

public:

  /** The 1st initialize method on boot.
   *  This method only called once on the system.(from system processor)
   *  @return ERR_OK  : success
   *  @return ERR_STS : error, initFirst() is already executing
   */

  static err_t initFirst(uint32_t num_pools, uint32_t top_drm);

  /** The 2nd initialize method on boot.
   *  This method only called once on each processor.
   *  @return ERR_OK  : success
   *  @return ERR_STS : error, initFirst() is not executed yet
   */

  static err_t initPerCpu();

  /** The finalize method on power-off.
   *  This method only called once on the system.(from system processor)
   *  @return ERR_OK  : success
   *  @return ERR_STS : error, initFirst() is not executed yet
   */

  static err_t finalize();

  /* Get initialization state. */

  static bool isInitFirstComplete();

  /* Get initialization state of message queue. */

  static err_t isInitComplete(MsgQueId id, bool &done);

  /* Get a reference to the specified message queue block. */

  static err_t referMsgQueBlock(MsgQueId id, MsgQueBlock **que);

  /* Transmission of message packet.(task context, no parameter) */
  static err_t send(MsgQueId dest, MsgPri pri, MsgType type, MsgQueId reply);

  /* Transmission of message packet.(task context, with parameter) */
  /** Send a Object to another task.
   *  This method can send a object to dest(MsgQueId) with type(MsgType).
   *  @param[in] dest   Destination id
   *  @param[in] pri    Priority
   *  @param[in] type   Message Type
   *  @param[in] reply  Reply id
   *  @param[in] param  Object to send
   *  @return err_t error code.
   */
  template<typename T>
  static err_t send(MsgQueId dest, MsgPri pri, MsgType type, MsgQueId reply, const T& param)
    {
      FAR MsgQueBlock* que;
      err_t            err_code = ERR_OK;

      err_code = referMsgQueBlock(dest, &que);
      if (err_code == ERR_OK)
        {
          return que->send(pri, type, reply, MsgPacket::MsgFlagWaitParam, param);
        }

      return err_code;
    }

  /* Transmission of message packet.(task context, address range parameter) */
  static err_t send(MsgQueId dest, MsgPri pri, MsgType type, MsgQueId reply, const void* param, size_t param_size);

  /* Transmission of message packet.(non task context, no parameters) */
  static err_t sendIsr(MsgQueId dest, MsgPri pri, MsgType type, MsgQueId reply);

  /* Transmission of message packet.(non task context, with parameter) */
  template<typename T>
  static err_t sendIsr(MsgQueId dest, MsgPri pri, MsgType type, MsgQueId reply, const T& param)
    {
      FAR MsgQueBlock* que;
      err_t            err_code = ERR_OK;

      err_code = referMsgQueBlock(dest, &que);
      if (err_code == ERR_OK)
        {
          return que->sendIsr(pri, type, reply, param);
        }

      return err_code;
    }

  /* Transmission of message packet.(non task context, address range parameter) */
  static err_t sendIsr(MsgQueId dest, MsgPri pri, MsgType type, MsgQueId reply, const void* param, size_t param_size);

  /* Notify message reception (call this API from inter-processor communication interrupt handler) */
  static err_t notifyRecv(MsgQueId dest);

  static void dump();

}; /* class MsgLib */

#include "MsgNotify.h"    /* User implemented by processor */

/**
 * @}
 */

/**
 * @}
 */

#endif /* __SONY_APPS_INCLUDE_MEMUTILS_MESSAGE_MESSAGE_H */
