# Small Particle Swarm Optimization library in C++

This is a small library that implements the basic Particle Swarm Optimation algorithm in C++.

The main classes are the following:
- (class PSO) The main PSO algorithm
- (class Particle) Represents a particle manipulated by the algorithm
- (class RandomNumberGenerator) To generate random numbers using the GNU Scientific Library

Also included is test code that applies the PSO library to the Ackley and 2D Gaussian functions. The results are plotted/animated using the included Python script.

This library requires GNU GSL.


### Test Program
To compile the test program:
```
g++ -o test particle.cpp pso.cpp test_pso_gendata.cpp -lgsl -lgslcblas -lm
```
Then run it:
```
test 16 100 testdata
```
The above will generate data using 16 particles for 100 iterations and save into a file named 'testdata'.

Then plot the results (requires Python):
```
./test_pso_plotdata.py testdata
```

