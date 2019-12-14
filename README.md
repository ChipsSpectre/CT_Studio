# CT Reconstruction Studio

This project is based on a final Project of the Lab Course: Image Reconstruction and Visualization using C++,
at Technical University of Munich.

## Team Members:
 - Alex Gaul
 - Simon Griebel
 - Maximilian Hornung

## Table of Contents

- Dependencies
- How to run
- Acquistion widget manual
- Reconstruction widget manual
- Visualization widget manual

### Dependencies

Our project depends on both Eigen3 and 
Qt5. 

We use CMake (minimum version 3.5.1) to  
manage our program's structure to simplify changes 
in it, and
 doxygen (>=1.8.0) for documentation.

### How to run

The program can be executed typing

./part2

into a terminal (at the location of the executable, which is
called part2 by default). After that, the UI starts and automatically loads a small 3D phantom.

Our project will automatically create its documentation 
during build - the documentation can be found in the 
docs/ directory inside of the build folder.
The documentation can be generated manually by 
executing 'make doc' at the build folder's location).

To view the documentation open the html/index.html file inside 
of that directory (e.g. in Firefox).

### AcquisitionWidget manual

In the acquisition widget, the user can see the position of 
X-ray emitter and detector (who form an 
acquisition pose together) from three all axes.
Using the keyboard arrow keys, the acquisition pose can be rotated 
all around the volume, the position of emitter and detector are 
updated accordingly. The user can also show the rays between 
the emitter and the pixels of the detector.

The user can add and remove acquisition poses by clicking "store current pose" and "delete current pose". These poses are then used for 
reconstruction.

In experiments, we found out 30 circles of poses with 30 poses per 
circle (maximum) are sufficient to reconstruct all information 
of a 10x10x10 volume (using 40 iterations of CG).

It would be cumbersome to enter all these poses manually - therefore
we made it possible to generate poses these poses automatically. The 
user has to provide the number of circles and the number of poses 
per circle only.

The user is also able to change the resolution and size of the 
detector, as well as its distance to the volume. This distance 
is not taken absolutely, but related to the size of the volume - 
because of that, similar values for the distance will result in 
similar behavior of the imaging setup.
By including these configuration possibilities,
many different imaging setups can be simulated without
changing the software.

All positions of acquisition poses' emitters are shown on the screen,
to convince the user of a regular sampling of the volume.

On the right side of the screen, the user can see the current acquisition
data (the image that reaches the detector in the current acquisition 
pose). It is also possible to show all acquisitions in a 
linearized way.

### ReconstructionWidget manual

The top part of this widget shows the reconstruction results. 
Each voxel is shown as a square area, therefore data is not blurred
when the window size is changed.

To perform the reconstruction using Conjugate Gradient (CG) algorithm, 
press "Load Acquisition". 
This task is computationally expensive and can take several
seconds. After the computations have finished, it is possible
to scroll through the reconstructed volume (along all three 
axes, which can be chosen using a slider).

There are several possibilities of adjusting the reconstruction
setup in the lower part of this widget:

- The number of iterations for CG can be changed - more iterations
improve the results, but also increase the computational effort. 
- The user has the possibility to add a custom amount of noise to the
acquisition (to simulate the situation in real world scenarios).
- Regularization can be enabled to reduce the impact of noise on 
reconstruction results. The user has the possibilty to enter a 
custom value for lambda, to change the 
regularization parameter of the Tikhonov regularization.
Warning: Too high values for lambda can degrade the reconstruction 
results to a level below the noisy image!

After changing the setup of reconstruction, the user has to hit 
"Update reconstruction" to apply his changes and recompute the 
reconstructed volume using the new setup. 

### VisualizationWidget manual

Our visualization widget implements Multiplanar Reconstruction (MPR)
and Direct Volume Rendering (DVR) using Maximum Intensity Projection
(MIP). 

Our implementation of MPR does not only support MPR from the
origin, but the user can specify a custom origin point in 3D.
Additionally, the user can change rotation around Y and Z axis as 
well as the zoom factor (e.g. to magnify interesting medical 
details).

The software supports both perspective and orthographic DVR.
The user is able to adjust the rotation along Y and Z axis, 
as well as a custom zoom factor, step width and distance between
camera and volume to be rendered.

### List of extensions to original task

- full screen mode

Acquisition: 
- 3 axes view of Acquisition Poses
- Configuration of Acquisition Poses
- Widget to show all acquisitions

Reconstruction:
- Parallelization using OpenMP
- Scrolling trough reconstruction result along all three axes

Visualization:
- User defined Zoom for MPR
- Configuration of MPR
- Perspective DVR
- Configuration / zoom for DVR