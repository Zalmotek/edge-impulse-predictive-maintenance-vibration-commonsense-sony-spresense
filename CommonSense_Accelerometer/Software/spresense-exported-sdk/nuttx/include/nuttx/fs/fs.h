/****************************************************************************
 * include/nuttx/fs/fs.h
 *
 *   Copyright (C) 2007-2009, 2011-2013, 2015-2018 Gregory Nutt. All rights
 *     reserved.
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

#ifndef __INCLUDE_NUTTX_FS_FS_H
#define __INCLUDE_NUTTX_FS_FS_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>
#include <nuttx/compiler.h>

#include <sys/types.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdbool.h>
#include <semaphore.h>

#ifdef CONFIG_FS_NAMED_SEMAPHORES
#  include <nuttx/semaphore.h>
#endif

#ifndef CONFIG_DISABLE_MQUEUE
#  include <nuttx/mqueue.h>
#endif

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* Most internal OS interfaces are not available in the user space in
 * PROTECTED and KERNEL builds.  In that context, the corresponding
 * application interfaces must be used.  The differences between the two
 * sets of interfaces are:  The internal OS interfaces (1) do not cause
 * cancellation points and (2) they do not modify the errno variable.
 *
 * This is only important when compiling libraries (libc or libnx) that are
 * used both by the OS (libkc.a and libknx.a) or by the applications
 * (libuc.a and libunx.a).  The that case, the correct interface must be
 * used for the build context.
 *
 * REVISIT:  In the flat build, the same functions must be used both by
 * the OS and by applications.  We have to use the normal user functions
 * in this case or we will fail to set the errno or fail to create the
 * cancellation point.
 *
 * The interfaces close(), creat(), read(), pread(), write(), pwrite(),
 * poll(), select(), fcntl(), and aio_suspend() are all cancellation
 * points.
 *
 * REVISIT:  These cancellation points are an issue and may cause
 * violations:  It use of these internally will cause the calling function
 * to become a cancellation points!
 */

#if !defined(CONFIG_BUILD_FLAT) && defined(__KERNEL__)
#  ifdef CONFIG_CPP_HAVE_VARARGS
#    define _NX_OPEN(p,f,...)  nx_open(p,f,##__VA_ARGS__)
#  else
#    define _NX_OPEN           nx_open
#  endif
#  define _NX_READ(f,b,s)      nx_read(f,b,s)
#  define _NX_WRITE(f,b,s)     nx_write(f,b,s)
#  define _NX_GETERRNO(r)      (-(r))
#  define _NX_SETERRNO(r)      set_errno(-(r))
#  define _NX_GETERRVAL(r)     (r)
#else
#  ifdef CONFIG_CPP_HAVE_VARARGS
#    define _NX_OPEN(p,f,...)  open(p,f,##__VA_ARGS__)
#  else
#    define _NX_OPEN           open
#  endif
#  define _NX_READ(f,b,s)      read(f,b,s)
#  define _NX_WRITE(f,b,s)     write(f,b,s)
#  define _NX_GETERRNO(r)      errno
#  define _NX_SETERRNO(r)
#  define _NX_GETERRVAL(r)     (-errno)
#endif

/* Stream flags for the fs_flags field of in struct file_struct */

#define __FS_FLAG_EOF   (1 << 0) /* EOF detected by a read operation */
#define __FS_FLAG_ERROR (1 << 1) /* Error detected by any operation */
#define __FS_FLAG_LBF   (1 << 2) /* Line buffered */
#define __FS_FLAG_UBF   (1 << 3) /* Buffer allocated by caller of setvbuf */

/* Inode i_flags values:
 *
 *   Bit 0-3: Inode type (Bit 3 indicates internal OS types)
 *   Bit 4:   Set if inode has been unlinked and is pending removal.
 */

#define FSNODEFLAG_TYPE_MASK       0x0000000f /* Isolates type field      */
#define   FSNODEFLAG_TYPE_DRIVER   0x00000000 /*   Character driver       */
#define   FSNODEFLAG_TYPE_BLOCK    0x00000001 /*   Block driver           */
#define   FSNODEFLAG_TYPE_MOUNTPT  0x00000002 /*   Mount point            */
#define FSNODEFLAG_TYPE_SPECIAL    0x00000008 /* Special OS type          */
#define   FSNODEFLAG_TYPE_NAMEDSEM 0x00000008 /*   Named semaphore        */
#define   FSNODEFLAG_TYPE_MQUEUE   0x00000009 /*   Message Queue          */
#define   FSNODEFLAG_TYPE_SHM      0x0000000a /*   Shared memory region   */
#define   FSNODEFLAG_TYPE_MTD      0x0000000b /*   Named MTD driver       */
#define   FSNODEFLAG_TYPE_SOFTLINK 0x0000000c /*   Soft link              */
#define FSNODEFLAG_DELETED         0x00000010 /* Unlinked                 */

#define INODE_IS_TYPE(i,t) \
  (((i)->i_flags & FSNODEFLAG_TYPE_MASK) == (t))
#define INODE_IS_SPECIAL(i) \
  (((i)->i_flags & FSNODEFLAG_TYPE_SPECIAL) != 0)

