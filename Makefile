include compiler.mk

LOCAL_DIR = ${PWD}
MODULES:=YTL System Utils Math

LIBRARY_MODULES:=$(foreach MODULE,${MODULES},$(LOCAL_DIR)/$(MODULE)/lib$(MODULE)${LIBRARY_SUFFIX}.so)

LIBRARY_PATH=${LD_LIBRARY_PATH}
LIBRARY_PATH:=$(foreach MODULE,${MODULES},:$(LOCAL_DIR)/$(MODULE))
export LD_LIBRARY_PATH=${LIBRARY_PATH}

LIB_DIR = /usr/local/lib:/usr/lib:/usr/lib/i386-linux-gnu
LIB = ${LIBRARY_MODULES} -lSDL -lGLESv2 -lopenal -lvorbisenc -lvorbisfile -lncurses -lfreetype -lEGL -lXxf86vm -lXext -lX11
SRC_DIR = ${LOCAL_DIR}/.
INC_DIR = ${LOCAL_DIR}/.
OBJ_DIR = ${LOCAL_DIR}/bin/${Mode}
INCLUDE_DIR = -I./ -I/usr/include/freetype2

INCLUDE_DIR:=${INCLUDE_DIR} $(foreach MODULE,${MODULES}, -I./${MODULE})
export INCLUDE_DIR

#targets for full compilation chain
$(MODULES): force
	cd $@ && $(MAKE) build

build: $(MODULES)

rebuild: clean_all build

clean_obj:
	rm -rf ${OBJ_DIR}/*.*

clean_all: clean_obj

.PHONY: force
force:;