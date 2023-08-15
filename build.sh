#!/bin/bash
gcc -o convert_byte.so convert_byte.c $(yed --print-cflags) $(yed --print-ldflags)
