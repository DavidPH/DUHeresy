##-----------------------------------------------------------------------------
##
## Copyright (C) 2015 David Hill
##
## See COPYING for license information.
##
##-----------------------------------------------------------------------------
##
##
##
##-----------------------------------------------------------------------------

AS = gdcc-as
CC = gdcc-cc
LD = gdcc-ld


all: build/ build/DUDamned.pkz build/DUHeresy.pkz build/DUMagick.pkz

clean:
	rm -f build/*.bin build/*.ir build/*.pkz

build/:
	mkdir "build"

##
## libGDCC
##

build/libGDCC.ir:
	gdcc-makelib --bc-target=ZDoom -co$@ libGDCC

##
## libc
##

build/libc.ir: $(libc_IR)
	gdcc-makelib --bc-target=ZDoom -co$@ libc

##
## DUCommon
##

DU_DEC = \
   src/DUCommon/code/du_defs.dec

DU_H = \
   src/DUCommon/code/du_defs.h

DU_INC = -isrc/DUCommon/code

DU_IR_CC = \
   build/du_defs.ir \
   build/du_menu.ir

DU_IR = $(DU_IR_CC)

build/DUCommon.ir: build/libGDCC.ir build/libc.ir $(DU_IR)
	$(LD) --bc-target=ZDoom -co$@ $^

$(DU_IR_CC) : build/du_%.ir : src/DUCommon/code/du_%.c $(DU_H)
	$(CC) --bc-target=ZDoom -isrc/DUCommon/code -co$@ $<

##
## DUDamned
##

DUD_DEC = \
   src/DUDamned/code/dud_arti.dec \
   src/DUDamned/code/dud_cell.dec \
   src/DUDamned/code/dud_clip.dec \
   src/DUDamned/code/dud_mobj.dec \
   src/DUDamned/code/dud_rckt.dec \
   src/DUDamned/code/dud_shel.dec \
   src/DUDamned/code/dud_weap.dec

DUD_H = \
   src/DUDamned/code/dud_abil.h \
   src/DUDamned/code/dud_weap.h

DUD_INC = -isrc/DUDamned/code $(DU_INC)

DUD_IR_CC = \
   build/dud_abil.ir \
   build/dud_menu.ir \
   build/dud_mobj.ir \
   build/dud_weap.ir

DUD_IR = $(DUD_IR_CC)

DUD_ROOT = \
   COPYING README \
   src/DUCommon/LOADACS \
   src/DUDamned/DECORATE \
   src/DUDamned/KEYCONF \
   src/DUDamned/LANGUAGE \
   src/DUDamned/TEXTURES \
   src/DUDamned/ZMAPINFO

build/DUDamned.pkz: build/DUDamned.bin $(DU_DEC) $(DUD_DEC) $(DUD_ROOT)
	@rm -f $@
	@rm -rf build/DUDamned

	@mkdir "build/DUDamned"
	@mkdir "build/DUDamned/acs"
	@mkdir "build/DUDamned/code"

	@cp $(DUD_ROOT)          build/DUDamned
	@cp build/DUDamned.bin   build/DUDamned/acs/du.o
	@cp $(DU_DEC) $(DUD_DEC) build/DUDamned/code

	@echo 7z a $@ build/DUDamned
	@cd build/DUDamned && 7z a ../DUDamned.pkz .

	@rm -rf build/DUDamned

build/DUDamned.bin: build/DUCommon.ir build/DUDamned.ir
	$(LD) --bc-target=ZDoom -o$@ $^

build/DUDamned.ir: $(DUD_IR)
	$(LD) --bc-target=ZDoom -co$@ $^

$(DUD_IR_CC) : build/dud_%.ir : src/DUDamned/code/dud_%.c $(DU_H) $(DUD_H)
	$(CC) --bc-target=ZDoom $(DUD_INC) --no-warn-unused-initializer -co$@ $<

##
## DUHeresy
##

DUH_DEC = \
   src/DUHeresy/code/duh_arti.dec \
   src/DUHeresy/code/duh_claw.dec \
   src/DUHeresy/code/duh_gaun.dec \
   src/DUHeresy/code/duh_hell.dec \
   src/DUHeresy/code/duh_mace.dec \
   src/DUHeresy/code/duh_mobj.dec \
   src/DUHeresy/code/duh_prod.dec \
   src/DUHeresy/code/duh_staf.dec \
   src/DUHeresy/code/duh_wand.dec \
   src/DUHeresy/code/duh_xbow.dec

DUH_H = \
   src/DUHeresy/code/duh_arti.h

DUH_INC = -isrc/DUHersey/code $(DU_INC)

DUH_IR_CC = \
   build/duh_arti.ir \
   build/duh_claw.ir \
   build/duh_hell.ir \
   build/duh_mace.ir \
   build/duh_mobj.ir \
   build/duh_prod.ir \
   build/duh_wand.ir \
   build/duh_xbow.ir

DUH_IR = $(DUH_IR_CC)

DUH_ROOT = \
   COPYING README \
   src/DUCommon/LOADACS \
   src/DUHeresy/DECORATE \
   src/DUHeresy/KEYCONF

build/DUHeresy.pkz: build/DUHeresy.bin $(DU_DEC) $(DUH_DEC) $(DUH_ROOT)
	@rm -f $@
	@rm -rf build/DUHeresy

	@mkdir "build/DUHeresy"
	@mkdir "build/DUHeresy/acs"
	@mkdir "build/DUHeresy/code"

	@cp $(DUH_ROOT)          build/DUHeresy
	@cp build/DUHeresy.bin   build/DUHeresy/acs/du.o
	@cp $(DU_DEC) $(DUH_DEC) build/DUHeresy/code

	@echo 7z a $@ build/DUHeresy
	@cd build/DUHeresy && 7z a ../DUHeresy.pkz .

	@rm -rf build/DUHeresy

build/DUHeresy.bin: build/DUCommon.ir build/DUHeresy.ir
	$(LD) --bc-target=ZDoom -o$@ $^

build/DUHeresy.ir: $(DUH_IR)
	$(LD) --bc-target=ZDoom -co$@ $^

$(DUH_IR_CC) : build/duh_%.ir : src/DUHeresy/code/duh_%.c $(DU_H) $(DUH_H)
	$(CC) --bc-target=ZDoom $(DUH_INC) -co$@ $<

##
## DUMagick
##

DUM_DEC = \
   src/DUMagick/code/dum_spel.dec

DUM_H = \
   src/DUMagick/code/dum_spel.h \
   src/DUMagick/code/dum_stan.h

DUM_INC = -isrc/DUMagick/code $(DU_INC)

DUM_IR_CC = \
   build/dum_main.ir \
   build/dum_spel.ir \
   build/dum_stan.ir

DUM_IR = $(DUM_IR_CC)

DUM_ROOT = \
   COPYING README \
   src/DUCommon/LOADACS \
   src/DUMagick/DECORATE

build/DUMagick.pkz: build/DUMagick.bin $(DU_DEC) $(DUM_DEC) $(DUM_ROOT)
	@rm -f $@
	@rm -rf build/DUMagick

	@mkdir "build/DUMagick"
	@mkdir "build/DUMagick/acs"
	@mkdir "build/DUMagick/code"

	@cp $(DUM_ROOT)          build/DUMagick
	@cp build/DUMagick.bin   build/DUMagick/acs/du.o
	@cp $(DU_DEC) $(DUM_DEC) build/DUMagick/code

	@echo 7z a $@ build/DUMagick
	@cd build/DUMagick && 7z a ../DUMagick.pkz .

	@rm -rf build/DUMagick

build/DUMagick.bin: build/DUCommon.ir build/DUMagick.ir
	$(LD) --bc-target=ZDoom -o$@ $^

build/DUMagick.ir: $(DUM_IR)
	$(LD) --bc-target=ZDoom -co$@ $^

$(DUM_IR_CC) : build/dum_%.ir : src/DUMagick/code/dum_%.c $(DU_H) $(DUM_H)
	$(CC) --bc-target=ZDoom $(DUM_INC) -co$@ $<

## EOF

