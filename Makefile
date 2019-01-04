PROG=proj

SRCS = proj.c video_card.c bitmap.c timer.c keyboard.c mouse_ih.c game.c RTC.c serial.c

CPPFLAGS += -pedantic -D __LCOM_OPTIMIZED__

DPADD += ${LIBLCF} ${LIBLM}
LDADD += -llcf -llm

.include <minix.lcom.mk>
