CC = g++
SOURCE_FILES = calc_models_CalcModelJNI.cc SmartCalcModel.cc
LIB_NAME = libsmartcalc.so

ifeq ($(JAVA_HOME),)
ifeq ($(OS),Windows_NT)
which = $(shell where $1)
else
which = $(shell which $1)
endif
JAVAC ?= $(realpath $(call which,javac))
JAVA_HOME = $(abspath $(dir $(JAVAC))..)
endif

ifneq ($(JAVA_HOME),)
JNI_INCLUDEDIR ?= $(JAVA_HOME)/include
endif

ifeq ($(JNI_INCLUDEDIR),)
$(error could not determine JNI include dir, try specifying either \
    JAVA_HOME or JNI_INCLUDEDIR)
endif

TARGETTRIPLET := $(shell $(CC) -dumpmachine)
ifeq ($(JNI_PLATFORM),)
ifeq ($(findstring mingw,$(TARGETTRIPLET)),mingw)
JNI_PLATFORM:= win32
else
ifeq ($(findstring linux,$(TARGETTRIPLET)),linux)
JNI_PLATFORM:= linux
endif
endif
endif

LIB_PATH = -I$(JNI_INCLUDEDIR) -I$(JNI_INCLUDEDIR)/$(JNI_PLATFORM)

all: clean build install

build:
	@$(CC) $(LIB_PATH) -fPIC $(SOURCE_FILES) -shared -o $(LIB_NAME)

install:
	@cp  $(LIB_NAME) /usr/lib

clean:
	@rm -f $(LIB_NAME)