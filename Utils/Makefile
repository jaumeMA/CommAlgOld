include ../compiler.mk

INCLUDE_DIR = -I./ -I../
LIB_DIR = /usr/local/lib:/usr/lib:/usr/lib/i386-linux-gnu
SRC_DIR = .
SUBDIRS = ${SRC_DIR}/rtti
OBJ_DIR = ../bin/${Mode}
OBJ_SUBDIRS= ${OBJ_DIR}/rtti

SRC_FILES = ${SRC_DIR}/cLog.cpp ${SRC_DIR}/engine.cpp
OBJ_FILES = ${OBJ_DIR}/cLog.o ${OBJ_DIR}/engine.o

BIN = libUtils${LIBRARY_SUFFIX}.so

OBJECTS=
CURR_OBJ_SUBDIR=

#targets for full compilation chain
${OBJ_DIR}/%: ${SRC_DIR}/%
	mkdir -p $@
	cd $< && make build 

${OBJ_DIR}/%.o: ${SRC_DIR}/%.cpp
	${CXX} ${INCLUDE_DIR} -fpic -c ${CC_FLAGS} ${PREDEFINED_MACRO} $< -o $@

${BIN}: ${OBJ_FILES} ${OBJ_SUBDIRS}
	$(eval OBJECTS = $(shell find $^ -name *.o))
	${LINKER} ${LD_FLAGS} -L${LIB_DIR} -o $@ $(OBJECTS) ${LIB} && cd ../

clean_bin:
	rm -f ${BIN}
clean_obj:
	rm -rf ${OBJ_FILES}
	rm -rf ${OBJ_SUBDIRS}

clean_all: clean_obj clean_bin

build: ${BIN}

rebuild: clean_all build