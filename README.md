DESCRIPTION

The modification of A* algorithm with limited depth BFS.

The idea : In the A * algorithm, when a node is expanded, we expand it up to a fixed and constant limited
depth by the BFS strategy. After the expansion, we add the nodes not in the BFS (FIFO) order but in the
usual A * order.

This was done by first implementing the A* for 8-Puzzle which would now accept a depth as input and both the heuristics : tile mismatch count and manhattan distance being implemented for comparison.
We call the solver twice for both the heuristics : once with the input depth parameter, and then again with the parameter being 1 (which is the usual Expand).
Thus the total number of times that the puzzle will be solved by A * is 4.

We also compare the results of all the four calls for at least 10 randomly generated initial states for optimality in the program itself and compare the computational costs (number of nodes generated and the maximum length of the fringe throughout) of all the four calls for the different initial states.

	Usage : make

		Or for custom input,
			./8puzzle < input.txt > output.txt
			(Sample input file included)
