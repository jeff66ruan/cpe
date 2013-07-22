cpe
===

This rdtsc.h provides primitives of evaluating cycles per element(CPE) in the book "computer system: a programmer's perspective"

Authors of the book define the concept of CPE and the data result of different combine routines in chapter 5. But the source code of measurement, which is how to get the CPE of a measured routine, isn't given. 

The cpe code is my study result. It works on Linux with a x86_64 chip with rdtscp feature. It can be checked by running the following command:
   > cat /proc/cpuinfo
If rdtscp is supported, it can be found in the flag field.


NOTE:

In order to get accurate and stable cycles, all kinds of power and speed optimization need to be turned off in BIOS. Otherwise, we cannot get the same lab result as the book according to my experience.