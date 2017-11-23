NB_PROC=4
SEED=42
NB_BY_PROC=10
C_STD=-std=c++11

run: hyperquick
	@mpirun -np $(NB_PROC) ./$< $(SEED) $(NB_BY_PROC)

test: test_file
	@./$<

test_file: sequential_functions.o test_file.o
	@g++ $(C_STD) $^ -o $@

hyperquick: hyperquick.o sequential_functions.o
	@mpic++ $(C_STD) $^ -o $@

hyperquick.o: hyperquick.cpp hyperquick.h
	@mpic++ -c $(C_STD) $<

sequential_functions.o: sequential_functions.cpp sequential_functions.h
	@g++ -c $(C_STD) $<

test_file.o: test_file.cpp catch.hpp
	@g++ -c $(C_STD) $<

clean:
	-@rm -f test_file *.o hyperquick