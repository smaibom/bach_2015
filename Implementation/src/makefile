# makefile for our implementation of NFA searching
# Files in folder:
#  Model.cpp - main file, constructs NFA, reads data and prints results
#   Node.h - Stucture for a node in the NFA
#   Pretty.h - Used for printing a NFA, used when -s flag is given at runtime
#   State.h - A state in the NFA, holds a node and values for insertions, deletions, mutations and path that lead to the node
#   States.h - Used for matching the NFA, holds a datastructure of State objects
#   World.h - The NFA structure, holds the nodes and keeps a start and end.
TPaMa: Model.o
	g++ -std=c++0x Model.o -Ofast -o TPaMa
Model.o: Model.cpp Model.h Node.h Pretty.h State.h States.h World.h
	g++ -std=c++0x -c Model.cpp -Ofast
clean:
	rm *.o TPaMa