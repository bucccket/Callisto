

#include "imageloader.h"

#pragma warning(push)
#pragma warning(disable:_UCRT_DISABLED_WARNINGS)

uint32_t readUint32BE(FILE* f, uint32_t offset)
{

	return 0;
}

uint32_t parseUintFromString(FILE*f) {
	char* width = (char*)calloc(1, sizeof(char));
	if (!width) {
		return NULL;
	}
	memset(width, 0, sizeof(char));
	if (!width) {
		return NULL;
	}
	int width_digit;
	while ((width_digit = getc(f)) != 0x0A && width_digit != ' ' && width_digit != EOF) {
		size_t length = strlen(width);
		char* realloc_width = realloc(width, length + 1);
		if (!realloc_width) {
			return NULL;
		}
		width = realloc_width;
		width[length] = 0;
		strncat(width, &width_digit, 1);
	}
	uint32_t result = atoi(width);
	free(width);
}

int openFile(const char* name, pnmImage* image) {

	if (!name) {
		return STATE_INTERNAL_ERROR;
	}

	FILE* f = fopen(name, "rb");
	if (!f) {
		return STATE_INVALID_BUFFER;
	}

	//get length of file
	fseek(f, 0, SEEK_END);
	size_t filelen = ftell(f);
	rewind(f);

	//SKIP HEADER
	fseek(f, 3, SEEK_CUR); //P6 \x0A

	//READ WIDTH
	char* width = (char*)calloc(1, sizeof(char));
	memset(width, 0, sizeof(char));
	if (!width) {
		return STATE_INVALID_BUFFER;
	}
	int width_digit;
	while ((width_digit = getc(f)) != 0x0A && width_digit != ' ' && width_digit != EOF) {
		size_t length = strlen(width);
		char* realloc_width = realloc(width, length + 1);
		if (!realloc_width) {
			return STATE_INVALID_BUFFER;
		}
		width = realloc_width;
		width[length] = 0;
		strncat(width, &width_digit, 1);
	}
	image->width = atoi(width);

	//READ HEIGHT
	char* height = (char*)calloc(1, sizeof(char));
	memset(height, 0, sizeof(char));
	if (!height) {
		return STATE_INVALID_BUFFER;
	}
	int height_digit;
	while ((height_digit = getc(f)) != 0x0A && height_digit != ' ' && height_digit != EOF) {
		size_t length = strlen(height);
		char* realloc_height = realloc(height, length + 1);
		if (!realloc_height) {
			return STATE_INVALID_BUFFER;
		}
		height = realloc_height;
		height[length] = 0;
		strncat(height, &height_digit, 1);
	}
	image->height = atoi(height);

	//READ bpp
	char* bpp = (char*)calloc(1, sizeof(char));
	memset(bpp, 0, sizeof(char));
	if (!bpp) {
		return STATE_INVALID_BUFFER;
	}
	int bpp_digit;
	while ((bpp_digit = getc(f)) != 0x0A && bpp_digit != ' ' && bpp_digit != EOF) {
		size_t length = strlen(bpp);
		char* realloc_bpp = realloc(bpp, length + 1);
		if (!realloc_bpp) {
			return STATE_INVALID_BUFFER;
		}
		bpp = realloc_bpp;
		bpp[length] = 0;
		strncat(bpp, &bpp_digit, 1);
	}
	image->bpp = atoi(bpp);

	off_t fileOffset = 0;
	uint32_t* pixels = (uint32_t*)calloc((size_t)image->width * (size_t)image->height, sizeof(uint32_t));
	if (!pixels) {
		return STATE_INVALID_BUFFER;
	}
	unsigned char color[3] = { 0 };
	while (ftell(f) < filelen) {
		fread(color, 3, 1, f);
		uint32_t byteOffset = fileOffset * sizeof(uint32_t);
		pixels[fileOffset] = (uint32_t)(0xFF << 030 | color[0] << 020 | color[1] << 010 | color[2]);
		fileOffset++;
	}

	image->pixels = pixels;

	printf("%d %d %d\n", image->width, image->height, image->bpp);
	printf("reading %lld bytes from file %s\n", filelen, name);

	return STATE_OK;
}