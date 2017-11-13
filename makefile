NB_PROC=4
SEED=42
NB_BY_PROC=10

run: hyperquick
	mpirun -np $(NB_PROC) ./$< $(SEED) $(NB_BY_PROC)

hyperquick: hyperquick.cpp hyperquick.h
	mpic++ -std=c++11 $< -o $@

clean:
	-rm hyperquick