#define INODE_IS_DRIVER(i)    INODE_IS_TYPE(i,FSNODEFLAG_TYPE_DRIVER)
#define INODE_IS_BLOCK(i)     INODE_IS_TYPE(i,FSNODEFLAG_TYPE_BLOCK)
#define INODE_IS_MOUNTPT(i)   INODE_IS_TYPE(i,FSNODEFLAG_TYPE_MOUNTPT)
#define INODE_IS_NAMEDSEM(i)  INODE_IS_TYPE(i,FSNODEFLAG_TYPE_NAMEDSEM)
#define INODE_IS_MQUEUE(i)    INODE_IS_TYPE(i,FSNODEFLAG_TYPE_MQUEUE)
#define INODE_IS_SHM(i)       INODE_IS_TYPE(i,FSNODEFLAG_TYPE_SHM)
#define INODE_IS_MTD(i)       INODE_IS_TYPE(i,FSNODEFLAG_TYPE_MTD)
#define INODE_IS_SOFTLINK(i)  INODE_IS_TYPE(i,FSNODEFLAG_TYPE_SOFTLINK)

#define INODE_GET_TYPE(i)     ((i)->i_flags & FSNODEFLAG_TYPE_MASK)
#define INODE_SET_TYPE(i,t) \
  do \
    { \
      (i)->i_flags = ((i)->i_flags & ~FSNODEFLAG_TYPE_MASK) | (t); \
    } \
  while (0)

#define INODE_SET_DRIVER(i)   INODE_SET_TYPE(i,FSNODEFLAG_TYPE_DRIVER)
#define INODE_SET_BLOCK(i)    INODE_SET_TYPE(i,FSNODEFLAG_TYPE_BLOCK)
#define INODE_SET_MOUNTPT(i)  INODE_SET_TYPE(i,FSNODEFLAG_TYPE_MOUNTPT)
#define INODE_SET_NAMEDSEM(i) INODE_SET_TYPE(i,FSNODEFLAG_TYPE_NAMEDSEM)
#define INODE_SET_MQUEUE(i)   INODE_SET_TYPE(i,FSNODEFLAG_TYPE_MQUEUE)
#define INODE_SET_SHM(i)      INODE_SET_TYPE(i,FSNODEFLAG_TYPE_SHM)
#define INODE_SET_MTD(i)      INODE_SET_TYPE(i,FSNODEFLAG_TYPE_MTD)
#define INODE_SET_SOFTLINK(i) INODE_SET_TYPE(i,FSNODEFLAG_TYPE_SOFTLINK)

/* Mountpoint fd_flags values */

#define DIRENTFLAGS_PSEUDONODE 1

#define DIRENT_SETPSEUDONODE(f) do (f) |= DIRENTFLAGS_PSEUDONODE; while (0)
#define DIRENT_ISPSEUDONODE(f) (((f) & DIRENTFLAGS_PSEUDONODE) != 0)

/* The struct file_operations open(0) normally returns zero on success and
 * a negated errno value on failure.  There is one case, however, where
 * the open method will redirect to another driver and return a file
 * descriptor instead.
 *
 * This case is when SUSv1 pseudo-terminals are used (CONFIG_PSEUDOTERM_SUSV1=y).
 * In this case, the output is encoded and decoded using these macros in
 * order to support (a) returning file descriptor 0 (which really should
 * not happen), and (b) avoiding confusion if some other open method returns
 * a positive, non-zero value which is not a file descriptor.
 *
 *   OPEN_ISFD(r) tests if the return value from the open method is
 *     really a file descriptor.
 *   OPEN_SETFD(f) is used by an implementation of the open() method
 *     in order to encode a file descriptor in the return value.
 *   OPEN_GETFD(r) is use by the upper level open() logic to decode
 *     the file descriptor encoded in the return value.
 *
 * REVISIT: This only works for file descriptors in the in range 0-255.
 */

#define OPEN_MAGIC      0x4200
#define OPEN_MASK       0x00ff
#define OPEN_MAXFD      0x00ff

#define OPEN_ISFD(r)    (((r) & ~OPEN_MASK) == OPEN_MAGIC)
#define OPEN_SETFD(f)   ((f) | OPEN_MAGIC)
#define OPEN_GETFD(r)   ((r) & OPEN_MASK)

/****************************************************************************
 * Public Type Definitions
 ****************************************************************************/

/* Forward references */

struct file;
struct inode;
struct stat;
struct statfs;
struct pollfd;
struct fs_dirent_s;
struct mtd_dev_s;

/* This structure is provided by devices when they are registered with the
 * system.  It is used to call back to perform device specific operations.
 */

struct file_operations
{
  /* The device driver open method differs from the mountpoint open method */

  int     (*open)(FAR struct file *filep);

  /* The following methods must be identical in signature and position because
   * the struct file_operations and struct mountp_operations are treated like
   * unions.
   */

  int     (*close)(FAR struct file *filep);
  ssize_t (*read)(FAR struct file *filep, FAR char *buffer, size_t buflen);
  ssize_t (*write)(FAR struct file *filep, FAR const char *buffer, size_t buflen);
  off_t   (*seek)(FAR struct file *filep, off_t offset, int whence);
  int     (*ioctl)(FAR struct file *filep, int cmd, unsigned long arg);

  /* The two structures need not be common after this point */

  int     (*poll)(FAR struct file *filep, struct pollfd *fds, bool setup);
#ifndef CONFIG_DISABLE_PSEUDOFS_OPERATIONS
  int     (*unlink)(FAR struct inode *inode);
#endif
};

/* This structure provides information about the state of a block driver */

#ifndef CONFIG_DISABLE_MOUNTPOINT
struct geometry
{
  bool   geo_available;    /* true: The device is available */
  bool   geo_mediachanged; /* true: The media has changed since last query */
  bool   geo_writeenabled; /* true: It is okay to write to this device */
  size_t geo_nsectors;     /* Number of sectors on the device */
  size_t geo_sectorsize;   /* Size of one sector */
};

