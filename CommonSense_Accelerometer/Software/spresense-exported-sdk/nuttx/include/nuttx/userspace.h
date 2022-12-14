/****************************************************************************
 * include/nuttx/userspace.h
 *
 *   Copyright (C) 2013 Gregory Nutt. All rights reserved.
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

#ifndef __INCLUDE_NUTTX_USERSPACE_H
#define __INCLUDE_NUTTX_USERSPACE_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>

#include <sys/types.h>
#include <stdint.h>
#include <signal.h>
#include <pthread.h>

#include <nuttx/arch.h>

#ifdef CONFIG_BUILD_PROTECTED

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
/* Configuration ************************************************************/
/* If CONFIG_BUILD_PROTECTED, then CONFIG_NUTTX_USERSPACE must be defined to
 * provide the address where the user-space header can be found in memory.
 */

#ifndef CONFIG_NUTTX_USERSPACE
#  error "CONFIG_NUTTX_USERSPACE is not defined"
#endif

/* Let's insist on 4-byte alignment.  This alignment may not be required
 * technically for all platforms.  However, neither is it an unreasonable
 * requirement for any platform.
 */

#if (CONFIG_NUTTX_USERSPACE & 3) != 0
#  warning "CONFIG_NUTTX_USERSPACE is not aligned to a 4-byte boundary"
#endif

/* Helper Macros ************************************************************/
/* This macro is used to access the struct userpace_s header that can be
 * found at the beginning of the user-space blob.
 */

#define USERSPACE ((FAR struct userspace_s *)CONFIG_NUTTX_USERSPACE)

/* In user space, these functions are directly callable.  In kernel space,
 * they can be called through the userspace structure.
 */

/****************************************************************************
 * Public Type Definitions
 ****************************************************************************/

struct mm_heaps_s; /* Forward reference */

 /* Every user-space blob starts with a header that provides information about
 * the blob.  The form of that header is provided by struct userspace_s.  An
 * instance of this structure is expected to reside at CONFIG_NUTTX_USERSPACE.
 */

struct userspace_s
{
  /* General memory map */

  main_t    us_entrypoint;
  uintptr_t us_textstart;
  uintptr_t us_textend;
  uintptr_t us_datasource;
  uintptr_t us_datastart;
  uintptr_t us_dataend;
  uintptr_t us_bssstart;
  uintptr_t us_bssend;
  uintptr_t us_heapend;

  /* Memory manager heap structure */

  FAR struct mm_heap_s *us_heap;

  /* Task/thread startup routines */

  CODE void (*task_startup)(main_t entrypt, int argc, FAR char *argv[])
    noreturn_function;
#ifndef CONFIG_DISABLE_PTHREAD
  CODE void (*pthread_startup)(pthread_startroutine_t entrypt,
    pthread_addr_t arg);
#endif

  /* Signal handler trampoline */

  CODE void (*signal_handler)(_sa_sigaction_t sighand, int signo,
    FAR siginfo_t *info, FAR void *ucontext);

  /* User-space work queue support */

#ifdef CONFIG_LIB_USRWORK
  CODE int (*work_usrstart)(void);
#endif
};

/****************************************************************************
 * Public Data
 ****************************************************************************/

#ifdef __cplusplus
#define EXTERN extern "C"
extern "C"
{
#else
#define EXTERN extern
#endif

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

/****************************************************************************
 * Name: task_startup
 *
 * Description:
 *   This function is the user-space, task startup function.  It is called
 *   from up_task_start() in user-mode.
 *
 * Input Parameters:
 *   entrypt - The user-space address of the task entry point
 *   argc and argv - Standard arguments for the task entry point
 *
 * Returned Value:
 *   None.  This function does not return.
 *
 ****************************************************************************/

#if defined(CONFIG_BUILD_PROTECTED) && !defined(__KERNEL__)
void task_startup(main_t entrypt, int argc, FAR char *argv[]) noreturn_function;
#endif

/****************************************************************************
 * Name: pthread_startup
 *
 * Description:
 *   This function is the user-space, pthread startup function.  It is called
 *   from up_pthread_start() in user-mode.
 *
 * Input Parameters:
 *   entrypt - The user-space address of the pthread entry point
 *   arg     - Standard argument for the pthread entry point
 *
 * Returned Value:
 *   None.  This function does not return.
 *
 ****************************************************************************/

#if defined(CONFIG_BUILD_PROTECTED) && !defined(__KERNEL__) && !defined(CONFIG_DISABLE_PTHREAD)
void pthread_startup(pthread_startroutine_t entrypt, pthread_addr_t arg);
#endif

#undef EXTERN
#ifdef __cplusplus
}
#endif

#endif /* CONFIG_BUILD_PROTECTED */
#endif /* __INCLUDE_NUTTX_USERSPACE_H */
