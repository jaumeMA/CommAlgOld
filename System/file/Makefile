LOCAL_DIR = ${PWD}
MODULES = Gui
LIB_DIR = -L/usr/local/lib -L/usr/lib:/usr/lib/i386-linux-gnu -L${LOCAL_DIR}/${MODULES}
SRC_DIR = ./
INC_DIR = ./
OBJ_DIR = ../../bin/${Mode}/file
INCLUDE_DIR = -I./ -I../ -I../..

SRC_FILES = ${SRC_DIR}/cFileIStream.cpp ${SRC_DIR}/cFileStream.cpp ${SRC_DIR}/cFile.cpp ${SRC_DIR}/cMappedFile.cpp

OBJ_FILES = ${OBJ_DIR}/cFileIStream.o ${OBJ_DIR}/cFileStream.o ${OBJ_DIR}/cFile.o ${OBJ_DIR}/cMappedFile.o

${OBJ_DIR}/%.o: ${SRC_DIR}/%.cpp
	${CXX} ${INCLUDE_DIR} -fpic -c ${CC_FLAGS} ${PREDEFINED_MACRO} $< -o $@

clean_obj:
	rm -rf ${OBJ_FILES}

clean_all: clean_obj

build: ${OBJ_FILES}
rebuild: clean_all build