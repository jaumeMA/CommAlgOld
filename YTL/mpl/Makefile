LIB_DIR = /usr/local/lib:/usr/lib:/usr/lib/i386-linux-gnu
SRC_DIR = ./
INC_DIR=./
OBJ_DIR = ../../bin/${Mode}/mpl
INCLUDE_DIR = -I./ -I../ -I../../ -I../../Utils
BIN_DIR=./

SRC_FILES = ${SRC_DIR}/cTemplateHelper.cpp

OBJ_FILES = ${OBJ_DIR}/cTemplateHelper.o

#targets for full compilation chain
${OBJ_DIR}/%.o: ${SRC_DIR}/%.cpp
	${CXX} ${INCLUDE_DIR} -fpic -c ${CC_FLAGS} $< -o $@

clean_obj:
	rm -rf ${OBJ_FILES}

clean_all: clean_obj

build: ${OBJ_FILES}
rebuild: clean_all build