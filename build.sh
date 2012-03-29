#!/bin/sh

set -e

cd "$(dirname "$0")"

rm -rf build/temp build/DUHeresy.pk7
mkdir build/temp
mkdir build/temp/ACS

compile_script()
{
   echo "Compiling $1.ds"

   DH-acc --target=ZDoom --output=object \
          -isrc/code/ -i/data/src/SELF/DH-acc/inc/ \
          --named-scripts \
          --no-string-func \
          --out=build/temp/ACS/$1.obj src/code/$1.ds
}

compile_script duh_arti
compile_script duh_claw
compile_script duh_defs
compile_script duh_hell
compile_script duh_mace
compile_script duh_mobj
compile_script duh_prod
compile_script duh_wand
compile_script duh_xbow

echo 'Linking duh.o'
DH-acc --target=ZDoom --named-scripts --out=build/temp/ACS/duh.o build/temp/ACS/*.obj
rm -f build/temp/ACS/*.obj

cp -r -tbuild/temp src/*
cp GPLv3.txt build/temp/COPYING
cp README.txt build/temp/README

( cd build/temp && 7z a -mx ../DUHeresy.pk7 .; )

rm -rf build/temp

# EOF

