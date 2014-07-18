## Small Particle Swarm Optimization library in C++

This is a small library that implements the basic Particle Swarm Optimation algorithm in C++.

The main classes are the following:
- (class PSO) _The main PSO algorithm_
- (class Particle) _Represents a particle manipulated by the algorithm_
- (class RandomNumberGenerator) _To generate random numbers using the GNU Scientific Library_

Also included is test code that applies the PSO library to the Ackley and 2D Gaussian functions. The results are plotted/animated using the included Python script.


### Requirements
- C++ compiler and linker.
- [GNU Scientific Library](http://www.gnu.org/software/gsl/).
- Python (To plot results of the supplied test program).

### Example test Program
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
./test_pso_plotdata.py testdata_ackley.part
```
The result will be an animation. The first image should look like the following:
![Image of frame 0](http://i.imgur.com/tEu4zRs.png)

