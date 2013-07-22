/*
 * License: GPL v2
 * Copyright (c) <jeff66ruan@gmail.com>
 *
 */
#ifndef RDTSC_H
#define RDTSC_H

#define CPUID {                        \
  __asm__ __volatile__ (               \
  "cpuid\n\t"                          \
  ::: "%rax", "%rbx", "%rcx", "%rdx"); \
}

//uint32_t lo, hi
#define RDTSC(lo, hi) {                        \
  __asm__ __volatile__ ("rdtsc\n\t"            \
                        "mov %%edx, %0\n\t"    \
                        "mov %%eax, %1\n\t"    \
                        : "=r" (hi), "=r" (lo) \
                        :: "%rax", "%rdx");    \
}

//uint32_t lo, hi
#define RDTSCP(lo, hi) {                        \
  __asm__ __volatile__ ("rdtscp\n\t"            \
                        "mov %%edx, %0\n\t"     \
                        "mov %%eax, %1\n\t"     \
                        : "=r" (hi), "=r" (lo)  \
                        :: "%rax", "%rdx",      \
                        "%rbx", "%rcx");        \
}

#define START_COUNTER(lo, hi) {CPUID;RDTSC(lo, hi);}
#define STOP_COUNTER(lo, hi) {RDTSCP(lo, hi);CPUID;}
#define MAKE_TIME_FROM_COUNTERS(lo, hi)  ((uint64_t)hi << 32 | lo)


#endif
