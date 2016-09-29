This repository is a collection of normal C programs which use hard-disk as persistent sotrage and their equivalents based on nmv heap using `pmem.io` library.

Here `file1.c` is a simple program which reads from cmdline and writes to file and reads back from file based on cmdline args. `file1_persistent.c` is its equivalent based on pmem.io library which uses non-volatile for the same.
