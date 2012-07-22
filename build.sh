#!/bin/sh

set -e

cd "$(dirname "$0")"

rm -rf build/temp build/DUDamned.pk7 build/DUHeresy.pk7 build/DUMagick.pk7
mkdir build/temp
mkdir build/temp/DU
mkdir build/temp/DU/ACS
mkdir build/temp/DUD
mkdir build/temp/DUD/ACS
mkdir build/temp/DUD/code
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

   DH-acc --target=ZDoom -c \
          -isrc/DUCommon/code/ -i/data/src/SELF/DH-acc/inc/ \
          --no-string-func \
          -D__LIBDS_NOLIB \
          "$@" -o"${OBJ}" "${SRC}"
}

compile_script_du()
{
   SRC=src/DUCommon/code/"$1".ds
   OBJ=build/temp/DU/ACS/"$1".obj

   compile_script "${SRC}" "${OBJ}"
   cp "${OBJ}" build/temp/DUD/ACS/"$1".obj
   cp "${OBJ}" build/temp/DUH/ACS/"$1".obj
   cp "${OBJ}" build/temp/DUM/ACS/"$1".obj
}

compile_script_dud()
{
   SRC=src/DUDamned/code/"$1".ds
   OBJ=build/temp/DUD/ACS/"$1".obj

   compile_script "${SRC}" "${OBJ}" -isrc/DUDamned/code/
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
compile_script_du du_menu

compile_script_dud dud_abil
compile_script_dud dud_weap

compile_script_duh duh_arti
compile_script_duh duh_claw
compile_script_duh duh_hell
compile_script_duh duh_mace
compile_script_duh duh_mobj
compile_script_duh duh_prod
compile_script_duh duh_wand
compile_script_duh duh_xbow

compile_script_dum dum_main
compile_script_dum dum_spel
compile_script_dum dum_stan


# DUDamned.pk7
echo 'Linking dud.o'
DH-acc --target=ZDoom --script-list=- \
   -obuild/temp/DUD/ACS/du.o build/temp/DUD/ACS/*.obj |
   awk '{print "const int " $1 " = " $2 ";"}' >build/temp/DUD/code/dud_defs.dec
rm -f build/temp/DUD/ACS/*.obj

cp -r -tbuild/temp/DUD src/DUCommon/*
cp -r -tbuild/temp/DUD src/DUDamned/*
cp GPLv3.txt build/temp/DUD/COPYING
cp README.txt build/temp/DUD/README

( cd build/temp/DUD && 7z a -mx ../../DUDamned.pk7 .; )


# DUHeresy.pk7
echo 'Linking duh.o'
DH-acc --target=ZDoom --script-list=- \
   -obuild/temp/DUH/ACS/du.o build/temp/DUH/ACS/*.obj |
   awk '{print "const int " $1 " = " $2 ";"}' >build/temp/DUH/code/duh_defs.dec
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
   awk '{print "const int " $1 " = " $2 ";"}' >build/temp/DUM/code/dum_defs.dec
rm -f build/temp/DUM/ACS/*.obj

cp -r -tbuild/temp/DUM src/DUCommon/*
cp -r -tbuild/temp/DUM src/DUMagick/*
cp GPLv3.txt build/temp/DUM/COPYING
cp README.txt build/temp/DUM/README

( cd build/temp/DUM && 7z a -mx ../../DUMagick.pk7 .; )


rm -rf build/temp

# EOF

