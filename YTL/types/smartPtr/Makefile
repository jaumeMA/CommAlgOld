LIB_DIR = /usr/local/lib:/usr/lib:/usr/lib/i386-linux-gnu
SRC_DIR = .
INC_DIR=.
OBJ_DIR = ../../../bin/${Mode}/types/smartPtr
INCLUDE_DIR = -I./ -I../ -I../../ -I../../../ -I../../../System -I../../../Utils -I../../../Math
BIN_DIR=./

SRC_FILES = ${SRC_DIR}/enable_ref_from_this.cpp ${SRC_DIR}/reference_counter.cpp

OBJ_FILES = ${OBJ_DIR}/enable_ref_from_this.o ${OBJ_DIR}/reference_counter.o

#targets for full compilation chain
${OBJ_DIR}/%.o: ${SRC_DIR}/%.cpp
	${CXX} ${INCLUDE_DIR} -fpic -c ${CC_FLAGS} ${PREDEFINED_MACRO} $< -o $@

clean_bin:
	rm -f ${BIN_DIR}/${BIN}

clean_obj:
	rm -rf ${OBJ_FILES}

clean_all: clean_obj clean_bin

build: ${OBJ_FILES}
rebuild: clean_all build