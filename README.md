trikTest
========

An application for testing TRIK controllers

usage:

> source <path to trik-sdk>
> git clean -xdf && qmake [CONFIG+=debug] trikTest.pro && make [-j8] && rsync -avz --progress ./build/<debug/release>  root@<ip>:/home/root/

ssh / minicom:

> cd <release/debug>
> ./runme.sh