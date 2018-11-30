export FLEX=/usr/bin/flex
export BISON=/usr/bin/bison
export CXX=/usr/bin/g++-6
export CC=/usr/bin/gcc-6
export LINKER=/usr/bin/g++-6

ifeq ($(Mode),Debug)
PREDEFINED_MACRO=-D_DEBUG
ifeq ($(MemoryTrack),True)
PREDEFINED_MACRO:=${PREDEFINED_MACRO} -DMEMORY_TRACKER
endif
export LIBRARY_SUFFIX=_debug
CC_FLAGS= -g -std=c++1z -O0 -fno-stack-protector -fconcepts
else ifeq ($(Mode),Release)
export LIBRARY_SUFFIX=_release
CC_FLAGS= -std=c++1z -O3 -fconcepts
else
$(error Target not supported)
endif

export PREDEFINED_MACRO

#check for custom preprocessor
ifdef preproc
CC_FLAGS:=$(CC_FLAGS) -no-integrated-cpp -B$(preproc)
endif

export CC_FLAGS
export LD_FLAGS=-shared -Wl,--no-as-needed