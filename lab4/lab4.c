#include <minix/drivers.h>
#include <limits.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include "test4.h"
#include "i8042.h"
#include "mouse.h"



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
			"\t service run %s -args \"packet <decimal no.- number of packets>\"\n"
			"\t service run %s -args \"remote <decimal no.- period> <decimal no.- number of packets>\"\n"
			"\t service run %s -args \"async <decimal no. - idle time>\"\n"
			"\t service run %s -args \"gesture < no. - length>\"\n",
			argv[0], argv[0], argv[0]);
}

static int proc_args(int argc, char **argv)
{
	unsigned long packet, idle, period, length;
	if (strncmp(argv[1], "packet", strlen("packet")) == 0) {
		if (argc != 3) {
			printf("test4: wrong no. of arguments for mouse_test_packet()\n");
			return 1;
		}
		packet = parse_ulong(argv[2], 10);						/* Parses string to unsigned long */
		if (packet == ULONG_MAX)
			return 1;
		printf("test4::mouse_test_packet(%lu)\n", packet);
		return mouse_test_packet(packet);
	}

	else if (strncmp(argv[1], "remote", strlen("remote")) == 0) {
		if (argc != 4) {
			printf("test4: wrong no. of arguments for mouse_test_remote()\n");
			return 1;
		}
		period = parse_ulong(argv[2], 10);
		packet = parse_ulong(argv[3], 10);						/* Parses string to unsigned long */
			if (packet == ULONG_MAX)
				return 1;
			if (period == ULONG_MAX)
				return 1;
		printf("test4::mouse_test_remote(%lu , %lu)\n", period, packet);
		return mouse_test_remote(period, packet);
	}

	else if (strncmp(argv[1], "async", strlen("async")) == 0) {
		if (argc != 3) {
			printf("test4: wrong no of arguments for mouse_test_async()\n");
			return 1;
		}
		idle = parse_ulong(argv[2], 10);						// Parses string to unsigned long
		if (idle == ULONG_MAX)
			return 1;
		printf("test4::mouse_test_async(%lu)\n", idle);
		return mouse_test_async(idle);
	}

	else if (strncmp(argv[1], "gesture", strlen("gesture")) == 0) {
		if (argc != 3) {
			printf("test4: wrong no of arguments for mouse_test_gesture()\n");
			return 1;
		}
		length = parse_ulong(argv[2], 10);						// Parses string to unsigned long
		if (length == ULONG_MAX)
			return 1;
		printf("test4::mouse_test_gesture(%lu)\n", length);
		return mouse_test_gesture(length);
		}
	else {
		printf("test4: %s - no valid function!\n", argv[1]);
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
