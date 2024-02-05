BUILD_DIR = build
SOURCE_DIR = src

MAIN_SOURCE = ${SOURCE_DIR}/main.cpp
MAIN_EXEC = ${BUILD_DIR}/main

all: build_dir
	g++ -o ${MAIN_EXEC} ${MAIN_SOURCE} -I ${SOURCE_DIR}

build_dir:
	@mkdir -p ${BUILD_DIR}
