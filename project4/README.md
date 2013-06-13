# UCLA CS174
## Computer Graphics
### Assignment #4 - May 14, 2013

# Implemented Features
## Base GLUT Window
Implemented simple GLUT window that displays a scene on screen. Using the q, Q, or ESC keys allows you
to close the window.

## Load Images into Buffers for Display
Use TA's code to load TGA images into display buffer.

## Apply Texture onto Cube Faces
The loaded texture is mapped onto the face of a cube.

## Create a Second Cube with a Zoomed out Texture
This second cube (hereinafter #4, like the spec), is located on the left. Its texture is zoomed out by 50%, or a factor
of 2. Since it's a cube, this means we'll see 2^2=4 repeats of the pattern.

## Implement Tri-Linear Filtering for the Left-Most Cube (#4), Nearest Neighbor for Right-Most Cube
See main.cpp:193-208 for details.

## Cube Position
Both cubes are visible within the default view.

## Camera Movement
Implemented the following keyboard controls:
i - Move closer to the quadrilaterals
o - Move away from the quadrilaterals

### Contact
Problems running? Contact me. pcrumm@p3net.net
UID: 804005575