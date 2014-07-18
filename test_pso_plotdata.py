#!/usr/bin/env python
import fnmatch
#import os
#import subprocess
import matplotlib
matplotlib.use('Agg')
import numpy as np
import matplotlib.mlab as mlab
import matplotlib.pyplot as plt
import pylab
import sys
import math
import scipy
from matplotlib import rc
from matplotlib import animation

from config import settings
FIG_FORMAT = settings['figure.format']
FIG_DPI = int( settings['figure.dpi'] )

def savefig( fn ):
	outfn = fn + "." + FIG_FORMAT
	plt.savefig(outfn, format=FIG_FORMAT, dpi=FIG_DPI)
	print "Figure saved as", outfn


args = sys.argv;
if len(args) != 2:
	sys.exit("Must provide filename");

# Filename
fn = args[1];

print "Processing", fn

#FIXME: Find dimension dynamically
dim = 2
print "Particle dimension =", dim

data = pylab.loadtxt(fn, skiprows=1);

#data = data[1:-1,:] # remove the first line containing the particle dimension
numFrames = data.shape[0] # Subtract one because the first line has the particle dimension
numParticles = data.shape[1] / dim
print "Particles used", numParticles
print "Frames =", numFrames

# FIXME: We should get two or more sets of these for each axis
minValue = min(data.reshape(numFrames*numParticles*dim,1))
maxValue = max(data.reshape(numFrames*numParticles*dim,1))

# First set up the figure, the axis, and the plot element we want to animate
fig = plt.figure()
ax = plt.axes(xlim=(minValue[0], maxValue[0]), ylim=(minValue[0], maxValue[0]))
plt.grid(True)

line, = ax.plot([], [], 'ro', lw=2)


# initialization function: plot the background of each frame
def init():
    line.set_data([], [])
    return line,

# animation function.  This is called sequentially
def animate(i):
	#print "i =", i
	D = data[i,:];
	D = D.reshape(numParticles,dim);
	x = D[:,0];
	y = D[:,1];
	minX = min(x);
	maxX = max(x);
	minY = min(y);
	maxY = max(y);
	#plt.axes(xlim=(minX, maxX), ylim=(minY, maxY))
	#print "X =", x
	#print "Y =", y
	line.set_data(x, y)
	s = "Ackely Benchmark: Step %s" % i
	plt.title(s)
	return line,

# call the animator.  blit=True means only re-draw the parts that have changed.
anim = animation.FuncAnimation(fig, animate, init_func=init,
                               frames=numFrames, interval=100, blit=True)

# save the animation as an mp4.  This requires ffmpeg or mencoder to be
# installed.  The extra_args ensure that the x264 codec is used, so that
# the video can be embedded in html5.  You may need to adjust this for
# your system: for more information, see
# http://matplotlib.sourceforge.net/api/animation_api.html
#anim.save('basic_animation.mp4', fps=30, extra_args=['-vcodec', 'libx264'])
anim.save(fn + ".mp4", fps=5)

plt.show()

print "Processing of", fn, "is complete."

