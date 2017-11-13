NB_PROC=8
SEED=42
NB_BY_PROC=10

run: hyperquick
	mpirun -np $(NB_PROC) ./$< $(SEED) $(NB_BY_PROC)

hyperquick: hyperquick.cpp hyperquick.h
	mpic++ $< -o $@

clean:
	-rm hyperquick