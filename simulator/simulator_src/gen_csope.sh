#!/bin/bash
SRC_PATH=../../sequencepter/
rm -f cscope.*
find $SRC_PATH -name "*.cpp" > cscope.files
find $SRC_PATH -name "*.h" >> cscope.files
find $SRC_PATH -name "*.ino" >> cscope.files
cscope -b -q -k
