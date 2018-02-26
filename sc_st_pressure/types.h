/*
 $License:
    Copyright (C) 2011 InvenSense Corporation, All Rights Reserved.
 $
 */

/**
 *  @defgroup MLERROR
 *  @brief  Motion Library - Error definitions.
 *          Definition of the error codes used within the MPL and
 *          returned to the user.
 *          Every function tries to return a meaningful error code basing
 *          on the occuring error condition. The error code is numeric.
 *
 *          The available error codes and their associated values are:
 *          - (0)               SC_SUCCESS
 *          - (32)              SC_ERROR
 *          - (22 / EINVAL)     SC_ERROR_INVALID_PARAMETER
 *          - (1  / EPERM)      SC_ERROR_FEATURE_NOT_ENABLED
 *          - (36)              SC_ERROR_FEATURE_NOT_IMPLEMENTED
 *          - (38)              SC_ERROR_DMP_NOT_STARTED
 *          - (39)              SC_ERROR_DMP_STARTED
 *          - (40)              SC_ERROR_NOT_OPENED
 *          - (41)              SC_ERROR_OPENED
 *          - (19 / ENODEV)     SC_ERROR_INVALID_MODULE
 *          - (12 / ENOMEM)     SC_ERROR_MEMORY_EXAUSTED
 *          - (44)              SC_ERROR_DIVIDE_BY_ZERO
 *          - (45)              SC_ERROR_ASSERTION_FAILURE
 *          - (46)              SC_ERROR_FILE_OPEN
 *          - (47)              SC_ERROR_FILE_READ
 *          - (48)              SC_ERROR_FILE_WRITE
 *          - (49)              SC_ERROR_INVALID_CONFIGURATION
 *          - (52)              SC_ERROR_SERIAL_CLOSED
 *          - (53)              SC_ERROR_SERIAL_OPEN_ERROR
 *          - (54)              SC_ERROR_SERIAL_READ
 *          - (55)              SC_ERROR_SERIAL_WRITE
 *          - (56)              SC_ERROR_SERIAL_DEVICE_NOT_RECOGNIZED
 *          - (57)              SC_ERROR_SM_TRANSITION
 *          - (58)              SC_ERROR_SM_IMPROPER_STATE
 *          - (62)              SC_ERROR_FIFO_OVERFLOW
 *          - (63)              SC_ERROR_FIFO_FOOTER
 *          - (64)              SC_ERROR_FIFO_READ_COUNT
 *          - (65)              SC_ERROR_FIFO_READ_DATA
 *          - (72)              SC_ERROR_MEMORY_SET
 *          - (82)              SC_ERROR_LOG_MEMORY_ERROR
 *          - (83)              SC_ERROR_LOG_OUTPUT_ERROR
 *          - (92)              SC_ERROR_OS_BAD_PTR
 *          - (93)              SC_ERROR_OS_BAD_HANDLE
 *          - (94)              SC_ERROR_OS_CREATE_FAILED
 *          - (95)              SC_ERROR_OS_LOCK_FAILED
 *          - (102)             SC_ERROR_COMPASS_DATA_OVERFLOW
 *          - (103)             SC_ERROR_COMPASS_DATA_UNDERFLOW
 *          - (104)             SC_ERROR_COMPASS_DATA_NOT_READY
 *          - (105)             SC_ERROR_COMPASS_DATA_ERROR
 *          - (107)             SC_ERROR_CALIBRATION_LOAD
 *          - (108)             SC_ERROR_CALIBRATION_STORE
 *          - (109)             SC_ERROR_CALIBRATION_LEN
 *          - (110)             SC_ERROR_CALIBRATION_CHECKSUM
 *          - (111)             SC_ERROR_ACCEL_DATA_OVERFLOW
 *          - (112)             SC_ERROR_ACCEL_DATA_UNDERFLOW
 *          - (113)             SC_ERROR_ACCEL_DATA_NOT_READY
 *          - (114)             SC_ERROR_ACCEL_DATA_ERROR
 *
 *          The available warning codes and their associated values are:
 *          - (115)             SC_WARNING_MOTION_RACE
 *          - (116)             SC_WARNING_QUAT_TRASHED
 *
 *  @{
 *      @file mltypes.h
 *  @}
 */

