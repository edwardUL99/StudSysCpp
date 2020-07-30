#! /usr/bin/bash

cd /home/eddy/Coding/C++/StudSysCppCLI/headers/studsys 

cp /home/eddy/Coding/C++/StudSysCppLibrary/headers/* .

cd /home/eddy/Coding/C++/StudSysCppLibrary

make

cd /home/eddy/Coding/C++/StudSysCppCLI

cp /home/eddy/Coding/C++/StudSysCppLibrary/libstudsys.so .
