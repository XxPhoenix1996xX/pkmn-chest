#include "ndsHeader.h"
#include "fileBrowse.h"
#include "graphics.h"
#include <fstream>
#include "colors.h"
#include "utils.hpp"

void loadIcon(NDSBanner &ndsBanner, std::vector<u16> &imageBuffer) {
	imageBuffer = std::vector<u16>(32*32);

	// Load pixels
	for (size_t row = 0; row < 4; row++) {
		for (size_t col = 0; col < 4; col ++) {
			for (size_t y = 0; y < 8; y++) {
				for (size_t x = 0; x < 8; x += 2) {
					imageBuffer[((row*8+y)*32)+col*8+x] = ndsBanner.palette[ndsBanner.icon[row][col][y][x/2] & 0xF] | BIT(15);
					imageBuffer[((row*8+y)*32)+col*8+x+1] = ndsBanner.palette[ndsBanner.icon[row][col][y][x/2] >> 4] | BIT(15);
				}
			}
		}
	}
}

void getIconTitle(std::string name, std::vector<u16> &imageBuffer, std::string &title) {
	if(nameEndsWith(name, {"nds", "dsi", "ids", "app"})) {
		NDSHeader ndsHeader;
		NDSBanner ndsBanner;

		FILE *rom = fopen(name.c_str(), "rb");
		if(!rom) return;

		fseek(rom, 0, SEEK_SET); // Seek to the start of the file
		fread(&ndsHeader, sizeof(ndsHeader), 1, rom); // Read the header
		fseek(rom, ndsHeader.bannerOffset, SEEK_SET); // Seek to the banner
		fread(&ndsBanner, sizeof(ndsBanner), 1, rom); // Read the banner

		// Icon
		loadIcon(ndsBanner, imageBuffer);

		// Title
		title = StringUtils::UTF16toUTF8((char16_t*)ndsBanner.titles[0]);
	}
}