#ifndef MLTYPES_H
#define MLTYPES_H

#ifdef __KERNEL__
#include <linux/types.h>
#include <asm-generic/errno-base.h>
#else
#ifdef __KERNEL__
#include <linux/types.h>
#else
#include <stdint.h>
#endif
#include <errno.h>
#endif
#include <limits.h>

#ifndef REMOVE_SC_ERROR_T
/*---------------------------
 *    ML Types
 *--------------------------*/

/**
 *  @struct inv_error_t mltypes.h "mltypes"
 *  @brief  The MPL Error Code return type.
 *
 *  @code
 *      typedef unsigned char inv_error_t;
 *  @endcode
 */
//typedef unsigned char inv_error_t;
typedef int sc_error_t;
#endif

typedef long long sc_time_t;

#if !defined __GNUC__ && !defined __KERNEL__
typedef int8_t   __s8;
typedef int16_t  __s16;
typedef int32_t  __s32;
typedef int32_t  __s64;

typedef uint8_t   __u8;
typedef uint16_t  __u16;
typedef uint32_t  __u32;
typedef uint64_t  __u64;
#elif !defined __KERNEL__
#include <sys/types.h>
#endif

#define false      0
#define true       1

#ifndef __KERNEL__
#define DEBUG_MSG printf
#endif

#ifndef __cplusplus
#ifndef __KERNEL__
typedef int_fast8_t bool;

#ifndef false
#define false 0
#endif

#ifndef true
#define true 1
#endif

#endif
#endif

/*---------------------------
 *    ML Defines
 *--------------------------*/

#ifndef NULL
#define NULL 0
#endif

#ifndef __KERNEL__
#ifndef ARRAY_SIZE
/* Dimension of an array */
#define ARRAY_SIZE(array) (sizeof(array)/sizeof((array)[0]))
#endif
#endif
/* - ML Errors. - */
#define ERROR_NAME(x)   (#x)
#define ERROR_CHECK_FIRST(first, x) \
	{ if (SC_SUCCESS == first) first = x; }

#define SC_SUCCESS                       (0)
/* Generic Error code.  Proprietary Error Codes only */
#define SC_ERROR_BASE                    (0x20)
#define SC_ERROR                         (SC_ERROR_BASE)

/* Compatibility and other generic error codes */
#define SC_ERROR_INVALID_PARAMETER             (EINVAL)
#define SC_ERROR_FEATURE_NOT_ENABLED           (EPERM)
#define SC_ERROR_FEATURE_NOT_IMPLEMENTED       (SC_ERROR_BASE + 4)
#define SC_ERROR_DMP_NOT_STARTED               (SC_ERROR_BASE + 6)
#define SC_ERROR_DMP_STARTED                   (SC_ERROR_BASE + 7)
#define SC_ERROR_NOT_OPENED                    (SC_ERROR_BASE + 8)
#define SC_ERROR_OPENED                        (SC_ERROR_BASE + 9)
#define SC_ERROR_INVALID_MODULE                (ENODEV)
#define SC_ERROR_MEMORY_EXAUSTED               (ENOMEM)
#define SC_ERROR_DIVIDE_BY_ZERO                (SC_ERROR_BASE + 12)
#define SC_ERROR_ASSERTION_FAILURE             (SC_ERROR_BASE + 13)
#define SC_ERROR_FILE_OPEN                     (SC_ERROR_BASE + 14)
#define SC_ERROR_FILE_READ                     (SC_ERROR_BASE + 15)
#define SC_ERROR_FILE_WRITE                    (SC_ERROR_BASE + 16)
#define SC_ERROR_INVALID_CONFIGURATION         (SC_ERROR_BASE + 17)
#define SC_ERROR_NOT_AUTHORIZED                (SC_ERROR_BASE + 18)

