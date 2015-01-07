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

GDCC_LIB_PATH = /usr/share/gdcc/lib


all: build/DUHeresy.pkz

clean:
	rm -f build/*.bin build/*.ir build/*.pkz

##
## libGDCC
##

libGDCC_IR_AS = \
   build/libGDCC_AddF.ir \
   build/libGDCC_AddU.ir \
   build/libGDCC_Bclz.ir \
   build/libGDCC_CmpF.ir \
   build/libGDCC_CmpI.ir \
   build/libGDCC_CmpU.ir \
   build/libGDCC_DivF.ir \
   build/libGDCC_DivI.ir \
   build/libGDCC_DivU.ir \
   build/libGDCC_MulF.ir \
   build/libGDCC_MulU.ir \
   build/libGDCC_Sh.ir \
   build/libGDCC_SubF.ir \
   build/libGDCC_SubU.ir

libGDCC_IR_CC = \
   build/libGDCC_alloc.ir \
   build/libGDCC_format.ir

libGDCC_IR = $(libGDCC_IR_AS) $(libGDCC_IR_CC)

build/libGDCC.ir: $(libGDCC_IR)
	$(LD) -co$@ $^

$(libGDCC_IR_AS) : build/libGDCC_%.ir : $(GDCC_LIB_PATH)/src/libGDCC/ZDACS/%.asm
	$(AS) --bc-target=ZDoom -o$@ $<

$(libGDCC_IR_CC) : build/libGDCC_%.ir : $(GDCC_LIB_PATH)/src/libGDCC/%.c
	$(CC) --bc-target=ZDoom -o$@ $<

##
## libc
##

libc_IR_AS = \
   build/libc_approx.ir \
   build/libc_fpclassify.ir

libc_IR_CC = \
   build/libc_ctype.ir \
   build/libc_errno.ir \
   build/libc_exp.ir \
   build/libc_locale.ir \
   build/libc_math.ir \
   build/libc_printf.ir \
   build/libc_round.ir \
   build/libc_sort.ir \
   build/libc_stdfix.ir \
   build/libc_stdio.ir \
   build/libc_stdlib.ir \
   build/libc_string.ir \
   build/libc_trig.ir

libc_IR = $(libc_IR_AS) $(libc_IR_CC)

build/libc.ir: $(libc_IR)
	$(LD) -co$@ $^

$(libc_IR_AS) : build/libc_%.ir : $(GDCC_LIB_PATH)/src/libc/ZDACS/%.asm
	$(AS) --bc-target=ZDoom -o$@ $<

$(libc_IR_CC) : build/libc_%.ir : $(GDCC_LIB_PATH)/src/libc/%.c
	$(CC) --bc-target=ZDoom -o$@ $<

##
## DUCommon
##

DUC_DEC = \
   src/DUCommon/code/du_defs.dec

DUC_H = \
   src/DUCommon/code/du_defs.h

DUC_INC = -isrc/DUCommon/code

DUC_IR_CC = \
   build/du_defs.ir

DUC_IR = $(DUC_IR_CC)

build/DUCommon.ir: build/libGDCC.ir build/libc.ir $(DUC_IR)
	$(LD) --bc-target=ZDoom -co$@ $^

$(DUC_IR_CC) : build/du_%.ir : src/DUCommon/code/du_%.c $(DUC_H)
	$(CC) --bc-target=ZDoom -isrc/DUCommon/code -o$@ $<

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

DUH_INC = -isrc/DUHersey/code $(DUC_INC)

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

build/DUHeresy.pkz: build/DUHeresy.bin $(DUC_DEC) $(DUH_DEC)
	@rm -f $@
	@rm -rf build/DUHeresy

	@mkdir build/DUHeresy
	@mkdir build/DUHeresy/acs
	@mkdir build/DUHeresy/code

	@cp COPYING README        build/DUHeresy
	@cp src/DUCommon/LOADACS  build/DUHeresy
	@cp src/DUHeresy/DECORATE build/DUHeresy
	@cp src/DUHeresy/KEYCONF  build/DUHeresy
	@cp build/DUHeresy.bin    build/DUHeresy/acs/du.o
	@cp $(DUC_DEC) $(DUH_DEC) build/DUHeresy/code

	@echo 7z a $@ build/DUHeresy
	@cd build/DUHeresy && 7z a ../DUHeresy.pkz . >/dev/null

	@rm -rf build/DUHeresy

build/DUHeresy.bin: build/DUCommon.ir build/DUHeresy.ir
	$(LD) --bc-target=ZDoom -o$@ $^

build/DUHeresy.ir: $(DUH_IR)
	$(LD) --bc-target=ZDoom -co$@ $^

$(DUH_IR_CC) : build/duh_%.ir : src/DUHeresy/code/duh_%.c $(DUC_H) $(DUH_H)
	$(CC) --bc-target=ZDoom $(DUH_INC) -o$@ $<

## EOF

