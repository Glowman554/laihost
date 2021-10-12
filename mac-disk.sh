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

dev_mount=`hdiutil attach -nomount -noverify foxos.img | egrep -o '/dev/disk[0-9]+' | head -1`

echo "Mounted disk as ${dev_mount}"

mcopy -D o -i ${dev_mount}s1 limine.cfg ::
mcopy -D o -i ${dev_mount}s1 bin/module.o ::

hdiutil detach ${dev_mount}