/* This structure is provided by block devices when they register with the
 * system.  It is used by file systems to perform filesystem transfers.  It
 * differs from the normal driver vtable in several ways -- most notably in
 * that it deals in struct inode vs. struct filep.
 */

struct inode;
struct block_operations
{
  int     (*open)(FAR struct inode *inode);
  int     (*close)(FAR struct inode *inode);
  ssize_t (*read)(FAR struct inode *inode, FAR unsigned char *buffer,
            size_t start_sector, unsigned int nsectors);
  ssize_t (*write)(FAR struct inode *inode, FAR const unsigned char *buffer,
            size_t start_sector, unsigned int nsectors);
  int     (*geometry)(FAR struct inode *inode, FAR struct geometry *geometry);
  int     (*ioctl)(FAR struct inode *inode, int cmd, unsigned long arg);
#ifndef CONFIG_DISABLE_PSEUDOFS_OPERATIONS
  int     (*unlink)(FAR struct inode *inode);
#endif
};

/* This structure is provided by a filesystem to describe a mount point.
 * Note that this structure differs from file_operations ONLY in the form of
 * the open method.  Once the file is opened, it can be accessed either as a
 * struct file_operations or struct mountpt_operations
 */

struct mountpt_operations
{
  /* The mountpoint open method differs from the driver open method
   * because it receives (1) the inode that contains the mountpoint
   * private data, (2) the relative path into the mountpoint, and (3)
   * information to manage privileges.
   */

  int     (*open)(FAR struct file *filep, FAR const char *relpath,
            int oflags, mode_t mode);

  /* The following methods must be identical in signature and position
   * because the struct file_operations and struct mountpt_operations are
   * treated like unions.
   */

  int     (*close)(FAR struct file *filep);
  ssize_t (*read)(FAR struct file *filep, FAR char *buffer, size_t buflen);
  ssize_t (*write)(FAR struct file *filep, FAR const char *buffer,
            size_t buflen);
  off_t   (*seek)(FAR struct file *filep, off_t offset, int whence);
  int     (*ioctl)(FAR struct file *filep, int cmd, unsigned long arg);

  /* The two structures need not be common after this point. The following
   * are extended methods needed to deal with the unique needs of mounted
   * file systems.
   *
   * Additional open-file-specific mountpoint operations:
   */

  int     (*sync)(FAR struct file *filep);
  int     (*dup)(FAR const struct file *oldp, FAR struct file *newp);
  int     (*fstat)(FAR const struct file *filep, FAR struct stat *buf);
  int     (*truncate)(FAR struct file *filep, off_t length);

  /* Directory operations */

  int     (*opendir)(FAR struct inode *mountpt, FAR const char *relpath,
            FAR struct fs_dirent_s *dir);
  int     (*closedir)(FAR struct inode *mountpt,
            FAR struct fs_dirent_s *dir);
  int     (*readdir)(FAR struct inode *mountpt,
            FAR struct fs_dirent_s *dir);
  int     (*rewinddir)(FAR struct inode *mountpt,
            FAR struct fs_dirent_s *dir);

  /* General volume-related mountpoint operations: */

  int     (*bind)(FAR struct inode *blkdriver, FAR const void *data,
            FAR void **handle);
  int     (*unbind)(FAR void *handle, FAR struct inode **blkdriver,
            unsigned int flags);
  int     (*statfs)(FAR struct inode *mountpt, FAR struct statfs *buf);

  /* Operations on paths */

  int     (*unlink)(FAR struct inode *mountpt, FAR const char *relpath);
  int     (*mkdir)(FAR struct inode *mountpt, FAR const char *relpath,
            mode_t mode);
  int     (*rmdir)(FAR struct inode *mountpt, FAR const char *relpath);
  int     (*rename)(FAR struct inode *mountpt, FAR const char *oldrelpath,
            FAR const char *newrelpath);
  int     (*stat)(FAR struct inode *mountpt, FAR const char *relpath,
            FAR struct stat *buf);

  /* NOTE:  More operations will be needed here to support:  disk usage
   * stats file stat(), file attributes, file truncation, etc.
   */
};
#endif /* CONFIG_DISABLE_MOUNTPOINT */

/* Named OS resources are also maintained by the VFS.  This includes:
 *
 *   - Named semaphores:     sem_open(), sem_close(), and sem_unlink()
 *   - POSIX Message Queues: mq_open() and mq_close()
 *   - Shared memory:        shm_open() and shm_unlink();
 *
 * These are a special case in that they do not follow quite the same
 * pattern as the other file system types in that they have operations.
 */

/* These are the various kinds of operations that can be associated with
 * an inode.
 */

union inode_ops_u
{
  FAR const struct file_operations     *i_ops;    /* Driver operations for inode */
#ifndef CONFIG_DISABLE_MOUNTPOINT
  FAR const struct block_operations    *i_bops;   /* Block driver operations */
  FAR struct mtd_dev_s                 *i_mtd;    /* MTD device driver */
  FAR const struct mountpt_operations  *i_mops;   /* Operations on a mountpoint */
#endif
#ifdef CONFIG_FS_NAMED_SEMAPHORES
  FAR struct nsem_inode_s              *i_nsem;   /* Named semaphore */
#endif
#ifndef CONFIG_DISABLE_MQUEUE
  FAR struct mqueue_inode_s            *i_mqueue; /* POSIX message queue */
#endif
#ifdef CONFIG_PSEUDOFS_SOFTLINKS
  FAR char                             *i_link;   /* Full path to link target */
#endif
};

/* This structure represents one inode in the NuttX pseudo-file system */

