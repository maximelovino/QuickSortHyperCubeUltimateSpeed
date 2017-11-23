# Program variables
CPU_COUNT=4
SEED=42
VALUES_COUNT_PER_CPU=10

# Compilation variables
C_STD=-std=c++11
CPP_COMPILER=g++
MPI_COMPILER=mpic++
RUN_WITH_MPI=mpirun

help:
	@echo "Usage:"
	@echo "make run to run (and compile) the program"
	@echo "make test to run the tests"
	@echo ""
	@echo "To change processor count, number of values by processor or seed, change the variables of the makefile"
	@echo "If you don't use g++ or mpic++ to compile on your machine, you can change it in the variables of the makefile as well"

run: hyperquick
	@$(RUN_WITH_MPI) -np $(CPU_COUNT) ./$< $(SEED) $(VALUES_COUNT_PER_CPU)

test: test_file
	@./$<

test_file: sequential_functions.o test_file.o
	@$(CPP_COMPILER) $(C_STD) $^ -o $@

hyperquick: hyperquick.o sequential_functions.o
	@$(MPI_COMPILER) $(C_STD) $^ -o $@

hyperquick.o: hyperquick.cpp hyperquick.h
	@$(MPI_COMPILER) -c $(C_STD) $<

sequential_functions.o: sequential_functions.cpp sequential_functions.h
	@$(CPP_COMPILER) -c $(C_STD) $<

test_file.o: test_file.cpp catch.hpp
	@$(CPP_COMPILER) -c $(C_STD) $<

clean:
	-@rm -f test_file *.o hyperquick