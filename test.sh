#!/bin/bash

gcc -o test/testing -ldl test/testing.c

LD_PRELOAD=obj64/libpart2.so ./test/testing
