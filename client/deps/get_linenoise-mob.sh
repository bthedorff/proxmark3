#!/bin/bash

version=a898fd484982f29354d32a2a1afe01a01024d99f
mkdir -p linenoise-mob
wget https://github.com/rain-1/linenoise-mob/archive/$version.zip
ZP=linenoise-mob-$version
unzip -o -j $version $ZP/LICENSE $ZP/linenoise.c $ZP/linenoise.h $ZP/utf8.c $ZP/utf8.h -d linenoise-mob
rm $version.zip
echo "Please do make style"
cat > linenoise-mob/Makefile << EOF
MYSRCPATHS =
MYINCLUDES =
MYCFLAGS = -Wno-shadow -Wno-missing-prototypes -Wno-missing-declarations -Wno-redundant-decls
MYDEFS =
MYSRCS = utf8.c linenoise.c

LIB_A = liblinenoise.a

include ../../../Makefile.host
EOF
