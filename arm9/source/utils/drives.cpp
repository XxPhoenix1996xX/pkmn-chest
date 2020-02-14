#include <fat.h>
#include <nds.h>
#include <stdio.h>
#include <sys/stat.h>

#include "nandio.h"

static bool sdAccessed = false;
static bool sdRead = false;

static bool nandAccessed = false;
static bool nandRead = false;

static bool flashcardAccessed = false;
static bool flashcardRead = false;

bool sdFound(void) {
	if (!sdAccessed) {
		if (access("sd:/", F_OK) == 0) {
			sdRead = true;
		} else {
			sdRead = false;
		}
		sdAccessed = true;
	}
	return sdRead;
}

bool flashcardFound(void) {
	if (!flashcardAccessed) {
		if (access("fat:/", F_OK) == 0) {
			flashcardRead = true;
		} else {
			flashcardRead = false;
		}
		flashcardAccessed = true;
	}
	return flashcardRead;
}

bool nandFound(void) {
	if (!nandAccessed) {
		if (access("nand:/", F_OK) == 0) {
			nandRead = true;
		} else {
			nandRead = false;
		}
		nandAccessed = true;
	}
	return nandRead;
}

bool bothSDandFlashcard(void) {
	if (sdFound() && flashcardFound()) {
		return true;
	} else {
		return false;
	}
}

TWL_CODE bool nandMount(void) {
	fatMountSimple("nand", &io_dsi_nand);
	if (nandFound()) {
		return true;
	}
	return false;
}
