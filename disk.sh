#!/bin/bash

if [ "$1" != "" ]; then
	export PREFIX=$1
else
	export PREFIX="/usr/local/foxos-x86_64_elf_gcc"
fi

if [ "$2" != "" ]; then
	export PROG_PREFIX=$2
else
	export PROG_PREFIX="foxos-"
fi

if [ -f $PREFIX'/bin/'$PROG_PREFIX'losetup' ]; then
	dev_mount=`$PREFIX'/bin/'$PROG_PREFIX'losetup' f | egrep -o '[0-9]+'`
else
	dev_mount=`losetup -f | egrep -o '[0-9]+'`
fi

if [ -f $PREFIX'/bin/'$PROG_PREFIX'losetup' ]; then
	$PREFIX'/bin/'$PROG_PREFIX'losetup' m ${dev_mount}
else
	losetup /dev/loop${dev_mount} foxos.img -P
fi

echo Mounted disk as /dev/loop${dev_mount}

mcopy -i /dev/loop${dev_mount}p1 limine.cfg :: -o
mcopy -i /dev/loop${dev_mount}p1 bin/module.o :: -o

if [ -f $PREFIX'/bin/'$PROG_PREFIX'losetup' ]; then
	$PREFIX'/bin/'$PROG_PREFIX'losetup' u ${dev_mount}
else
	losetup -d /dev/loop${dev_mount}
fi