struct inode
{
  FAR struct inode *i_peer;     /* Link to same level inode */
  FAR struct inode *i_child;    /* Link to lower level inode */
  int16_t           i_crefs;    /* References to inode */
  uint16_t          i_flags;    /* Flags for inode */
  union inode_ops_u u;          /* Inode operations */
#ifdef CONFIG_FILE_MODE
  mode_t            i_mode;     /* Access mode flags */
#endif
  FAR void         *i_private;  /* Per inode driver private data */
  char              i_name[1];  /* Name of inode (variable) */
};

#define FSNODE_SIZE(n) (sizeof(struct inode) + (n))

/* This is the underlying representation of an open file.  A file
 * descriptor is an index into an array of such types. The type associates
 * the file descriptor to the file state and to a set of inode operations.
 */

struct file
{
  int               f_oflags;   /* Open mode flags */
  off_t             f_pos;      /* File position */
  FAR struct inode *f_inode;    /* Driver or file system interface */
  void             *f_priv;     /* Per file driver private data */
};

/* This defines a list of files indexed by the file descriptor */

struct filelist
{
  sem_t   fl_sem;               /* Manage access to the file list */
  struct file fl_files[CONFIG_NFILE_DESCRIPTORS];
};

/* The following structure defines the list of files used for standard C I/O.
 * Note that NuttX can support the standard C APIs with or without buffering
 *
 * When buffering is used, the following describes the usage of the I/O buffer.
 * The buffer can be used for reading or writing -- but not both at the same time.
 * An fflush is implied between each change in direction of access.
 *
 * The field fs_bufread determines whether the buffer is being used for reading or
 * for writing as follows:
 *
 *              BUFFER
 *     +----------------------+ <- fs_bufstart Points to the beginning of the buffer.
 *     | WR: Buffered data    |                WR: Start of buffered write data.
 *     | RD: Already read     |                RD: Start of already read data.
 *     +----------------------+
 *     | WR: Available buffer | <- fs_bufpos   Points to next byte:
 *     | RD: Read-ahead data  |                WR: End+1 of buffered write data.
 *     |                      |                RD: Points to next char to return
 *     +----------------------+
 *     | WR: Available        | <- fs_bufread  Top+1 of buffered read data
 *     | RD: Available        |                WR: bufstart buffer used for writing.
 *     |                      |                RD: Pointer to last buffered read char+1
 *     +----------------------+
 *                              <- fs_bufend   Points to the end of the buffer+1
 */

#if CONFIG_NFILE_STREAMS > 0
struct file_struct
{
  int                fs_fd;        /* File descriptor associated with stream */
#ifndef CONFIG_STDIO_DISABLE_BUFFERING
  sem_t              fs_sem;       /* For thread safety */
  pid_t              fs_holder;    /* Holder of sem */
  int                fs_counts;    /* Number of times sem is held */
  FAR unsigned char *fs_bufstart;  /* Pointer to start of buffer */
  FAR unsigned char *fs_bufend;    /* Pointer to 1 past end of buffer */
  FAR unsigned char *fs_bufpos;    /* Current position in buffer */
  FAR unsigned char *fs_bufread;   /* Pointer to 1 past last buffered read char. */
#endif
  uint16_t           fs_oflags;    /* Open mode flags */
  uint8_t            fs_flags;     /* Stream flags */
#if CONFIG_NUNGET_CHARS > 0
  uint8_t            fs_nungotten; /* The number of characters buffered for ungetc */
  unsigned char      fs_ungotten[CONFIG_NUNGET_CHARS];
#endif
};

struct streamlist
{
  sem_t               sl_sem;   /* For thread safety */
  struct file_struct sl_streams[CONFIG_NFILE_STREAMS];
};
#endif /* CONFIG_NFILE_STREAMS */

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

/****************************************************************************
 * Name: fs_initialize
 *
 * Description:
 *   This is called from the OS initialization logic to configure the file
 *   system.
 *
 ****************************************************************************/

void fs_initialize(void);

/****************************************************************************
 * Name: register_driver
 *
 * Description:
 *   Register a character driver inode the pseudo file system.
 *
 * Input Parameters:
 *   path - The path to the inode to create
 *   fops - The file operations structure
 *   mode - Access privileges (not used)
 *   priv - Private, user data that will be associated with the inode.
 *
 * Returned Value:
 *   Zero on success (with the inode point in 'inode'); A negated errno
 *   value is returned on a failure (all error values returned by
 *   inode_reserve):
 *
 *   EINVAL - 'path' is invalid for this operation
 *   EEXIST - An inode already exists at 'path'
 *   ENOMEM - Failed to allocate in-memory resources for the operation
 *
 ****************************************************************************/

int register_driver(FAR const char *path,
                    FAR const struct file_operations *fops, mode_t mode,
                    FAR void *priv);

/****************************************************************************
 * Name: register_blockdriver
 *
 * Description:
 *   Register a block driver inode the pseudo file system.
 *
 * Input Parameters:
 *   path - The path to the inode to create
 *   bops - The block driver operations structure
 *   mode - Access privileges (not used)
 *   priv - Private, user data that will be associated with the inode.
 *
 * Returned Value:
 *   Zero on success (with the inode point in 'inode'); A negated errno
 *   value is returned on a failure (all error values returned by
 *   inode_reserve):
 *
 *   EINVAL - 'path' is invalid for this operation
 *   EEXIST - An inode already exists at 'path'
 *   ENOMEM - Failed to allocate in-memory resources for the operation
 *
 ****************************************************************************/

