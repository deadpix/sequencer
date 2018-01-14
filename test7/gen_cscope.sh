#!/bin/bash
rm -f cscope.*
find . -name "*.cpp" > cscope.files
find . -name "*.h" >> cscope.files
find . -name "*.ino" >> cscope.files
cscope -b -q -k
