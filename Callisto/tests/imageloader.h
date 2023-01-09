#pragma once

#include <MiniFB.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>

typedef struct pnmImage pnmImage;

struct pnmImage {
	uint32_t width;
	uint32_t height;
	uint32_t bpp;
	uint32_t* pixels;
};

uint32_t readUint32BE(FILE* f, uint32_t offset);

int openFile(const char* name, pnmImage* buffer);