#ifndef CONFIG_DISABLE_MOUNTPOINT
int register_blockdriver(FAR const char *path,
                         FAR const struct block_operations *bops,
                         mode_t mode, FAR void *priv);
#endif

/****************************************************************************
 * Name: register_blockpartition
 *
 * Description:
 *   Register a block partition driver inode the pseudo file system.
 *
 * Input Parameters:
 *   partition   - The path to the partition inode
 *   parent      - The path to the parent inode
 *   firstsector - The offset in sectors to the partition
 *   nsectors    - The number of sectors in the partition
 *
 * Returned Value:
 *   Zero on success (with the inode point in 'inode'); A negated errno
 *   value is returned on a failure (all error values returned by
 *   inode_reserve):
 *
 *   EINVAL - 'path' is invalid for this operation
 *   EEXIST - An inode already exists at 'path'
 *   ENOMEM - Failed to allocate in-memory resources for the operation
 *
 ****************************************************************************/

#ifndef CONFIG_DISABLE_MOUNTPOINT
int register_blockpartition(FAR const char *partition,
                            mode_t mode, FAR const char *parent,
                            size_t firstsector, size_t nsectors);
#endif

/****************************************************************************
 * Name: unregister_driver
 *
 * Description:
 *   Remove the character driver inode at 'path' from the pseudo-file system
 *
 ****************************************************************************/

int unregister_driver(FAR const char *path);

/****************************************************************************
 * Name: unregister_blockdriver
 *
 * Description:
 *   Remove the block driver inode at 'path' from the pseudo-file system
 *
 ****************************************************************************/

int unregister_blockdriver(FAR const char *path);

/****************************************************************************
 * Name: register_mtddriver
 *
 * Description:
 *   Register an MTD driver inode the pseudo file system.
 *
 * Input Parameters:
 *   path - The path to the inode to create
 *   mtd  - The MTD driver structure
 *   mode - inode privileges (not used)
 *   priv - Private, user data that will be associated with the inode.
 *
 * Returned Value:
 *   Zero on success (with the inode point in 'inode'); A negated errno
 *   value is returned on a failure (all error values returned by
 *   inode_reserve):
 *
 *   EINVAL - 'path' is invalid for this operation
 *   EEXIST - An inode already exists at 'path'
 *   ENOMEM - Failed to allocate in-memory resources for the operation
 *
 ****************************************************************************/

#ifdef CONFIG_MTD
int register_mtddriver(FAR const char *path, FAR struct mtd_dev_s *mtd,
                       mode_t mode, FAR void *priv);
#endif

/****************************************************************************
 * Name: register_mtdpartition
 *
 * Description:
 *   Register a mtd partition driver inode the pseudo file system.
 *
 * Input Parameters:
 *   partition  - The path to the partition inode
 *   parent     - The path to the parent inode
 *   firstblock - The offset in block to the partition
 *   nblocks    - The number of block in the partition
 *
 * Returned Value:
 *   Zero on success (with the inode point in 'inode'); A negated errno
 *   value is returned on a failure (all error values returned by
 *   inode_reserve):
 *
 *   EINVAL - 'path' is invalid for this operation
 *   EEXIST - An inode already exists at 'path'
 *   ENOMEM - Failed to allocate in-memory resources for the operation
 *
 ****************************************************************************/

#ifdef CONFIG_MTD
int register_mtdpartition(FAR const char *partition,
                          mode_t mode, FAR const char *parent,
                          off_t firstblock, size_t nblocks);
#endif

/****************************************************************************
 * Name: unregister_mtddriver
 *
 * Description:
 *   Remove the named TMD driver inode at 'path' from the pseudo-file system
 *
 ****************************************************************************/

#ifdef CONFIG_MTD
int unregister_mtddriver(FAR const char *path);
#endif

/****************************************************************************
 * Name: inode_checkflags
 *
 * Description:
 *   Check if the access described by 'oflags' is supported on 'inode'
 *
 ****************************************************************************/

int inode_checkflags(FAR struct inode *inode, int oflags);

/****************************************************************************
 * Name: files_initlist
 *
 * Description:
 *   Initializes the list of files for a new task
 *
 ****************************************************************************/

void files_initlist(FAR struct filelist *list);

/****************************************************************************
 * Name: files_releaselist
 *
 * Description:
 *   Release a reference to the file list
 *
 ****************************************************************************/

void files_releaselist(FAR struct filelist *list);

/****************************************************************************
 * Name: file_dup2
 *
 * Description:
 *   Assign an inode to a specific files structure.  This is the heart of
 *   dup2.
 *
 *   Equivalent to the non-standard fs_dupfd2() function except that it
 *   accepts struct file instances instead of file descriptors and it does
 *   not set the errno variable.
 *
 * Returned Value:
 *   Zero (OK) is returned on success; a negated errno value is return on
 *   any failure.
 *
 ****************************************************************************/

int file_dup2(FAR struct file *filep1, FAR struct file *filep2);

/****************************************************************************
 * Name: fs_dupfd OR dup
 *
 * Description:
 *   Clone a file descriptor 'fd' to an arbitrary descriptor number (any value
 *   greater than or equal to 'minfd'). If socket descriptors are
 *   implemented, then this is called by dup() for the case of file
 *   descriptors.  If socket descriptors are not implemented, then this
 *   function IS dup().
 *
 *   This alternative naming is used when dup could operate on both file and
 *   socket descriptors to avoid drawing unused socket support into the link.
 *
 * Returned Value:
 *   fs_dupfd is sometimes an OS internal function and sometimes is a direct
 *   substitute for dup().  So it must return an errno value as though it
 *   were dup().
 *
 ****************************************************************************/

