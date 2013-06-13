# UCLA CS174
## Computer Graphics
### Assignment #3 - May 7, 2013

# Implemented Features
## Enable Window Closing
Using `q` key.

## Develop a function that generates sphere geometry
Actually a class, see planet.h.

## Create a small solar system
Solar system with orbiting planets around sun at (1, 1, 0). Each planet
rotates independently, and is colored and shaded as described.

## Camera navigation
We use the same navigation system as the last project. Notable changes:
(space) - reset to default view
e - Make camera look up
d - Make camera look down

The camera defaults to a thirty degree downward view on the entire solar system.
The reset key returns this view.

## Phong and Gourand Shading
For both of these techniques, we use the normal at each vertex instead of
computing a single normal for the entire trianglular primitive as we do
for flat shading. We calculate the normal on the CPU for all shading types.

Gourand shading utilizes the same per-vertex and per-fragment lighting
calculations computed for flat shading. Here, however, we do all calulations
in the vertex shader and simply pass the final value into the fragment shader.

For Gourand shading, we have this overview:
CPU - Normals
Vertex basis (on the graphics card, vshader) - Normalized vectors, diffuse, ambient,
and specular products, combining to form our final color for all parts of the vertex.
Fragment basis (on the graphics card, fshader) - Simply assign our vertex-computed
value to the appropriate rasterized fragments.

Phong shading is calculated similarly, but is instead implemented on a
per-fragment basis. We simply normalize the normals and other necessary
lighting vectors in the vertex shader, and pass them into the fragment
shader for per-fragment calculation.

For Phong shading, we have this overview:
CPU - Normals
Vertex basis (on the graphics card, vshader) - Normalized vectors, actual light position.
Fragment shader (fshader) - Actual per-fragment calculations of diffuse, ambient, and specular
products.

For simplicity, we use the same shaders for all types of shading. GLSL's conditionals
execute regardless of their truthiness for my old version of GLSL, so our duplicated code
(including code guarded in if statements) is ran for all cases, unfortunately.

# Extra Credit
## Moon
The third planet (the water planet) has a small white moon that retrogrde
orbits it.

## Camera Attachment
Attach the camera to the brown planet on the press of b. You can press b or space
to leave the mode, and use arrow keys to control movement. The camera looks at the
planet and orbits with it, so the view will be moving radially also.

# Contact
Problems running? Contact me. pcrumm@p3net.net
UID: 804005575