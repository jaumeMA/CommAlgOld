include ../compiler.mk

LIB_DIR = -L/usr/local/lib -L/usr/lib:/usr/lib/i386-linux-gnu -L${LOCAL_DIR}/${MODULES}
SRC_DIR = .
SUBDIRS = ${SRC_DIR}/memory ${SRC_DIR}/thread ${SRC_DIR}/file
OBJ_DIR = ../bin/${Mode}
OBJ_SUBDIRS= ${OBJ_DIR}/memory ${OBJ_DIR}/thread ${OBJ_DIR}/file

BIN_DIR = ${LOCAL_DIR}
BIN = libSystem${LIBRARY_SUFFIX}.so

TOTAL_LD_FLAGS=${LD_FLAGS} -ldl

${OBJ_DIR}/%: ${SRC_DIR}/%
	mkdir -p $@
	cd $< && make build 

${BIN}: ${OBJ_SUBDIRS}
	$(eval OBJECTS = $(shell find $^ -name *.o))
	${LINKER} ${TOTAL_LD_FLAGS} -L${LIB_DIR} -o $@ $(OBJECTS) ${LIB} && cd ../

clean_bin:
	rm -f ${BIN_DIR}/${BIN}

clean_obj:
	rm -rf ${OBJ_SUBDIRS}

clean_all: clean_obj clean_bin

build: ${BIN}

rebuild: clean_all build