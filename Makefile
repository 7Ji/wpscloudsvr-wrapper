BINARY := wpscloudsvr
LIBS := gtk+-3.0 appindicator3-0.1
CFLAGS += -Wall -Wextra
CFLAGS += $(shell pkgconf --cflags ${LIBS})
LDFLAGS += $(shell pkgconf --libs ${LIBS})
INSTALL_PATH := ${DESTDIR}/opt/kingsoft/wps-office/office6/wpscloudsvr

${BINARY}: ${BINARY}.c
	${CC} ${CFLAGS} ${LDFLAGS} -o $@ $^

clean:
	rm -f ${BINARY}

install: ${BINARY}
ifeq ("$(wildcard ${INSTALL_PATH}.real)", "")
	mv ${INSTALL_PATH} ${INSTALL_PATH}.real
endif
	install -DTm755 ${BINARY} ${INSTALL_PATH}

.PHONY: clean install