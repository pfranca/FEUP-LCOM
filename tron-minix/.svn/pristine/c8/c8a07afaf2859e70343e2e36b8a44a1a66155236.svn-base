# Makefile for lab5

COMPILER_TYPE= gnu

CC= gcc

PROG= tron-minix
SRCS= video_gr.c keyboard.c timer.c mouse.c tron.c vbe.c bike.c main.c stbi_wrapper.c menu.c mainMenu.c

CFLAGS= -Wall

DPADD+= ${LIBDRIVER} ${LIBSYS}
LDADD+= -llm -ldriver -lsys

LDFLAGS+= -L .

MAN=

.include <bsd.prog.mk>
.include <bsd.gcc.mk>
