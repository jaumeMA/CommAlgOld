LIB_DIR = /usr/local/lib:/usr/lib:/usr/lib/i386-linux-gnu
SRC_DIR = ./
INC_DIR=./
OBJ_DIR = ../../bin/${Mode}/container
INCLUDE_DIR = -I./ -I../ -I../../ -I../../System -I../../Utils -I../../Math
BIN_DIR=./

SRC_FILES = ${SRC_DIR}/cArray.cpp ${SRC_DIR}/cString.cpp ${SRC_DIR}/cTupla.cpp ${SRC_DIR}/cUnorderedMap.cpp ${SRC_DIR}/cIterableImpl.cpp

OBJ_FILES = ${OBJ_DIR}/cArray.o ${OBJ_DIR}/cString.o ${OBJ_DIR}/cTupla.o ${OBJ_DIR}/cUnorderedMap.o ${OBJ_DIR}/cIterableImpl.o

#targets for full compilation chain
${OBJ_DIR}/%.o: ${SRC_DIR}/%.cpp
	${CXX} ${INCLUDE_DIR} -fpic -c ${CC_FLAGS} ${PREDEFINED_MACRO} $< -o $@

clean_bin:
	rm -f ${BIN_DIR}/${BIN}

clean_obj:
	rm -rf ${OBJ_FILES}

clean_all: clean_obj

build: ${OBJ_FILES}
rebuild: clean_all build