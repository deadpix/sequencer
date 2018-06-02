#!/bin/bash
rm -f cscope.*
find . -name "*.cpp" > cscope.files
find . -name "*.h" >> cscope.files
find . -name "*.ino" >> cscope.files
find ../test7-refactor/ -name "*.cpp" >> cscope.files
find ../test7-refactor/ -name "*.h" >> cscope.files
find ../test7-refactor/ -name "*.ino" >> cscope.files

cscope -b -q -k
