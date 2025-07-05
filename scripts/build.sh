#!/bin/bash

export PREFIX="$HOME/opt/cross"
export TARGET=i686-elf
export PATH="$PREFIX/bin:$PATH"

CURRENT_DIR=$PWD
cd $CURRENT_DIR

make clean
make all
