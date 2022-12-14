/****************************************************************************
 * include/dirent.h
 *
 *   Copyright (C) 2007-2009 Gregory Nutt. All rights reserved.
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

#ifndef __INCLUDE_DIRENT_H
#define __INCLUDE_DIRENT_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>

#include <sys/types.h>
#include <stdint.h>
#include <limits.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* File type code for the d_type field in dirent structure.
 * Note that because of the simplified filesystem organization of the NuttX,
 * top-level, pseudo-file system, an inode can be BOTH a file and a directory
 */

#define DTYPE_UNKNOWN   0         /* The file type could not be determined */
#define DTYPE_FILE      (1 << 0)  /* Bit 0: Regular file */
#define DTYPE_CHR       (1 << 1)  /* Bit 1: Character device */
#define DTYPE_BLK       (1 << 2)  /* Bit 2: Block device */
#define DTYPE_DIRECTORY (1 << 3)  /* Bit 3: Directory */
#define DTYPE_LINK      (1 << 4)  /* Bit 4: Symbolic link */
#define DTYPE_FIFO      (1 << 5)  /* Bit 5: Named Pipe (FIFO) */
#define DTYPE_SOCK      (1 << 6)  /* Bit 6: UNIX domain socket */

#define DIRENT_ISFILE(dtype)      (((dtype) & DTYPE_FILE) != 0)
#define DIRENT_ISCHR(dtype)       (((dtype) & DTYPE_CHR) != 0)
#define DIRENT_ISBLK(dtype)       (((dtype) & DTYPE_BLK) != 0)
#define DIRENT_ISDIRECTORY(dtype) (((dtype) & DTYPE_DIRECTORY) != 0)
#define DIRENT_ISLINK(dtype)      (((dtype) & DTYPE_LINK) != 0)

/* The d_type field of the dirent structure is not specified by POSIX.  It
 * is a non-standard, 4.5BSD extension that is implemented by most OSs.  A
 * POSIX compliant OS may not implement the d_type field at all.  Many OS's
 * (including glibc) may use the following alternative naming for the file
 * type names:
 */

#define DT_UNKNOWN DTYPE_UNKNOWN
#define DT_FIFO    DTYPE_FIFO
#define DT_CHR     DTYPE_CHR
#define DT_DIR     DTYPE_DIRECTORY
#define DT_BLK     DTYPE_BLK
#define DT_REG     DTYPE_FILE
#define DT_LNK     DTYPE_LINK
#define DT_SOCK    DTYPE_SOCK

/****************************************************************************
 * Public Type Definitions
 ****************************************************************************/

/* The POSIX specification requires that the caller of readdir_r provide
 * storage "large enough for a dirent with the d_name member and an array
 * of char containing at least {NAME_MAX} plus one elements.
 *
 * POSIX also requires the field d_ino (type ino_t) that provides the file
 * serial number.  This functionality is not implemented in NuttX.
 */

struct dirent
{
  uint8_t  d_type;                /* Type of file */
  char     d_name[NAME_MAX + 1];  /* File name */
};

typedef void DIR;

/****************************************************************************
 * Public Data
 ****************************************************************************/

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

#undef EXTERN
#if defined(__cplusplus)
#define EXTERN extern "C"
extern "C"
{
#else
#define EXTERN extern
#endif

/* POSIX-like File System Interfaces */

int        closedir(FAR DIR *dirp);
FAR DIR   *opendir(FAR const char *path);
FAR struct dirent *readdir(FAR DIR *dirp);
int        readdir_r(FAR DIR *dirp, FAR struct dirent *entry,
                     FAR struct dirent **result);
void       rewinddir(FAR DIR *dirp);
void       seekdir(FAR DIR *dirp, off_t loc);
off_t      telldir(FAR DIR *dirp);
int        scandir(FAR const char *path, FAR struct dirent ***namelist,
                   CODE int (*filter)(FAR const struct dirent *),
                   CODE int (*compar)(FAR const struct dirent **,
                                      FAR const struct dirent **));
int        alphasort(FAR const struct dirent **a,
                     FAR const struct dirent **b);

#undef EXTERN
#if defined(__cplusplus)
}
#endif

#endif /* __INCLUDE_DIRENT_H */
