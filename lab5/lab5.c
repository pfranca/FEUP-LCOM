#include <minix/drivers.h>
#include <limits.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include "test5.h"
#include "pixmap.h"
//#include "i8042.h"
//#include "mouse.h"



static int proc_args(int argc, char **argv);
static unsigned long parse_ulong(char *str, int base);
static void print_usage(char **argv);

int main(int argc, char **argv)
{
	/* DO NOT FORGET TO initialize service */
	sef_startup();


	if (argc == 1) {					/* Prints usage of the program if no arguments are passed */
		print_usage(argv);
		return 0;
	}
	else return proc_args(argc, argv);
}

static void print_usage(char **argv)
{
	printf("Usage: one of the following:\n"
			"\t service run %s -args \"init <decimal no.- modes> <decimal no.- delay>\"\n"
			"\t service run %s -args \"square <decimal no.- x> <decimal no.- y> <decimal no.- size> <decimal no.- color>\"\n"
			"\t service run %s -args \"line <decimal no. - xi> <decimal no. - yi> <decimal no. - xf> <decimal no. - yf> <decimal no. - color>\"\n"
			"\t service run %s -args \"xpm <decimal no. - xi> <decimal no. - yi> <decimal no. - xpm>\"\n"
			"\t service run %s -args \"move <decimal no. - xi> <decimal no. - yi> <decimal no. - xpm> <decimal no. - hor> <decimal no. - delta> <decimal no. - time>\"\n"
			"\t service run %s -args \"controller\"\n",
			argv[0], argv[0], argv[0],argv[0],argv[0],argv[0]);
}

