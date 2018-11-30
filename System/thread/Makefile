LIB_DIR = -L/usr/local/lib -L/usr/lib:/usr/lib/i386-linux-gnu -L${LOCAL_DIR}/${MODULES}
SRC_DIR = ./
INC_DIR = ./
OBJ_DIR = ../../bin/${Mode}/thread
INCLUDE_DIR = -I./ -I../ -I../../ -I../Utils -I../YTL -I../Math -I./smartPtr
BIN_DIR = ${LOCAL_DIR}

SRC_FILES = ${SRC_DIR}/cThread.cpp

OBJ_FILES = ${OBJ_DIR}/cThread.o

${OBJ_DIR}/%.o: ${SRC_DIR}/%.cpp
	${CXX} ${INCLUDE_DIR} -fpic -c ${CC_FLAGS} $< -o $@

clean_obj:
	rm -rf ${OBJ_FILES}

clean_all: clean_obj

build: ${OBJ_FILES}
rebuild: clean_all build