int fs_dupfd(int fd, int minfd);

/****************************************************************************
 * Name: file_dup
 *
 * Description:
 *   Equivalent to the non-standard fs_dupfd() function except that it
 *   accepts a struct file instance instead of a file descriptor and does
 *   not set the errno variable.
 *
 * Returned Value:
 *   Zero (OK) is returned on success; a negated errno value is returned on
 *   any failure.
 *
 ****************************************************************************/

int file_dup(FAR struct file *filep, int minfd);

/****************************************************************************
 * Name: fs_dupfd2 OR dup2
 *
 * Description:
 *   Clone a file descriptor to a specific descriptor number. If socket
 *   descriptors are implemented, then this is called by dup2() for the
 *   case of file descriptors.  If socket descriptors are not implemented,
 *   then this function IS dup2().
 *
 *   This alternative naming is used when dup2 could operate on both file and
 *   socket descriptors to avoid drawing unused socket support into the link.
 *
 * Returned Value:
 *   fs_dupfd2 is sometimes an OS internal function and sometimes is a direct
 *   substitute for dup2().  So it must return an errno value as though it
 *   were dup2().
 *
 ****************************************************************************/

#ifdef CONFIG_NET
int fs_dupfd2(int fd1, int fd2);
#else
#  define fs_dupfd2(fd1, fd2) dup2(fd1, fd2)
#endif

/****************************************************************************
 * Name: file_open
 *
 * Description:
 *   file_open() is similar to the standard 'open' interface except that it
 *   returns an instance of 'struct file' rather than a file descriptor.  It
 *   also is not a cancellation point and does not modify the errno variable.
 *
 * Input Parameters:
 *   filep  - The caller provided location in which to return the 'struct
 *            file' instance.
 *   path   - The full path to the file to be open.
 *   oflags - open flags
 *   ...    - Variable number of arguments, may include 'mode_t mode'
 *
 * Returned Value:
 *   Zero (OK) is returned on success.  On failure, a negated errno value is
 *   returned.
 *
 ****************************************************************************/

int file_open(FAR struct file *filep, FAR const char *path, int oflags, ...);

/****************************************************************************
 * Name: file_detach
 *
 * Description:
 *   This function is used to device drivers to create a task-independent
 *   handle to an entity in the file system.  file_detach() duplicates the
 *   'struct file' that underlies the file descriptor, then closes the file
 *   descriptor.
 *
 *   This function will fail if fd is not a valid file descriptor.  In
 *   particular, it will fail if fd is a socket descriptor.
 *
 * Input Parameters:
 *   fd    - The file descriptor to be detached.  This descriptor will be
 *           closed and invalid if the file was successfully detached.
 *   filep - A pointer to a user provided memory location in which to
 *           received the duplicated, detached file structure.
 *
 * Returned Value:
 *   Zero (OK) is returned on success; A negated errno value is returned on
 *   any failure to indicate the nature of the failure.
 *
 ****************************************************************************/

int file_detach(int fd, FAR struct file *filep);

/****************************************************************************
 * Name: file_close
 *
 * Description:
 *   Close a file that was previously opened with file_open() (or detached
 *   with file_detach()).
 *
 * Input Parameters:
 *   filep - A pointer to a user provided memory location containing the
 *           open file data returned by file_detach().
 *
 * Returned Value:
 *   Zero (OK) is returned on success; A negated errno value is returned on
 *   any failure to indicate the nature of the failure.
 *
 ****************************************************************************/

int file_close(FAR struct file *filep);

/****************************************************************************
 * Name: open_blockdriver
 *
 * Description:
 *   Return the inode of the block driver specified by 'pathname'
 *
 * Input Parameters:
 *   pathname - the full path to the block driver to be opened
 *   mountflags - if MS_RDONLY is not set, then driver must support write
 *     operations (see include/sys/mount.h)
 *   ppinode - address of the location to return the inode reference
 *
 * Returned Value:
 *   Returns zero on success or a negated errno on failure:
 *
 *   EINVAL  - pathname or pinode is NULL
 *   ENOENT  - No block driver of this name is registered
 *   ENOTBLK - The inode associated with the pathname is not a block driver
 *   EACCESS - The MS_RDONLY option was not set but this driver does not
 *     support write access
 *
 ****************************************************************************/

int open_blockdriver(FAR const char *pathname, int mountflags,
                     FAR struct inode **ppinode);

/****************************************************************************
 * Name: close_blockdriver
 *
 * Description:
 *   Call the close method and release the inode
 *
 * Input Parameters:
 *   inode - reference to the inode of a block driver opened by open_blockdriver
 *
 * Returned Value:
 *   Returns zero on success or a negated errno on failure:
 *
 *   EINVAL  - inode is NULL
 *   ENOTBLK - The inode is not a block driver
 *
 ****************************************************************************/

int close_blockdriver(FAR struct inode *inode);

/****************************************************************************
 * Name: fs_ioctl
 *
 * Description:
 *   Perform device specific operations.
 *
 * Input Parameters:
 *   fd       File/socket descriptor of device
 *   req      The ioctl command
 *   arg      The argument of the ioctl cmd
 *
 * Returned Value:
 *   >=0 on success (positive non-zero values are cmd-specific)
 *   -1 on failure with errno set properly:
 *
 *   EBADF
 *     'fd' is not a valid descriptor.
 *   EFAULT
 *     'arg' references an inaccessible memory area.
 *   EINVAL
 *     'cmd' or 'arg' is not valid.
 *   ENOTTY
 *     'fd' is not associated with a character special device.
 *   ENOTTY
 *      The specified request does not apply to the kind of object that the
 *      descriptor 'fd' references.
 *
 ****************************************************************************/

