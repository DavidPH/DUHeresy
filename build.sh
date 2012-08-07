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


ACC_DIR=/data/src/SELF/DH-acc

compile_script()
{
   SRC="$1"
   OBJ="$2"
   shift 2

   echo "Compiling ${SRC}"

   DH-acc --target=ZDoom -c \
          -isrc/DUCommon/code/ -i"${ACC_DIR}"/inc/ \
          --no-string-func \
          -D__LIBDS_NOLIB \
          "$@" -o"${OBJ}" "${SRC}"
}

compile_script_std()
{
   SRC="${ACC_DIR}"/lib/"$1".ds
   OBJ=build/temp/DU/ACS/"$1".obj

   compile_script "${SRC}" "${OBJ}"
   cp "${OBJ}" build/temp/DUD/ACS/"$1".obj
   cp "${OBJ}" build/temp/DUH/ACS/"$1".obj
   cp "${OBJ}" build/temp/DUM/ACS/"$1".obj
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


compile_script_std stdlib
compile_script_std string

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


link_script()
{
   LOWER="$1" ; UPPER="$2"

   echo "Linking ${LOWER}.o"

   DH-acc --target=ZDoom --script-list=- --script-start=256 \
      -obuild/temp/"${UPPER}"/ACS/du.o build/temp/"${UPPER}"/ACS/*.obj |
      awk '{print "const int " $1 " = " $2 ";"}' >build/temp/"${UPPER}"/code/"${LOWER}_defs.dec"

   rm -f build/temp/"${UPPER}"/ACS/*.obj
}

link_script dud DUD
link_script duh DUH
link_script dum DUM


build_mod()
{
   NAME="$1"
   SHRT="$2"

   echo "Building ${NAME}.pk7"

   cp -r -tbuild/temp/"${SHRT}" src/DUCommon/*
   cp -r -tbuild/temp/"${SHRT}" src/"${NAME}"/*
   cp GPLv3.txt build/temp/"${SHRT}"/COPYING
   cp README.txt build/temp/"${SHRT}"/README

   ( cd build/temp/"${SHRT}" && 7z a -mx ../../"${NAME}.pk7" . >/dev/null ; )
}

build_mod DUDamned DUD
build_mod DUHeresy DUH
build_mod DUMagick DUM


rm -rf build/temp

# EOF

