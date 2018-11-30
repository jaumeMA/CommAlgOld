include ../compiler.mk

LIB_DIR = /usr/local/lib:/usr/lib:/usr/lib/i386-linux-gnu
SRC_DIR = ./
INC_DIR=./
OBJ_DIR = ../bin/${Mode}
INCLUDE_DIR = -I./ -I../ -I../YTL -I../Utils -I../IO
BIN_DIR=./

SRC_FILES = ${SRC_DIR}/cXmlTree.cpp ${SRC_DIR}/cProperty.cpp ${SRC_DIR}/cXMLdoc.cpp ${SRC_DIR}/cXMLreader.cpp ${SRC_DIR}/cXMLwriter.cpp ${SRC_DIR}/cDefaultHandler.cpp ${SRC_DIR}/cXmlData.cpp

OBJ_FILES = ${OBJ_DIR}/cXmlTree.o ${OBJ_DIR}/cProperty.o ${OBJ_DIR}/cXMLdoc.o ${OBJ_DIR}/cXMLreader.o ${OBJ_DIR}/cXMLwriter.o ${OBJ_DIR}/cDefaultHandler.o ${OBJ_DIR}/lexParser.o ${OBJ_DIR}/xmlParserHelper.o ${OBJ_DIR}/cXmlData.o

BIN = libXmlParser${LIBRARY_SUFFIX}.so

BISONS:
	${BISON} -v -d ${SRC_DIR}/lexParser.ypp

LEXS: BISONS
	${FLEX} -o xmlParserHelper.cpp ${SRC_DIR}/lexParser.l
	${CXX} ${INCLUDE_DIR} -g -fpic -c ${CC_FLAGS} ${SRC_DIR}/xmlParserHelper.cpp -o ${OBJ_DIR}/xmlParserHelper.o
	${CXX} ${INCLUDE_DIR} -g -fpic -c ${CC_FLAGS} ${SRC_DIR}/lexParser.tab.cpp -o ${OBJ_DIR}/lexParser.o

#targets for full compilation chain
${OBJ_DIR}/%.o: ${SRC_DIR}/%.cpp
	${CXX} ${INCLUDE_DIR} -fpic -c ${CC_FLAGS} $< -o $@

${BIN}: ${OBJ_FILES}
	${LINKER} ${LD_FLAGS} -L${LIB_DIR} -o $@ $^ ${LIB} && cd ../

clean_bin:
	rm -f ${BIN_DIR}/${BIN}

clean_obj:
	rm -rf ${OBJ_FILES}

clean_all: clean_obj clean_bin

build: LEXS ${BIN}

rebuild: clean_all build