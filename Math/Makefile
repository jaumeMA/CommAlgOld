include ../compiler.mk

LIB_DIR = /usr/local/lib:/usr/lib:/usr/lib/i386-linux-gnu
LIB = -lgmp
SRC_DIR = ./
INC_DIR=./
OBJ_DIR = ../bin/${Mode}
INCLUDE_DIR = -I./ -I../ -I../Utils -I../YTL -I../IO
BIN_DIR=./

SRC_FILES = ${SRC_DIR}/cNumberSpaces.cpp ${SRC_DIR}/cNumberTraits.cpp ${SRC_DIR}/cPolynomial.cpp

OBJ_FILES = ${OBJ_DIR}/cNumberSpaces.o ${OBJ_DIR}/cNumberTraits.o ${OBJ_DIR}/cPolynomial.o

BIN = libMath${LIBRARY_SUFFIX}.so

#targets for full compilation chain
${OBJ_DIR}/%.o: ${SRC_DIR}/%.cpp
	${CXX} ${INCLUDE_DIR} -fpic -c ${CC_FLAGS} ${PREDEFINED_MACRO} $< -o $@

${BIN}: ${OBJ_FILES}
	${LINKER} ${LD_FLAGS} -L${LIB_DIR} -o $@ $^ ${LIB} && cd ../

clean_bin:
	rm -f ${BIN_DIR}/${BIN}

clean_obj:
	rm -rf ${OBJ_FILES}

clean_all: clean_obj clean_bin

build: ${BIN}

rebuild: clean_all build