SRC_DIR = ./
INC_DIR = ./
OBJ_DIR = ../../bin/${Mode}/memory
INCLUDE_DIR = -I./ -I../ -I../..

SRC_FILES = ${SRC_DIR}/cMemoryPool.cpp ${SRC_DIR}/cBuddyAllocator.cpp ${SRC_DIR}/cMemoryMgr.cpp ${SRC_DIR}/cMemoryTracker.cpp ${SRC_DIR}/cSystemAllocator.cpp

OBJ_FILES = ${OBJ_DIR}/cMemoryPool.o ${OBJ_DIR}/cBuddyAllocator.o ${OBJ_DIR}/cMemoryMgr.o ${OBJ_DIR}/cMemoryTracker.o ${OBJ_DIR}/cSystemAllocator.o

${OBJ_DIR}/%.o: ${SRC_DIR}/%.cpp
	${CXX} ${INCLUDE_DIR} -fpic -c ${CC_FLAGS} ${PREDEFINED_MACRO} $< -o $@

clean_obj:
	rm -rf ${OBJ_FILES}

clean_lex:
	rm -f ${SRC_DIR}/scriptParser.tab.*
	rm -f ${SRC_DIR}/scriptParserHelper.cpp

clean_all: clean_lex clean_obj

build: ${OBJ_FILES}
rebuild: clean_all build