#ifdef CONFIG_LIBC_IOCTL_VARIADIC
int fs_ioctl(int fd, int req, unsigned long arg);
#endif

/****************************************************************************
 * Name: fs_fdopen
 *
 * Description:
 *   This function does the core operations for fopen and fdopen.  It is
 *   used by the OS to clone stdin, stdout, stderr
 *
 ****************************************************************************/

#if CONFIG_NFILE_STREAMS > 0
struct tcb_s; /* Forward reference */
FAR struct file_struct *fs_fdopen(int fd, int oflags, FAR struct tcb_s *tcb);
#endif

/****************************************************************************
 * Name: lib_flushall
 *
 * Description:
 *   Called either (1) by the OS when a task exits, or (2) from fflush()
 *   when a NULL stream argument is provided.
 *
 ****************************************************************************/

#if CONFIG_NFILE_STREAMS > 0
int lib_flushall(FAR struct streamlist *list);
#endif

/****************************************************************************
 * Name: lib_sendfile
 *
 * Description:
 *   Transfer a file
 *
 ****************************************************************************/

#ifdef CONFIG_NET_SENDFILE
ssize_t lib_sendfile(int outfd, int infd, off_t *offset, size_t count);
#endif

/****************************************************************************
 * Name: fs_getfilep
 *
 * Description:
 *   Given a file descriptor, return the corresponding instance of struct
 *   file.  NOTE that this function will currently fail if it is provided
 *   with a socket descriptor.
 *
 * Input Parameters:
 *   fd    - The file descriptor
 *   filep - The location to return the struct file instance
 *
 * Returned Value:
 *   Zero (OK) is returned on success; a negated errno value is returned on
 *   any failure.
 *
 ****************************************************************************/

int fs_getfilep(int fd, FAR struct file **filep);

/****************************************************************************
 * Name: nx_open and nx_vopen
 *
 * Description:
 *   nx_open() is similar to the standard 'open' interface except that is is
 *   not a cancellation point and it does not modify the errno variable.
 *
 *   nx_vopen() is identical except that it accepts a va_list as an argument
 *   versus taking a variable length list of arguments.
 *
 *   nx_open() and nx_vopen are internal NuttX interface and should not be
 *   called from applications.
 *
 * Returned Value:
 *   Zero (OK) is returned on success; a negated errno value is returned on
 *   any failure.
 *
 ****************************************************************************/

int nx_vopen(FAR const char *path, int oflags, va_list ap);
int nx_open(FAR const char *path, int oflags, ...);

/****************************************************************************
 * Name: file_read
 *
 * Description:
 *   file_read() is an internal OS interface.  It is functionally similar to
 *   the standard read() interface except:
 *
 *    - It does not modify the errno variable,
 *    - It is not a cancellation point,
 *    - It does not handle socket descriptors, and
 *    - It accepts a file structure instance instead of file descriptor.
 *
 * Input Parameters:
 *   filep  - File structure instance
 *   buf    - User-provided to save the data
 *   nbytes - The maximum size of the user-provided buffer
 *
 * Returned Value:
 *   The positive non-zero number of bytes read on success, 0 on if an
 *   end-of-file condition, or a negated errno value on any failure.
 *
 ****************************************************************************/

ssize_t file_read(FAR struct file *filep, FAR void *buf, size_t nbytes);

/****************************************************************************
 * Name: nx_read
 *
 * Description:
 *   nx_read() is an internal OS interface.  It is functionally similar to
 *   the standard read() interface except:
 *
 *    - It does not modify the errno variable, and
 *    - It is not a cancellation point.
 *
 * Input Parameters:
 *   fd     - File descriptor to read from
 *   buf    - User-provided to save the data
 *   nbytes - The maximum size of the user-provided buffer
 *
 * Returned Value:
 *   The positive non-zero number of bytes read on success, 0 on if an
 *   end-of-file condition, or a negated errno value on any failure.
 *
 ****************************************************************************/

ssize_t nx_read(int fd, FAR void *buf, size_t nbytes);

/****************************************************************************
 * Name: file_write
 *
 * Description:
 *   Equivalent to the standard write() function except that is accepts a
 *   struct file instance instead of a file descriptor.  Currently used
 *   only by aio_write();
 *
 ****************************************************************************/

ssize_t file_write(FAR struct file *filep, FAR const void *buf, size_t nbytes);

/****************************************************************************
 * Name: nx_write
 *
 * Description:
 *  nx_write() writes up to nbytes bytes to the file referenced by the file
 *  descriptor fd from the buffer starting at buf.  nx_write() is an
 *  internal OS function.  It is functionally equivalent to write() except
 *  that:
 *
 *  - It does not modify the errno variable, and
 *  - It is not a cancellation point.
 *
 * Input Parameters:
 *   fd     - file descriptor (or socket descriptor) to write to
 *   buf    - Data to write
 *   nbytes - Length of data to write
 *
 * Returned Value:
 *  On success, the number of bytes written are returned (zero indicates
 *  nothing was written).  On any failure, a negated errno value is returned
 *  (see comments with write() for a description of the appropriate errno
 *   values).
 *
 ****************************************************************************/

ssize_t nx_write(int fd, FAR const void *buf, size_t nbytes);

/****************************************************************************
 * Name: file_pread
 *
 * Description:
 *   Equivalent to the standard pread function except that is accepts a
 *   struct file instance instead of a file descriptor.  Currently used
 *   only by aio_read();
 *
 ****************************************************************************/

