/*
 * performance.h - Routines and macros for performance statistics of our system
 */

#ifndef __PERFORMANCE_H__
#define __PERFORMANCE_H__

#define TICKS_SECOND 18

/* Prints the frames per second */
void print_fps();

/* Increments the number of frames */
void inc_fps();

#endif /* __PERFORMANCE_H__ */
