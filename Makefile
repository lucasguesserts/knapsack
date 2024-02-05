BUILD_DIR = build
BUILD_TYPE = Release
NUMBER_OF_THREADS := $$(expr `nproc --all` / 2)

MAIN_EXEC = ${BUILD_DIR}/main
OUTPUT_FILE = results.log

INSTANCES = $(wildcard instances/*.txt)
MODELS = greedy ip brkga

all:
	cmake . -DCMAKE_BUILD_TYPE=$(BUILD_TYPE) -B ${BUILD_DIR} &&\
	cmake --build $(BUILD_DIR) -- -j $(NUMBER_OF_THREADS) &&\
	cp /dev/null ${OUTPUT_FILE} &&\
	for I in ${INSTANCES}; do \
		for M in ${MODELS}; do \
			echo -e "===\n\ninstance: $$I\nmodel: $$M" | tee -a ${OUTPUT_FILE}; \
			LD_LIBRARY_PATH=${GUROBI_LIB_DIR}:$$LD_LIBRARY_PATH \
			${MAIN_EXEC} $$M $$I | tee -a ${OUTPUT_FILE}; \
		done \
	done

enforce_code_style:
	find src/ -iname "*.hpp" -o -iname "*.cpp" | xargs clang-format -i

clean:
	rm -rf ${BUILD_DIR} .cache/ *.log*