static int proc_args(int argc, char **argv)
{
	unsigned long mode, delay, x, y, xf, yf, size, color,xi,yi,s,f;
	char **xpm;

	if (strncmp(argv[1], "init", strlen("init")) == 0) {
		if (argc != 4) {
			printf("test5: wrong no. of arguments for video_test_init()\n");
			return 1;
		}
		mode = parse_ulong(argv[2], 16);						/* Parses string to unsigned long */
		delay = parse_ulong(argv[3], 10);
		if (mode == ULONG_MAX)
			return 1;
		if (delay == ULONG_MAX)
					return 1;
		printf("test5::video_test_init(%lu, %lu)\n", mode, delay);
		return video_test_init(mode, delay);
	}

	else if (strncmp(argv[1], "square", strlen("square")) == 0) {
		if (argc != 6) {
			printf("test5: wrong no. of arguments for video_test_square()\n");
			return 1;
		}
		x = parse_ulong(argv[2], 10);
		y = parse_ulong(argv[3], 10);
		size = parse_ulong(argv[4], 10);
		color = parse_ulong(argv[5], 16);
		if (x == ULONG_MAX)
			return 1;
		if (y == ULONG_MAX)
			return 1;
		if (size == ULONG_MAX)
			return 1;
		if (color == ULONG_MAX)
			return 1;
		printf("test5::video_test_square(%lu , %lu, %lu, %lu)\n", x, y, size, color);
		return video_test_square(x, y, size, color);
	}

	else if (strncmp(argv[1], "line", strlen("line")) == 0) {
		if (argc != 7) {
			printf("test5: wrong no of arguments for video_test_line()\n");
			return 1;
		}
		x = parse_ulong(argv[2], 10);
		y = parse_ulong(argv[3], 10);
		xf = parse_ulong(argv[4], 10);
		yf = parse_ulong(argv[5], 10);
		color = parse_ulong(argv[6], 16);
		if (x == ULONG_MAX)
			return 1;
		if (y == ULONG_MAX)
			return 1;
		if (xf == ULONG_MAX)
			return 1;
		if (yf == ULONG_MAX)
			return 1;
		if (color == ULONG_MAX)
			return 1;
		printf("test5::video_test_line(%lu, %lu , %lu, %lu, %lu)\n", x, y, xf, yf, color);
		return video_test_line(x, y, xf, yf, color);
	}
	else if (strncmp(argv[1], "xpm", strlen("xpm")) == 0) {
		if (argc != 5) {
			printf("test5: wrong no of arguments for video_test_xpm()\n");
			return 1;
		}
		if (strncmp(argv[2], "pic1", strlen("pic1")) == 0) {
			xpm = pic1;
		} else if (strncmp(argv[2], "pic2", strlen("pic2")) == 0) {
			xpm = pic2;
		} else if (strncmp(argv[2], "cross", strlen("cross")) == 0) {
			xpm = cross;
		} else if (strncmp(argv[2], "pic3", strlen("pic3")) == 0) {
			xpm = pic3;
		} else if (strncmp(argv[2], "penguin", strlen("penguin")) == 0) {
			xpm = penguin;
		} else {
			printf("video_card: invalid arguments for video_test_xpm(xpm,xi,yi)\n");
			return 1;
		}
		xi = parse_ulong(argv[3], 10);
		yi = parse_ulong(argv[4], 10);
		if (xi == ULONG_MAX)
			return 1;
		if (yi == ULONG_MAX)
			return 1;
		if (xi < 0 || yi < 0) {
			printf("video_card: invalid arguments for video_test_xpm(xpm,xi,yi)\n");
			return 1;
		}
		printf("test5::video_test_xpm(%s, %lu , %lu)\n", xpm,xi,yi);
		return video_test_xpm(xpm,xi,yi);
	}
	else if (strncmp(argv[1], "move", strlen("move")) == 0) {
		if (argc != 9) {
			printf("test5: wrong no of arguments for video_test_move()\n");
			return 1;
		}
		if (strncmp(argv[2], "pic1", strlen("pic1")) == 0) {
			xpm = pic1;
		} else if (strncmp(argv[2], "pic2", strlen("pic2")) == 0) {
			xpm = pic2;
		} else if (strncmp(argv[2], "cross", strlen("cross")) == 0) {
			xpm = cross;
		} else if (strncmp(argv[2], "pic3", strlen("pic3")) == 0) {
			xpm = pic3;
		} else if (strncmp(argv[2], "penguin", strlen("penguin")) == 0) {
			xpm = penguin;
		} else {
			printf("video_card: invalid arguments for video_test_move(xpm,xi,yi,xf,yf,s,f)\n");
			return 1;
		}
		xi = parse_ulong(argv[3], 10);
		yi = parse_ulong(argv[4], 10);
		xf = parse_ulong(argv[5], 10);
		yf = parse_ulong(argv[6], 10);
		if (xi == ULONG_MAX)
			return 1;
		if (yi == ULONG_MAX)
			return 1;
		if (xf == ULONG_MAX)
				return 1;
		if (yf == ULONG_MAX)
			return 1;
		if (xi < 0 || yi < 0 || xf < 0 || yf < 0 || xi > 1024 || yi > 768 ||xf > 1024 || yf > 768) {
			printf("video_card: invalid arguments for video_test_move(xpm,xi,yi,xf,yf,s,f)\n");
			return 1;
		}
		s = parse_ulong(argv[7], 10);
		if (s == ULONG_MAX)
			return 1;
		f = parse_ulong(argv[8], 10);
		if (f == ULONG_MAX)
			return 1;


		printf("test5::video_test_move(%s, %lu , %lu, %lu, %lu, %lu, %lu)\n", xpm,xi,yi,xf,yf,s,f);
		return video_test_move(xpm,xi,yi,xf,yf,s,f);
		}


	else {
		printf("test5: %s - no valid function!\n", argv[1]);
		return 1;
	}
}

static unsigned long parse_ulong(char *str, int base)
{
	char *endptr;
	unsigned long val;

	/* Convert string to unsigned long */
	val = strtoul(str, &endptr, base);

	/* Check for conversion errors */
	if ((errno == ERANGE && val == ULONG_MAX) || (errno != 0 && val == 0)) {
		perror("strtoul");
		return ULONG_MAX;
	}

	if (endptr == str) {
		printf("teste3: parse_ulong: no digits were found in %s\n", str);
		return ULONG_MAX;
	}

	/* Successful conversion */
	return val;
}
