PROG=proj

SRCS = proj.c video_card.c bitmap.c

CPPFLAGS += -pedantic #-D __LCOM_OPTIMIZED__

DPADD += ${LIBLCF} ${LIBLM}
LDADD += -llcf -llm

.include <minix.lcom.mk>
