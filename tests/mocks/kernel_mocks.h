/*
 * Mock implementations of kernel functions for unit testing
 * 
 * This file provides stub/mock implementations of kernel-specific functions
 * that allow us to compile and test kernel code on macOS without the full
 * kernel environment.
 */

#ifndef _KERNEL_MOCKS_H
#define _KERNEL_MOCKS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <stddef.h>

/* Mock kernel types */
typedef unsigned long size_t;
typedef long off_t;
typedef int pid_t;
typedef unsigned int uid_t;
typedef unsigned int gid_t;
typedef unsigned int mode_t;
typedef unsigned int dev_t;
typedef unsigned long ino_t;
typedef unsigned int nlink_t;

/* Mock printk - just use printf */
int printk(const char *fmt, ...) {
    va_list args;
    int result;
    
    va_start(args, fmt);
    result = vprintf(fmt, args);
    va_end(args);
    
    return result;
}

/* Mock panic - print message and exit */
void panic(const char *fmt, ...) {
    va_list args;
    
    fprintf(stderr, "KERNEL PANIC: ");
    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    va_end(args);
    fprintf(stderr, "\n");
    
    exit(1);
}

/* Mock memory allocation that tracks allocations */
#define MAX_ALLOCS 1000
static void *mock_allocs[MAX_ALLOCS];
static int mock_alloc_count = 0;

void *mock_kmalloc(size_t size) {
    void *ptr = malloc(size);
    if (ptr && mock_alloc_count < MAX_ALLOCS) {
        mock_allocs[mock_alloc_count++] = ptr;
    }
    return ptr;
}

void mock_kfree(void *ptr) {
    if (ptr) {
        free(ptr);
        /* Remove from tracking */
        for (int i = 0; i < mock_alloc_count; i++) {
            if (mock_allocs[i] == ptr) {
                mock_allocs[i] = NULL;
                break;
            }
        }
    }
}

void mock_free_all(void) {
    for (int i = 0; i < mock_alloc_count; i++) {
        if (mock_allocs[i]) {
            free(mock_allocs[i]);
            mock_allocs[i] = NULL;
        }
    }
    mock_alloc_count = 0;
}

/* Mock memory operations */
#define cli() /* No-op on modern systems */
#define sti() /* No-op on modern systems */
#define save_flags(x) (x = 0)
#define restore_flags(x) /* No-op */

/* Mock inline assembly functions */
static inline void outb(unsigned char value, unsigned short port) {
    /* No-op for testing */
}

static inline unsigned char inb(unsigned short port) {
    return 0;
}

static inline void outw(unsigned short value, unsigned short port) {
    /* No-op for testing */
}

static inline unsigned short inw(unsigned short port) {
    return 0;
}

/* Mock segment operations */
#define get_fs() 0
#define get_ds() 0
#define set_fs(x) /* No-op */

static inline unsigned char get_user_byte(const char *addr) {
    return *addr;
}

static inline void put_user_byte(unsigned char val, char *addr) {
    *addr = val;
}

/* Mock verification functions */
static inline int verify_area(int type, const void *addr, unsigned long size) {
    return 0; /* Always succeeds in tests */
}

#define VERIFY_READ 0
#define VERIFY_WRITE 1

/* Mock scheduling functions */
#define schedule() /* No-op for testing */
#define sleep_on(x) /* No-op for testing */
#define wake_up(x) /* No-op for testing */

/* Mock locking primitives */
typedef struct { int dummy; } spinlock_t;
#define SPIN_LOCK_UNLOCKED { 0 }
#define spin_lock(x) /* No-op */
#define spin_unlock(x) /* No-op */
#define spin_lock_irqsave(x, flags) (flags = 0)
#define spin_unlock_irqrestore(x, flags) /* No-op */

/* Mock time functions */
static inline unsigned long jiffies(void) {
    return 0;
}

/* Mock current process */
#define current ((struct task_struct *)NULL)

/* Mock kernel configuration */
#ifndef NULL
#define NULL ((void *)0)
#endif

#ifndef PAGE_SIZE
#define PAGE_SIZE 4096
#endif

/* Error numbers */
#define ENOMEM 12
#define EINVAL 22
#define EFAULT 14
#define EACCES 13
#define ENOENT 2

#endif /* _KERNEL_MOCKS_H */