ssize_t file_pread(FAR struct file *filep, FAR void *buf, size_t nbytes,
                   off_t offset);

/****************************************************************************
 * Name: file_pwrite
 *
 * Description:
 *   Equivalent to the standard pwrite function except that is accepts a
 *   struct file instance instead of a file descriptor.  Currently used
 *   only by aio_write();
 *
 ****************************************************************************/

ssize_t file_pwrite(FAR struct file *filep, FAR const void *buf,
                    size_t nbytes, off_t offset);

/****************************************************************************
 * Name: file_seek
 *
 * Description:
 *   Equivalent to the standard lseek() function except that is accepts a
 *   struct file instance instead of a file descriptor.  Currently used
 *   only by net_sendfile()
 *
 ****************************************************************************/

off_t file_seek(FAR struct file *filep, off_t offset, int whence);

/****************************************************************************
 * Name: file_fsync
 *
 * Description:
 *   Equivalent to the standard fsync() function except that is accepts a
 *   struct file instance instead of a file descriptor and it does not set
 *   the errno variable.
 *
 ****************************************************************************/

#ifndef CONFIG_DISABLE_MOUNTPOINT
int file_fsync(FAR struct file *filep);
#endif

/****************************************************************************
 * Name: file_truncate
 *
 * Description:
 *   Equivalent to the standard ftruncate() function except that is accepts
 *   a struct file instance instead of a file descriptor and it does not set
 *   the errno variable.
 *
 ****************************************************************************/

#ifndef CONFIG_DISABLE_MOUNTPOINT
int file_truncate(FAR struct file *filep, off_t length);
#endif

/****************************************************************************
 * Name: file_ioctl
 *
 * Description:
 *   Perform device specific operations.
 *
 * Input Parameters:
 *   file     File structure instance
 *   req      The ioctl command
 *   arg      The argument of the ioctl cmd
 *
 * Returned Value:
 *   Returns a non-negative number on success;  A negated errno value is
 *   returned on any failure (see comments ioctl() for a list of appropriate
 *   errno values).
 *
 ****************************************************************************/

int file_ioctl(FAR struct file *filep, int req, unsigned long arg);

/****************************************************************************
 * Name: file_vfcntl
 *
 * Description:
 *   Similar to the standard vfcntl function except that is accepts a struct
 *   struct file instance instead of a file descriptor.
 *
 * Input Parameters:
 *   filep - Instance for struct file for the opened file.
 *   cmd   - Indentifies the operation to be performed.
 *   ap    - Variable argument following the command.
 *
 * Returned Value:
 *   The nature of the return value depends on the command.  Non-negative
 *   values indicate success.  Failures are reported as negated errno
 *   values.
 *
 ****************************************************************************/

int file_vfcntl(FAR struct file *filep, int cmd, va_list ap);

/****************************************************************************
 * Name: file_fcntl
 *
 * Description:
 *   Similar to the standard fcntl function except that is accepts a struct
 *   struct file instance instead of a file descriptor.
 *
 * Input Parameters:
 *   filep - Instance for struct file for the opened file.
 *   cmd   - Identifies the operation to be performed.  Command specific
 *           arguments may follow.
 *
 * Returned Value:
 *   The nature of the return value depends on the command.  Non-negative
 *   values indicate success.  Failures are reported as negated errno
 *   values.
 *
 ****************************************************************************/

int file_fcntl(FAR struct file *filep, int cmd, ...);

/****************************************************************************
 * Name: file_poll
 *
 * Description:
 *   Low-level poll operation based on struct file.  This is used both to (1)
 *   support detached file, and also (2) by fdesc_poll() to perform all
 *   normal operations on file descriptors descriptors.
 *
 * Input Parameters:
 *   file     File structure instance
 *   fds   - The structure describing the events to be monitored, OR NULL if
 *           this is a request to stop monitoring events.
 *   setup - true: Setup up the poll; false: Teardown the poll
 *
 * Returned Value:
 *  0: Success; Negated errno on failure
 *
 ****************************************************************************/

int file_poll(FAR struct file *filep, FAR struct pollfd *fds, bool setup);

/****************************************************************************
 * Name: file_fstat
 *
 * Description:
 *   file_fstat() is an internal OS interface.  It is functionally similar to
 *   the standard fstat() interface except:
 *
 *    - It does not modify the errno variable,
 *    - It is not a cancellation point,
 *    - It does not handle socket descriptors, and
 *    - It accepts a file structure instance instead of file descriptor.
 *
 * Input Parameters:
 *   filep  - File structure instance
 *   buf    - The caller provide location in which to return information about
 *            the open file.
 *
 * Returned Value:
 *   Upon successful completion, 0 shall be returned. Otherwise, -1 shall be
 *   returned and errno set to indicate the error.
 *
 ****************************************************************************/

int file_fstat(FAR struct file *filep, FAR struct stat *buf);

/****************************************************************************
 * Name: fdesc_poll
 *
 * Description:
 *   The standard poll() operation redirects operations on file descriptors
 *   to this function.
 *
 * Input Parameters:
 *   fd    - The file descriptor of interest
 *   fds   - The structure describing the events to be monitored, OR NULL if
 *           this is a request to stop monitoring events.
 *   setup - true: Setup up the poll; false: Teardown the poll
 *
 * Returned Value:
 *  0: Success; Negated errno on failure
 *
 ****************************************************************************/

int fdesc_poll(int fd, FAR struct pollfd *fds, bool setup);

#undef EXTERN
#if defined(__cplusplus)
}
#endif

#endif /* __INCLUDE_NUTTX_FS_FS_H */
