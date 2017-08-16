#!/bin/sh -e
LOCALDIR=$(dirname $0)/
linkHere () {
  test -e ${LOCALDIR}$1 || ln -sfvt ${LOCALDIR} /home/root/trik/$1
}

modprobe -rv jcx_pwm
modprobe -v jcx_ecap_pwm

linkHere system-config.xml
linkHere model-config.xml

exec $LOCALDIR/trikTest