/* Serial Communication */
#define SC_ERROR_SERIAL_CLOSED                 (SC_ERROR_BASE + 20)
#define SC_ERROR_SERIAL_OPEN_ERROR             (SC_ERROR_BASE + 21)
#define SC_ERROR_SERIAL_READ                   (SC_ERROR_BASE + 22)
#define SC_ERROR_SERIAL_WRITE                  (SC_ERROR_BASE + 23)
#define SC_ERROR_SERIAL_DEVICE_NOT_RECOGNIZED  (SC_ERROR_BASE + 24)

/* SM = State Machine */
#define SC_ERROR_SM_TRANSITION                 (SC_ERROR_BASE + 25)
#define SC_ERROR_SM_IMPROPER_STATE             (SC_ERROR_BASE + 26)

/* Fifo */
#define SC_ERROR_FIFO_OVERFLOW                 (SC_ERROR_BASE + 30)
#define SC_ERROR_FIFO_FOOTER                   (SC_ERROR_BASE + 31)
#define SC_ERROR_FIFO_READ_COUNT               (SC_ERROR_BASE + 32)
#define SC_ERROR_FIFO_READ_DATA                (SC_ERROR_BASE + 33)

/* Memory & Registers, Set & Get */
#define SC_ERROR_MEMORY_SET                    (SC_ERROR_BASE + 40)

#define SC_ERROR_LOG_MEMORY_ERROR              (SC_ERROR_BASE + 50)
#define SC_ERROR_LOG_OUTPUT_ERROR              (SC_ERROR_BASE + 51)

/* OS interface errors */
#define SC_ERROR_OS_BAD_PTR                    (SC_ERROR_BASE + 60)
#define SC_ERROR_OS_BAD_HANDLE                 (SC_ERROR_BASE + 61)
#define SC_ERROR_OS_CREATE_FAILED              (SC_ERROR_BASE + 62)
#define SC_ERROR_OS_LOCK_FAILED                (SC_ERROR_BASE + 63)

/* Compass errors */
#define SC_ERROR_COMPASS_DATA_OVERFLOW         (SC_ERROR_BASE + 70)
#define SC_ERROR_COMPASS_DATA_UNDERFLOW        (SC_ERROR_BASE + 71)
#define SC_ERROR_COMPASS_DATA_NOT_READY        (SC_ERROR_BASE + 72)
#define SC_ERROR_COMPASS_DATA_ERROR            (SC_ERROR_BASE + 73)

/* Load/Store calibration */
#define SC_ERROR_CALIBRATION_LOAD              (SC_ERROR_BASE + 75)
#define SC_ERROR_CALIBRATION_STORE             (SC_ERROR_BASE + 76)
#define SC_ERROR_CALIBRATION_LEN               (SC_ERROR_BASE + 77)
#define SC_ERROR_CALIBRATION_CHECKSUM          (SC_ERROR_BASE + 78)

/* Accel errors */
#define SC_ERROR_ACCEL_DATA_OVERFLOW           (SC_ERROR_BASE + 79)
#define SC_ERROR_ACCEL_DATA_UNDERFLOW          (SC_ERROR_BASE + 80)
#define SC_ERROR_ACCEL_DATA_NOT_READY          (SC_ERROR_BASE + 81)
#define SC_ERROR_ACCEL_DATA_ERROR              (SC_ERROR_BASE + 82)

/* No Motion Warning States */
#define SC_WARNING_MOTION_RACE                 (SC_ERROR_BASE + 83)
#define SC_WARNING_QUAT_TRASHED                (SC_ERROR_BASE + 84)
#define SC_WARNING_GYRO_MAG                    (SC_ERROR_BASE + 85)

#define SC_WARNING_SEMAPHORE_TIMEOUT           (SC_ERROR_BASE + 86)


/* For Linux coding compliance */
#ifndef __KERNEL__
#define EXPORT_SYMBOL(x)
#endif

#endif				/* MLTYPES_H */
