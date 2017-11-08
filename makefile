NB_PROC=8

run: hyperquick
	mpirun -np $(NB_PROC) ./$< 42 10

hyperquick: hyperquick.cpp hyperquick.h
	mpic++ $< -o $@