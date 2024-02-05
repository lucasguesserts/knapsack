BUILD_DIR = build
SOURCE_DIR = src
DEBUG = 0

MAIN_SOURCE = ${SOURCE_DIR}/main.cpp
MAIN_EXEC = ${BUILD_DIR}/main
OUTPUT_FILE = ${BUILD_DIR}/output.log

GUROBI_DIR = /opt/gurobi/gurobi1100
GUROBI_INCLUDE_DIR = ${GUROBI_DIR}/include
GUROBI_LIB_DIR = ${GUROBI_DIR}/lib
GUROBI_LIB = gurobi110
GUROBI_CXX_LIB = gurobi_c++

INCLUDE = -I ${SOURCE_DIR} -I ${GUROBI_INCLUDE_DIR}
LIBS = -lm -L ${GUROBI_LIB_DIR} -l${GUROBI_LIB} -l${GUROBI_CXX_LIB}
FLAGS = -std=c++14
DEF = -D DEBUG=${DEBUG}


INSTANCES = $(wildcard instances/*.txt)
MODELS = greedy ip

all: build_dir
	g++ -o ${MAIN_EXEC} ${MAIN_SOURCE} ${FLAGS} ${INCLUDE} ${LIBS} ${DEF}
	cp /dev/null ${OUTPUT_FILE}
	for I in ${INSTANCES}; do \
		for M in ${MODELS}; do \
			echo -e "===\n\ninstance: $$I\nmodel: $$M" | tee -a ${OUTPUT_FILE}; \
			LD_LIBRARY_PATH=${GUROBI_LIB_DIR}:$$LD_LIBRARY_PATH \
			${MAIN_EXEC} $$M $$I | tee -a ${OUTPUT_FILE}; \
		done \
	done

build_dir:
	@mkdir -p ${BUILD_DIR}
