LIB_DIR = /usr/local/lib:/usr/lib:/usr/lib/i386-linux-gnu
SRC_DIR = ./
INC_DIR=./
OBJ_DIR = ../../bin/${Mode}/signal
INCLUDE_DIR = -I./ -I../ -I../../ -I../../IO -I../../Utils -I../../Mat
BIN_DIR=./

SRC_FILES = ${SRC_DIR}/cSignalFunctor.cpp ${SRC_DIR}/cConnection.cpp ${SRC_DIR}/cAsyncConnection.cpp

OBJ_FILES = ${OBJ_DIR}/cSignalFunctor.o ${OBJ_DIR}/cConnection.o ${OBJ_DIR}/cAsyncConnection.o

#targets for full compilation chain
${OBJ_DIR}/%.o: ${SRC_DIR}/%.cpp
	${CXX} ${INCLUDE_DIR} -fpic -c ${CC_FLAGS} ${PREDEFINED_MACRO} $< -o $@

clean_obj:
	rm -rf ${OBJ_FILES}

clean_all: clean_obj

build: ${OBJ_FILES}
rebuild: clean_all build