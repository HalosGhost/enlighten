CFLAGS	+=	-Os -Wall
PROG	=	enlighten
PREFIX	?=	/usr
VER		=	1.0

${PROG}: ${PROG}.c
	@${CC} ${CFLAGS} -o ${PROG} ${PROG}.c
	@strip ${PROG}

clean:
	@rm -f ${PROG}

install: ${PROG}
	@install -Dm755 ${PROG} ${DESTDIR}${PREFIX}/bin/${PROG}
	@install -Dm644 _${PROG} ${DESTDIR}${PREFIX}/share/zsh/site-functions/_${PROG}
