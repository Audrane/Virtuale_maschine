#!/bin/bash

# compiler

./njc $1.nj --output $1.asm 
./nja $1.asm $1.bin
