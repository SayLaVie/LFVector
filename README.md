This is an implementation of the Lock-Free Vector from Dr. Damian Dechev's 2006 paper "Lock-Free Dynamically Resizable Arrays".

This is a thread-safe vector that uses Compare-And-Swap (CAS) operations to achieve lock-free progress.


Compile with:
	g++ -std=c++11 -pthread -I./src/include src/<nameOfMainFile>
or
	./compile.sh <relativePathToMainFile>


Reference:
Damian Dechev, Peter Pirkelbauer, and Bjarne Stroustrup. 2006. Lock-free dynamically resizable arrays. In Proceedings of the 10th international conference on Principles of Distributed Systems (OPODIS'06), Mariam A. Shvartsman (Ed.). Springer-Verlag, Berlin, Heidelberg, 142-156. DOI=http://dx.doi.org/10.1007/11945529_11
