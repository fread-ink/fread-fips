#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <png.h>

#define HELP_TEXT \
	"fread.ink print screen tool\n"                                       \
	"usage:\n"                                                            \
	"to show image:    fips -g image_path [-f]\n"                         \
	"                    -g for png\n"                                    \
	"                    -f for full update, default is partial update\n" \
	"to clear display: fips -c\n\n"

#define VERSION "0.1"

void print_help() {
	printf(HELP_TEXT);
}

void clear_display() {
	system("echo 0 > /proc/eink_fb/update_display");
}

void update_display(bool full_update) {
	if (full_update) {
		system("echo 2 > /proc/eink_fb/update_display");
	} else {
		system("echo 1 > /proc/eink_fb/update_display");
	}
}

void display_png(char *image_path, bool full_update) {
	int width, height;
	png_byte color_type;
	png_byte bit_depth;
	png_bytep *row_pointers;

	FILE *fpng = fopen(image_path, "rb");

	png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL,
			NULL);
	if (!png)
		abort();

	png_infop info = png_create_info_struct(png);
	if (!info)
		abort();

	if (setjmp(png_jmpbuf(png)))
		abort();

	png_init_io(png, fpng);

	png_read_info(png, info);

	width = png_get_image_width(png, info);
	height = png_get_image_height(png, info);
	color_type = png_get_color_type(png, info);
	bit_depth = png_get_bit_depth(png, info);

	if (color_type == PNG_COLOR_TYPE_RGB
			|| color_type == PNG_COLOR_TYPE_RGB_ALPHA) {
		png_set_rgb_to_gray(png, 2, -1, -1);
	}

	if (color_type == PNG_COLOR_TYPE_PALETTE) {
		png_set_palette_to_rgb(png);
		png_set_rgb_to_gray(png, 2, -1, -1);
	}

	if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8) {
		png_set_expand_gray_1_2_4_to_8(png);
	}

	row_pointers = (png_bytep*) malloc(sizeof(png_bytep) * height);
	for (int y = 0; y < height; y++) {
		row_pointers[y] = (png_byte*) malloc(png_get_rowbytes(png, info));
	}

	png_read_image(png, row_pointers);

	fclose(fpng);

	FILE *fb = fopen("/dev/fb0", "wb");

	for (int y = 0; y < height; y++) {
		png_bytep row = row_pointers[y];
		for (int x = 0; x < width; x++) {
			png_bytep px = &(row[x]);
			int inverted_px = abs(255 - px[0]);
			fputc(inverted_px, fb);
		}
	}

	fclose(fb);

	update_display(full_update);
}

int main(int argc, char **argv) {
	if (argc == 0) {
		print_help();
		exit(0);
	}

	char *image_path = NULL;
	bool full_update = false;

	int option = 0;
	while ((option = getopt(argc, argv, "g:fc")) != -1) {
		switch (option) {
		case 'g':
			image_path = optarg;
			break;
		case 'f':
			full_update = true;
			break;
		case 'c':
			clear_display();
			exit(0);
		default:
			print_help();
			exit(1);
		}
	}

	if (image_path) {
		display_png(image_path, full_update);
	} else {
		print_help();
		exit(1);
	}
}

