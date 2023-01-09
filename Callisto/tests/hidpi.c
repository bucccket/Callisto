#include <MiniFB.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "imageloader.h"

#define DIMEN 1024
static uint32_t g_buffer_custom[DIMEN * DIMEN];

void custom_draw(unsigned int* p, int dimen, pnmImage* image) {
	memset(p, (uint32_t)(0xFFEE8B64), dimen * dimen * 4);
	for (int x = 0; x < dimen; x++)
		for (int y = 0; y < dimen; y++) {
			p[y * dimen + x] = MFB_ARGB(0xFF, x, y, 0xFF >> 1);
			if (x < image->width && y < image->height) {
				//image->pixels[y * image->height + x] & 0xBBGGRRAA
				p[y * dimen + x] = image->pixels[y * image->height + x];
			}
		}
}

int main() {

	char directory[_MAX_PATH];
	GetCurrentDirectoryA(_MAX_PATH, directory);
	printf("%s\n", directory);

	pnmImage* image = (pnmImage*)calloc(1, sizeof(pnmImage));
	if (!image) {
		return STATE_EXIT;
	}

	int error;
	if ((error = openFile("img\\luPushups.pnm", image))) {
		switch (error) {
		case STATE_INTERNAL_ERROR:
			printf("internal error\n");
			break;
		case STATE_INVALID_BUFFER:
			printf("invalid buffer error\n");
			break;
		case STATE_INVALID_WINDOW:
			printf("invalid window error\n");
			break;
		default:
			printf("unkown error opening file\n");
		}
	}

	custom_draw(g_buffer_custom, DIMEN, image);
	struct mfb_window* window_custom = mfb_open("custom", DIMEN / 2, DIMEN / 2);

	while (window_custom) {
		if (window_custom)
			if (mfb_update_ex(window_custom, g_buffer_custom, DIMEN, DIMEN) != STATE_OK)
				window_custom = NULL;

		if (window_custom)
			mfb_wait_sync(window_custom);
	}
	return 0;
}
