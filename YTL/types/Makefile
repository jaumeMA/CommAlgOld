LIB_DIR = /usr/local/lib:/usr/lib:/usr/lib/i386-linux-gnu
SRC_DIR = .
OBJ_DIR = ../../bin/${Mode}/types
OBJ_SUBDIRS= ${OBJ_DIR}/embedded_type ${OBJ_DIR}/optional ${OBJ_DIR}/smartPtr ${OBJ_DIR}/taggedPtr ${OBJ_DIR}/variant 

BIN = libYTL${LIBRARY_SUFFIX}.so

OBJECTS=
CURR_OBJ_SUBDIR=

#targets for full compilation chain
${OBJ_DIR}/%: ${SRC_DIR}/%
	mkdir -p $@
	cd $< && make build 

${BIN}: ${OBJ_SUBDIRS}

clean_bin:
	rm -f ${BIN}
clean_obj:
	rm -rf ${OBJ_SUBDIRS}

clean_all: clean_obj clean_bin

build: ${OBJ_SUBDIRS}

rebuild: clean_all build