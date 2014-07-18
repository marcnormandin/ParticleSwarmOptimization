## Small Particle Swarm Optimization library in C++

This is a small library that implements the basic Particle Swarm Optimation algorithm in C++. It was created and used in another project of mine, and I thought that it might be of help to others in their own projects or to learn about PSO.

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
To create the test program:
```
g++ -o test particle.cpp pso.cpp test_pso_gendata.cpp -lgsl -lgslcblas -lm
```
Then run it:
```
test 16 100 testdata
```
The above will generate data using 16 particles for 100 iterations and save into a file named 'testdata'. Displayed to standard out will be something looking like the following:
```
Evaluating: Two-dimensional Ackley.
True: (x,y) = (4,6.8)
PSO best: (x,y) = (4.00002,6.79984)
```
_The test code actually uses a shifted Ackley function, where the true best value is at (4,6.8)._ The results show that the PSO algorithm did find the best value. 

You can plot the intermediate results that were saved to the file 'testdata_ackley.part' (requires Python):
```
./test_pso_plotdata.py testdata_ackley.part
```
The result will be an animation. The first image should look like the following:
![Image of first frame](http://i.imgur.com/tEu4zRs.png)

The last image will show the particles clustering (some overlapping) around the best value found:
![Image of last frame](http://i.imgur.com/RIfBucY.png)

### Best wishes

