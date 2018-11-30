include ../compiler.mk

LIB_DIR = /usr/local/lib:/usr/lib:/usr/lib/i386-linux-gnu
SRC_DIR = .
SUBDIRS = ${SRC_DIR}/container ${SRC_DIR}/functional ${SRC_DIR}/mpl ${SRC_DIR}/types ${SRC_DIR}/signal
OBJ_DIR = ../bin/${Mode}
OBJ_SUBDIRS= ${OBJ_DIR}/container ${OBJ_DIR}/functional ${OBJ_DIR}/mpl ${OBJ_DIR}/types ${OBJ_DIR}/signal 

BIN = libYTL${LIBRARY_SUFFIX}.so

OBJECTS=
CURR_OBJ_SUBDIR=

#targets for full compilation chain
${OBJ_DIR}/%: ${SRC_DIR}/%
	mkdir -p $@
	cd $< && make build 

${BIN}: ${OBJ_SUBDIRS}
	$(eval OBJECTS = $(shell find $^ -name *.o))
	${LINKER} ${LD_FLAGS} -L${LIB_DIR} -o $@ $(OBJECTS) ${LIB} && cd ../

clean_bin:
	rm -f ${BIN}
clean_obj:
	rm -rf ${OBJ_SUBDIRS}

clean_all: clean_obj clean_bin

build: clean_bin ${BIN}

rebuild: clean_all build