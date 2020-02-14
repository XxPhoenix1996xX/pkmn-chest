#---------------------------------------------------------------------------------
.SUFFIXES:
#---------------------------------------------------------------------------------
ifeq ($(strip $(DEVKITARM)),)
$(error "Please set DEVKITARM in your environment. export DEVKITARM=<path to>devkitARM")
endif

export TARGET	:= pkmn-chest
export TOPDIR	:= $(CURDIR)

# specify a directory which contains the nitro filesystem
# this is relative to the Makefile
NITRO_FILES		:= nitrofiles

# These set the information text in the nds file
GAME_TITLE		:= Pokemon Chest
GAME_SUBTITLE1	:= Universal-Team

include $(DEVKITARM)/ds_rules

.PHONY: checkarm9 graphics lang sound clean

#---------------------------------------------------------------------------------
# main targets
#---------------------------------------------------------------------------------
all	:	checkarm9 $(TARGET).nds

skip-gs	:	checkarm9 lang $(NITRO_FILES) arm7/$(TARGET).elf arm9/$(TARGET).elf
	ndstool	-c $(TARGET).nds -7 arm7/$(TARGET).elf -9 arm9/$(TARGET).elf \
	-b1 icon.bmp "$(GAME_TITLE);$(GAME_SUBTITLE1)" $(_ADDFILES) \
	-z 80040000 -u 00030004 -a 00000138

#---------------------------------------------------------------------------------
checkarm9:
	$(MAKE) -C arm9

#---------------------------------------------------------------------------------
graphics:
	$(MAKE) -C graphics

LANG_DIRS	:= eng fre ger ita jpn kor spa
LANG_FILES	:= abilities.txt games.txt items.txt locations*.txt moves.txt natures.txt species.txt

#---------------------------------------------------------------------------------
lang:
	$(foreach dir, $(LANG_DIRS), cp $(foreach file, $(LANG_FILES), arm9/core/strings/$(dir)/$(file)) $(NITRO_FILES)/i18n/$(dir);)
	@echo i18n strings ...

#---------------------------------------------------------------------------------
sound:
	$(MAKE) -C sound

#---------------------------------------------------------------------------------
$(TARGET).nds	: graphics sound $(NITRO_FILES) arm7/$(TARGET).elf arm9/$(TARGET).elf
	ndstool	-c $(TARGET).nds -7 arm7/$(TARGET).elf -9 arm9/$(TARGET).elf \
	-b1 icon.bmp "$(GAME_TITLE);$(GAME_SUBTITLE1)" $(_ADDFILES) \
	-z 80040000 -u 00030004 -a 00000138

#---------------------------------------------------------------------------------
arm9/$(TARGET).elf:
	$(MAKE) -C arm9

arm7/$(TARGET).elf:
	$(MAKE) -C arm7

cia	:	$(NITRO_FILES) arm7/$(TARGET).elf arm9/$(TARGET).elf
	ndstool	-c $(TARGET).temp -7 arm7/$(TARGET).elf -9 arm9/$(TARGET).elf \
	-b1 icon.bmp "$(GAME_TITLE);$(GAME_SUBTITLE1)" $(_ADDFILES) \
	-g PKCH 01 "PKMN-CHEST" -z 80040000 -u 00030004 -a 00000138
	./make_cia.exe --srl="pkmn-chest.temp"
	rm pkmn-chest.temp

#---------------------------------------------------------------------------------
clean:
	$(MAKE) -C arm9 clean
	$(MAKE) -C graphics clean
	rm -rf nitrofiles/i18n/*/*.txt
	$(MAKE) -C sound clean
	rm -f $(TARGET).nds $(TARGET).arm9
