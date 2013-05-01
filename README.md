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

## Extra Credit
# Moon
The third planet (the water planet) has a small white moon that retrogrde
orbits it.

# Camera Attachment
Attach the camera to the brown planet on the press of b. You can press b or space
to leave the mode, and use arrow keys to control movement. The camera looks at the
planet and orbits with it, so the view will be moving radially also.

# Contact
Problems running? Contact me. pcrumm@p3net.net
UID: 804005575