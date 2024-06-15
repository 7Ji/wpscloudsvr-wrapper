BINARY := wpscloudsvr
LIBS := gtk+-3.0 appindicator3-0.1
CFLAGS += -Wall -Wextra -Wno-deprecated-declarations
CFLAGS += $(shell pkgconf --cflags ${LIBS})
LDFLAGS += $(shell pkgconf --libs ${LIBS})
SVR_PATH ?= /opt/kingsoft/wps-office/office6/wpscloudsvr
INSTALL_PATH ?= ${DESTDIR}${SVR_PATH}

${BINARY}: ${BINARY}.c
	${CC} ${CFLAGS} ${LDFLAGS} -DSVR_PATH=\"${SVR_PATH}\"  -o $@ $^

clean:
	rm -f ${BINARY}

install: ${BINARY}
ifeq ("$(wildcard ${INSTALL_PATH}.real)", "")
	mv ${INSTALL_PATH} ${INSTALL_PATH}.real
endif
	install -DTm755 ${BINARY} ${INSTALL_PATH}

.PHONY: clean install