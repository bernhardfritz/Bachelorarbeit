abstract
...

1. introduction
...

2. related work
Kamal and Uddin evaluated various parametrically controlled terrain generation algorithms like Midpoint-Displacement-/Diamond-Square-Algorithm and Fault-Algorithm and came up with their own algorithm called Repeated Magnification and Probing.

Thermal and hydraulic erosion algorithms were first described by Musgrave.

Based on Musgraves work, Olsen implemented his own version of thermal and hydraulic erosion algorithm and also improved them performance-wise to make them available for realtime-applications.

Vitanen describes the theoretical foundation of plate tectonics, compares several existing applications and ultimately implements his own tectonic plate simulation.

3. methods
Heightmap
A heightmap is a surface and its height (e.g. its y coordinate) is well-defined for any coordinate tuple (x,z). Due to this, it is easily possible to export heightmaps as grayscale image files (e.g. dark portions = low elevation, light portions = high elevation). Heightmaps consist of a predefined number of tiles. These tiles again consist of 2 triangles each. The resolution of a heightmap is the number of tiles it consists of. Usually heightmaps use square dimensions, meaning the number of columns and rows of tiles is the same.

Diamond-Square algorithm

Fault algorithm

Repeated Magnification and Probing (RMP) algorithm
Graph-based implementation
Voronoi-based implementation

Generation of Voronoi diagrams

Thermal erosion algorithm

Hydraulic erosion algorithm
Implementation based on previous research
Evaluation of shallow water model
Implementation based on Smoothed Particle Hydrodynamics (SPH) simulation

Alterations to SPH simulation implementation

Tectonic plate simulation
Implementation using Box2D physics engine
Generation of mountain ranges due to tectonic plate collisions

Visualization techniques
Texture splatting
Skybox
Water
Wave simulation
Transparency
Reflection
Bloom-Filter
Highpass-Filter
Gauss-Filter
Marching cubes
Taking screenshots using stblib
Video recording using ffmpeg

4. results
...
