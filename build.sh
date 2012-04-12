#!/bin/sh

set -e

cd "$(dirname "$0")"

rm -rf build/temp build/DUHeresy.pk7
mkdir build/temp
mkdir build/temp/DU
mkdir build/temp/DU/ACS
mkdir build/temp/DUH
mkdir build/temp/DUH/ACS
mkdir build/temp/DUH/code
mkdir build/temp/DUM
mkdir build/temp/DUM/ACS
mkdir build/temp/DUM/code


compile_script()
{
   SRC="$1"
   OBJ="$2"
   shift 2

   echo "Compiling ${SRC}"

   DH-acc --target=ZDoom --output=object \
          -isrc/DUCommon/code/ -i/data/src/SELF/DH-acc/inc/ \
          --no-string-func \
          "$@" -o"${OBJ}" "${SRC}"
}

compile_script_du()
{
   SRC=src/DUCommon/code/"$1".ds
   OBJ=build/temp/DU/ACS/"$1".obj

   compile_script "${SRC}" "${OBJ}"
   cp "${OBJ}" build/temp/DUH/ACS/"$1".obj
   cp "${OBJ}" build/temp/DUM/ACS/"$1".obj
}

compile_script_duh()
{
   SRC=src/DUHeresy/code/"$1".ds
   OBJ=build/temp/DUH/ACS/"$1".obj

   compile_script "${SRC}" "${OBJ}" -isrc/DUHeresy/code/
}

compile_script_dum()
{
   SRC=src/DUMagick/code/"$1".ds
   OBJ=build/temp/DUM/ACS/"$1".obj

   compile_script "${SRC}" "${OBJ}" -isrc/DUMagick/code/
}


compile_script_du du_defs

compile_script_duh duh_arti
compile_script_duh duh_claw
compile_script_duh duh_hell
compile_script_duh duh_mace
compile_script_duh duh_mobj
compile_script_duh duh_prod
compile_script_duh duh_wand
compile_script_duh duh_xbow

compile_script_dum dum_spel


# DUHeresy.pk7
echo 'Linking duh.o'
DH-acc --target=ZDoom --script-list=- \
   -obuild/temp/DUH/ACS/du.o build/temp/DUH/ACS/*.obj |
   awk '{print "const int " $2 " = " $3 ";"}' >build/temp/DUH/code/duh_defs.dec
rm -f build/temp/DUH/ACS/*.obj

cp -r -tbuild/temp/DUH src/DUCommon/*
cp -r -tbuild/temp/DUH src/DUHeresy/*
cp GPLv3.txt build/temp/DUH/COPYING
cp README.txt build/temp/DUH/README

( cd build/temp/DUH && 7z a -mx ../../DUHeresy.pk7 .; )


# DUMagick.pk7
echo 'Linking dum.o'
DH-acc --target=ZDoom --script-start=300 --script-list=- \
   -obuild/temp/DUM/ACS/du.o build/temp/DUM/ACS/*.obj |
   awk '{print "const int " $2 " = " $3 ";"}' >build/temp/DUM/code/dum_defs.dec
rm -f build/temp/DUM/ACS/*.obj

cp -r -tbuild/temp/DUM src/DUCommon/*
cp -r -tbuild/temp/DUM src/DUMagick/*
cp GPLv3.txt build/temp/DUM/COPYING
cp README.txt build/temp/DUM/README

( cd build/temp/DUM && 7z a -mx ../../DUMagick.pk7 .; )


rm -rf build/temp

# EOF

