BUILD_DIR = build
BUILD_TYPE = Release
NUMBER_OF_THREADS := $$(expr `nproc --all` / 2)

SOLVER_EXEC = ${BUILD_DIR}/knapsack-solver
OUTPUT_FILE = results.log

INSTANCES = $(wildcard instances/*.txt)
CONFIGS = "--model=greedy" \
	"--model=ip --maximum_running_time=30.0" \
	"--model=brkga --maximum_running_time=1.0 --population_size=204 --elite_percentage=0.1207 --mutants_percentage=0.0198 --num_elite_parents=2 --total_parents=5"
RANDOM_SEED = 1234

.PHONY: build

all: build run

run: build
	cp /dev/null ${OUTPUT_FILE} &&\
	for I in ${INSTANCES}; do \
		for C in ${CONFIGS}; do \
			echo -e "===\n\ninstance: $$I\nconfig: $$C" | tee -a ${OUTPUT_FILE}; \
			LD_LIBRARY_PATH=${GUROBI_LIB_DIR}:$$LD_LIBRARY_PATH \
			${SOLVER_EXEC} --debug --seed=${RANDOM_SEED} $$C $$I | tee -a ${OUTPUT_FILE}; \
		done \
	done

build:
	cmake . -DCMAKE_BUILD_TYPE=$(BUILD_TYPE) -B ${BUILD_DIR}
	cmake --build $(BUILD_DIR) -- -j $(NUMBER_OF_THREADS)

enforce_code_style:
	find src/ -iname "*.hpp" -o -iname "*.cpp" | xargs clang-format -i

clean:
	rm -rf ${BUILD_DIR} .cache/ *.log*
