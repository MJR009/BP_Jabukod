/**
 * @file System.h
 * @author Martin Jabůrek
 *
 * @brief File containing macros of values need to carry out system calls.
 */

#pragma once

/**
 * @defgroup systemConstants Values used as system call arguments
 * 
 * @{
 */
#define SYSCALL_WRITE 1 ///< "write" system call number
#define SYSCALL_EXIT 60 ///< "exit" system call number

#define STDOUT 1 ///< standard terminal output number
/** @} */
