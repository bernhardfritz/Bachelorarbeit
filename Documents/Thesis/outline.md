# abstract
...

# 1. introduction
...

# 2. related work
Kamal and Uddin evaluated various parametrically controlled terrain generation algorithms like Midpoint-Displacement-/Diamond-Square-Algorithm and Fault-Algorithm and came up with their own algorithm called Repeated Magnification and Probing.

Thermal and hydraulic erosion algorithms were first described by Musgrave.

Based on Musgraves work, Olsen implemented his own version of thermal and hydraulic erosion algorithm and also improved them performance-wise to make them available for realtime-applications.

Vitanen describes the theoretical foundation of plate tectonics, compares several existing applications and ultimately implements his own tectonic plate simulation.

# 3. methods
## Heightmap
A heightmap is a surface and its height (e.g. its y coordinate) is well-defined for any coordinate tuple (x,z). Due to this, it is easily possible to export heightmaps as grayscale image files (e.g. dark portions = low elevation, light portions = high elevation). Heightmaps consist of a predefined number of tiles. These tiles again consist of 2 triangles each. The resolution of a heightmap is the number of tiles it consists of. Usually heightmaps use square dimensions, meaning the number of columns and rows of tiles is the same.

## Diamond-Square algorithm
This algorithm was first introduced by Fournier et al. It can be considered as an extension of Midpoint-Displacement algorithm. By recursively subdividing a line into segments of equal length and displacing the resulting midpoints by a random amount, Midpoint-Displacement algorithm is able to produce a fairly arbitrary looking 1D-landscape. To put this to the next level Diamond-Square algorithm proceeds in a similar fashion. In this case, square tiles are recursively subdivided and their midpoints are displaced by a random amount.

## Fault algorithm
This algorithm was first described by Krten. He proposed to randomly draw a line across the heightmap and split the landscape into two regions. By raising one and lowering the other region and repeating the whole process several times, interesting mountain-like features can emerge. Due to the sheer randomness of this algorithm it is impossible to control the position, shape or spread of the resulting mountains.

## Repeated Magnification and Probing (RMP) algorithm
RMP was created by Kamal and Uddin and can be categorized as parametrically controllable mountain generation algorithm. Height, spread and location of the mountain can be controlled by parameters. This algorithm is similar to Fault algorithm, meaning that only selected regions are repeatedly altered in height. The process how these regions are obtained is not described in Kamal and Uddin's paper. In the following two techniques will be presented that are capable of generating randomly distributed polygon-like shapes which can be used for RMP algorithm.

### Graph-based implementation
This implementation starts by placing 4 lines along the landscape borders as well as l randomly distributed cutting lines all over the landscape. In the next step all line intersections will be determined. These intersections are represented as vertices in an undirected graph. A line can have several intersections. Therefore they will be iterated through from the start of the line to the end. Intersections connected by a cutting line will also be connected by an edge in the graph respectively. By using a Depth-First-Search (DFS) based algorithm all minimal cycles of the graph will be determined. These cycles represent the polygon shapes produced by the previously placed cutting lines. Once the polygons are obtained another undirected graph will be produced. This time the vertices of the graph are represented by polygons. In this graph 2 vertices are only connected by an edge if both polygons share a same cutting line. In this newly generated graph another DFS will be performed. As starting point of the DFS the polygon which should contain the desired mountain peak is chosen. The combined region of the first r elements of the DFS will be raised in height. A point-in-polygon test, originated by Shirmat and later translated to C by W. Randolph Franklin, was used to determine if a heightmap point should be affected by height alterations or not. The whole procedure will be repeated n times.

### Voronoi-based implementation

## Generation of Voronoi diagrams

## Thermal erosion algorithm

## Hydraulic erosion algorithm
### Implementation based on previous research
### Evaluation of various shallow water models
### Implementation based on Smoothed Particle Hydrodynamics (SPH) simulation
Several alterations to the SPH simulation had to take place in order to make it usable for this project.

## Tectonic plate simulation
### Implementation using Box2D physics engine
### Generation of mountain ranges due to tectonic plate collisions

## Visualization techniques
### Texture splatting
### Skybox
### Water
#### Wave simulation
#### Transparency
### Reflection
### Bloom-Filter
#### Highpass-Filter
#### Gauss-Filter
### Marching cubes
### Taking screenshots using stblib
### Video recording using ffmpeg

# 4. results
...
