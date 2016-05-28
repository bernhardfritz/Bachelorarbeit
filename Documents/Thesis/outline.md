# abstract
...

# 1. introduction
...

# 2. related work
Kamal and Uddin evaluated various terrain generation algorithms like Midpoint-Displacement-/Diamond-Square-Algorithm and Fault-Algorithm and came up with their own algorithm called Repeated Magnification and Probing.

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
This implementation starts by placing 4 lines along the landscape borders as well as l randomly distributed cutting lines all over the landscape. In the next step all line intersections will be determined. These intersections are represented as vertices in an undirected graph. A line can have several intersections. Therefore they will be iterated through from the start of the line to the end. Intersections connected by a cutting line will also be connected by an edge in the graph respectively. By using a Depth-First-Search (DFS) based algorithm all minimal cycles of the graph will be determined. These cycles represent the polygon shapes produced by the previously placed cutting lines. Once the polygons are obtained another undirected graph will be produced. This time the vertices of the graph are represented by polygons. In this graph 2 vertices are only connected by an edge if both polygons share a same cutting line. In this newly generated graph another DFS will be performed. As starting point of the DFS the polygon which should contain the desired mountain peak is chosen. The combined region of the first r elements of the DFS will be raised in height. A point-in-polygon test, originated by Shirmat and later translated to C by W. Randolph Franklin, was used to determine if a heightmap point should be affected by height alterations or not. The whole procedure will be repeated n times. After several tests, it was clear that the graph-based implementation was far too slow to be used for realtime applications. Therefore an alternative implementation was considered.

### Voronoi-based implementation
Instead of using expensive graph algorithms, the idea is to use a Voronoi diagram that does the same job by providing randomly distributed polygons. Still, the problem remains. How should one cheaply generate these diagrams? Actually, there is an intuitive solution to this problem. Imagine an arbitrary amount of randomly colored, flat shaded, overlapping cone geometries that are randomly distributed on a plane. If one looks at this scene from the top using orthographic projection, a Voronoi diagram can be identified. After some research it was evident that this technique was first mentioned in chapter 14 of the "OpenGL Red Book". This implementation relies on OpenGLs depth testing and is much faster than the graph-based approach. To combine this technique with RMP, one has to project the Voronoi diagram onto the heightmap, check the color of the polygon at the desired peak location and raise all heightmap tiles that have the same color projected onto them. By repeating the whole process n times, mountains with predefined peaks can be generated. The spread of the mountain can be altered by increasing or decreasing the amount of cones to be placed during Voronoi generation. The more cones, the more polygons will be created and the resulting mountain will be very steep. Using less cones, will result in less polygons and the mountain will therefore be more spread. The height of the mountain is proportional to the amount of iterations performed.

## Thermal erosion algorithm
Thermal erosion describes the process of material breaking loose due to variations in temperature during day/night cycles. This algorithm was implemented based on Jacob Olsen's description in his paper "Realtime Procedural Terrain Generation". Basically, he used a cellular automaton to model the process of thermal erosion. Cellular automata operate on grids of cells. By inspecting a cell itself and its neighborhood, one can decide how they should interact with each other. In this case, cells correspond to heightmap tiles. If the height difference of the inspected cell and one of its neighbors is larger than a so-called talus angle T, material will be transported from the inspected cell to the corresponding neighbor cell. This process will be repeated for all cells/tiles of the heightmap, resulting in a general smoothing of the landscape. Especially steep parts of the heightmap are most affected by this erosion process. By default Jacob Olsen's algorithm disregards material properties like hardness or softness (e.g. rock is hard, sand is soft). Therefore, as an extension of this algorithm, a way to also consider material properties will be introduced. By multiplying the amount of material to be transported by a factor that depends on the height of the inspected cell, it is possible to mimic the desired material properties.

## Hydraulic erosion algorithm
### Implementation based on previous research
In my reasearch I came across two different papers that described how to model hydraulic erosion algorithmically. The first one called "Realtime Procedural Terrain Generation" was written by Jacob Olsen and featured a decent algorithm based on a cellular automaton. The idea behind this algorithm is that water (e.g. rain) causes erosion of terrain by dissolving material which gets transported by running water and once the water evaporates the material will be deposited again. To model this process it is necessary to keep track of the height-, water- as well as sediment-levels. Therefore not only a heightmap, but also a water- and sedimentmap is necessary. Each iteration can be broken down into 4 steps. In the first step a constant amount of water is added to each cell of the watermap. In the second step water dissolves a specific amount of material by removing material from the heightmap and adding material to the sedimentmap. A similar technique as used with thermal erosion will be introduced to also consider material hardness. This time around not the height but the steepness of the mountain will be used as an indicator to determine how much material should be dissolved. In the third step water containing sediment will be transported from higher cells to lower cells accordingly. In the fourth step water will be evaporated and sediment will be deposited at its current position by lowering the cells of the water- as well as sedimentmap and raising the cells of the heightmap. The whole process will be repeated n times.

The second paper called "Fast Hydraulic Erosion Simulation and Visualization on GPU" by Mei et al showcased a more advanced model of hydraulic erosion. The basic principle was still the same as described by Olsen, but Mei used a more realistic water flow model which also considers fluid velocity. Due to the force of flowing water more sediment will be carried away which results in a generally more realistic model of hydraulic erosion. After studying Mei's algorithm and implementing his water flow model, a flaw in his algorithm became evident. Given that Mei's algorithm is timestep-based, it becomes unstable if too large timesteps are used. Water begins to oscillate and ultimately the water height will result in a variable type overflow and render the simulation unusable. To keep the simulation stable very small timesteps have to be used. In fact these timesteps are that small that one cannot even notice any difference in water flow for a couple of minutes. For a realtime application this is totally unacceptable and therefore this model was disregarded.

### Evaluation of various shallow water models
After implementing Olsen's hydraulic erosion simulation and trying out various parameters it was evident that the results were not too realistic. Due to the lack of performance, Mei's variant was not satisfying either. To achieve more realism a more sophisticated, yet performant model was needed. While researching about hydraulic erosion I also stumbled upon shallow water models. Apparently these models are commonly used to predict tsunami movement but can also be used for different fields of application like hydraulic erosion simulation. Due to the complexity at hand I was not able to implement my own shallow water model based on previous papers and kept on looking for existing solutions. I found two promising projects. The first one was written by Alex Darcy which used a Riemann solver to numerically solve the shallow water equations. The only problem was that he assumed that the terrain underneath the water was perfectly flat. This might be a valid assumption for tsunami simulations but did not work for my area of application.

The second project based on Trevor Dixon's implementation of the shallow water equations seemed very promising. His code was much more readable in comparison to Alex Darcy's. The only thing missing was water ground interaction. After spending a considerable amount of time I ultimately was not able to figure out how to include this feature and went back to searching for alternatives.

This is when Mr. Harders pointed out there is another alternative namely smoothed particle hydrodynamics.

### Implementation based on Smoothed Particle Hydrodynamics (SPH) simulation
There are dozens of different existing SPH solutions out there. Most of them were proprietary and those that were not brought other issues with them like hardware incompatibility or realtime incapability. Last but not least I came across a GitHub project by Saeed Mahani. He wrote a basic SPH simulation featuring a box which gets filled up by fluid particles while obeying the laws of physics. The project was realtime capable and had no special hardware requirements. However, several alterations to the SPH simulation had to take place in order to make it usable for this project. First of all the whole visualization code had to be replaced to be compatible with the rest of my GLFW based project. After that the simulation had to be extended to also support heightmaps which it previously did not. The original code only supported planes which could not be used to model arbitrarily shaped surfaces. Due to this an alternative solution had to be implemented. My first idea was to simply add support for static (non-moving) particles and distribute them all across the heightmap. After implementing it, it became evident that this approach had several flaws. Not only were particles passing through the heightmap because of small holes inbetween the static particles, the simulation also became horribly slow due to the sheer amount of particles present in the scene. As a result a different solution had to be found. In order to make the particles appear to be influenced by the heightmap surface, some kind of force has to counteract their tending downwards movement due to graviy. To implement this kind of behaviour it is necessary to continuously check if a particle is touching the surface. A fast triangle-ray intersection test by Möller and Trumbore was used to check if a particle is below the heightmap surface or not. If the intersection test indeed is positive, a short impulse in the direction perpendicular to the impact point on the heightmap surface has to be applied to the particle. Since the normals are only calculated at gridpoints on the heightmap it was necessary to interpolate them to aquire the correct impulse direction at the impact point. Since the heightmap is rasterized into tiles it can be guaranteed that there are always 4 normals in close proximity to the impact point. These normals are normalized vectors. To get the interpolated normal from these 4 vectors one has to sum them up and normalize the result. The performance of this solution was much better than the previous one and no more particles were dropping below the surface.
Once the SPH simulation was working as intended, it was possible to utilize it to simulate a more realistic hydraulic erosion model. The idea is that each particle gets some capacity to store sediment. If a particle touches the terrain it dissolves a specific amount of it based on the predefined particle acidity. Additionally each particle has a predefined lifespan and once its time runs out it evaporates and leaves behind the sediment it carried. Heightmap changes take place if a particle dissolves sediment or if a particle evaporates. A particle potentially travels a huge distance which implies that carried sediment will almost always end up somewhere distant from where it was dissolved in the first place. To simulate rain the particles will be spawned in a certain height at random positions on the xz-plane. The user can observe the simulation in realtime and experience the effects of hydraulic erosion first-hand.

## Tectonic plate simulation
Due to the complexity of real tectonic plate movement, the design of the simulation had to be simplified to a degree which allowed it to be implemented programmatically. As a result the problem at hand was reduced to two dimensions. The general idea is to split the heightmap into several pieces, representing tectonic plates. Additionally they should be kept in motion to simulate magma flowing underneath the surface. To model this non-trivial behaviour I decided to use a physics engine. Box2D, a 2D physics engine, supports convex polygons with up to 8 vertices as well as collision detection, is very well documented and offers a test environment called "testbed". Splitting of arbitrarily shaped polygons is not supported by Box2D though. Nevertheless this engine was far superior to its alternatives in terms of documentation, community support and features. After going through the documentation and some code samples I was able to implement the missing features myself.

### Implementation using Box2D physics engine
When working with Box2D it is very common to start off by writing a test application for "testbed". To make this test application available in "testbed" it is necessary to extend the class "Test" provided by Box2D and list the newly created test class in "TestEntries.cpp" as specified in the Box2D documentation. The advantage of "testbed" is that it provides its own visualization interface as well as several debugging options. The downside is that "testbed" tests can not be used for production systems right away and have to be altered during the integration process.
Once the base test class is set up, several methods like "Step", "Create", "Keyboard", etc. provided by the super class "Test" can be overwritten. "Step" will be called automatically by the test environment at each timestep. "Create" will be called only once and should be used to initialize variables or data structures. Method "Keyboard" will be triggered each time a key is pressed. Its parameters give information about which key was pressed. To add an object to the Box2D world several steps are necessary. Firstly, a body object has to be defined. Secondly, a fixture object has to be defined and linked with the body object it corresponds to. In Box2D bodies can either be static, dynamic or kinematic. Static bodies do not move. Even if a dynamic body collides with a static body, only the dynamic body will be affected by the collision. Dynamic bodies can move and are affected by all other bodies. Kinematic bodies can also move, but are not affected by collisions caused by dynamic bodies. The tectonic plate simulation will consist of dynamic as well as kinematic bodies. The dynamic bodies will represent the tectonic plates and the kinematic bodies will be responsible for keeping the simulation in motion as well as keeping the dynamic bodies contained within the simulation area. Body objects can be used to access information like current position, angle, velocity, mass, etc. To describe the shape of a Box2D body, fixtures will be used. To define a fixture it is necessary to set the fixture's density attribute and shape. Based on the fixture's density and area it consumes, the body's mass will be determined. Box2D offers basic shapes like rectangles and circles as well as convex polygons with a maximum of 8 vertices. The tectonic plate simulation will be initialized with 5 rectangle fixtures. 4 of these will be used to define the kinematic bodies' shapes, i.e. thin beams. The last one will be used to define the dynamic body's shape, a big square. This square will be randomly split a specific amount of times during set up. Unfortunately Box2D did not offer this splitting functionality per se and it therefore had to be implemented by hand. To implement splitting of polygons it is important to know that Box2D only supports convex polygons with up to 8 vertices which need to be specified in counter clockwise winding. In order to split polygons, Box2D's raycasting feature can be very helpful. It allows the programmer to cast rays and if a ray intersects with an object a callback function will be called. When a ray is cast through a convex polygon there should always be exactly one entry point and one exit point. Box2D's raycasting only registers the first intersection point per object though. Due to this two rays had to be cast in opposite directions. Having both, the first ray's entry point and the second ray's entry point as well as all vertices the polygon consists of, the only thing left to do is to determine which vertex belongs to which slice of the polygon. The following pseudocode should adequately elaborate this procedure:
```
entryPoint = entry point of first ray
exitPoint = entry point of second ray
rayCenter = ((entryPoint.x + exitPoint.x) / 2, (entryPoint.y + exitPoint.y) / 2)
rayAngle = atan((entryPoint.y - exitPoint.y) / (entryPoint.x - exitPoint.x))
for each vertex v of polygon:
  cutAngle = atan((v.y - rayCenter.y) / (v.x - rayCenter.x)) - rayAngle
  if cutAngle < -PI:
    cutAngle += 2 * PI
  if cutAngle > 0 && cutAngle <= PI:
    // vertex v belongs to polygon1
  else:
    // vertex v belongs to polygon2
```
Once the polygons (i.e. tectonic plates) are all set up, collision callbacks provided by Box2D are used to act whenever a collision occurs. For the tectonic plate simulation only the collisions between the dynamic objects, i.e. the plates, are important. Therefore the program is set up to only act on collisions in which two dynamic objects are involved. Additionally, collisions between two polygons which do not have two collision points are disregarded. By enforcing these constraints the program will only act on collisions that occur between two dynamic objects and consist of exactly two collision points. Finally, in order to use these collision points for heightmap alterations, they have to be transformed from the Box2D coordinate system to the heightmap coordinate system.

### Generation of mountain ranges due to tectonic plate collisions
Each tectonic plate collision results in two collision points due to the custom collision callbacks mentioned before. Using these points one can draw an imaginary line onto the heightmap. The goal would be to procedurally generate a mountain range along this imaginary line. This is where an advanced version of RMP algorithm comes into place. This version of RMP will also use a Voronoi diagram to generate arbitrary polygons. Different from the previous RMP implementation this time there will be two coordinate pairs instead of a single one. Between these coordinates Bresenham line algorithm will be performed and all colors passed will be remembered. In the next step the Voronoi diagram will be projected onto the heightmap and all heightmap vertices inside polygons that are colored with one of the remembered colors will be raised in height. This procedure will be repeated each time a tectonic plate collision occurs. As a result procedurally generated mountain ranges will arise between two colliding tectonic plates.

## Visualization techniques
### Texture splatting
Texture splatting can be used to make a computer generated terrain more visually appealing. By using textures and blending them together based on height and slope it is possible to texture procedurally generated terrain regardless of shape and complexity of terrain features. For this application four textures have been chosen:
* Sand
* Grass
* Stone
* Snow

Blending textures together is performed by the fragment shader. The OpenGL shader language offers a function "mix" that allows to linearely interpolate between two texels. Mix consists of three parameters:
* First texel color
* Second texel color
* A floating-point number between 0.0 and 1.0 representing the mixture ratio

```
color0 = sand texture
color1 = grass texture
color2 = rock texture
color3 = snow texture

max_height = max height of terrain
min_height = min height of terrain

difference = max_height - min_height
delta = difference / 4
threshold0 = delta * 1
threshold1 = delta * 2
threshold2 = delta * 3

position = pass-through position from vertex shader

if position.y < threshold0:
  texel = color0
if position.y >= threshold0 && position.y < threshold0 + delta):
  texel = mix(color0, color1, (position.y - threshold0) / delta)
if position.y >= threshold0 + delta && position.y < threshold1):
  texel = color1
// the remaining textures are blended similarily...
```
Up to now only the height information is used to determine which texture should be applied on the terrain. Of course this model does not look too realistic since the slope information has not been considered so far. Grass does not grow on steep slopes. Same goes for snow. Snow will only be at rest on flat areas. Therefore in steep slopes grass and snow texels have to be overridden by rock texels. A common approach to determine the steepness of a slope is to use the normal vectors' y coordinates. Using thresholds it is again possible to utilize OpenGL's mix function to mix two texels. Unfortunately there are some special cases, namely steep slopes inside an area between two height thresholds which required bilinear interpolation instead of linear interpolation. These cases were solved by calling mix in a nested way, meaning the first or second parameter of mix is itself the result of a previous mix invocation.

### Bloom shader effect
A bloom shader effect is a popular choice to enhance the realism of a scene. The goal is to reproduce image imperfections caused by real-world cameras. Brightly lit spots often appear to be glowing when captured with a camera. To model this effect two filters are necessary:
* Bright-pass filter
* Blur filter
The bright-pass filter will be applied to the original frame. After that the result will be blurred using a blur filter, e.g. a two-pass Gaussian filter. Finally the original frame as well as the blurred bright-pass frame will be blended together resulting in an image with brightly glowing regions.

#### Bright-pass filter
This filter makes bright regions even brighter while not modifying dark regions. It is possible to specify the range of colors to be brightened as well as the amount they should be brightened by. The bright-pass shader code is based on Erik Reinhard's formula
L_d(x,y) = (L(x,y) * (1 + L(x,y) / L_white^2)) / (1 + L(x,y))
which can be found in his paper "Photographic Tone Reproduction for Digital Images".

#### Two-pass Gaussian filter
A two-pass Gaussian filter consists of two passes, namely a horizontal and a vertical Gaussian blur pass. During these passes each pixel will be averaged using a one-dimensional Gaussian kernel. The two-pass Gaussian filter is computationally less expensive than the one-pass Gaussian filter using a two-dimensional Gaussian kernel. For this filter the vertex shader is responsible for setting up the pixel coordinates to be averaged by the fragment shader based on Gauss distributed weights.
Vertex shader pseudocode:
```
UV = input pixel location
direction = (1,0) if this is a horizontal pass or (0,1) if this a vertical pass
offsets[] = a one-dimensional Gaussian kernel

for i between 0 and offsets.length:
  blurUV[i] = UV + (direction.x * offsets[i], direction.y * offsets[i])
```
Fragment shader pseudocode:
```
texture = input frame
blurUV[] = array of input pixel locations to be averaged
weights[] = an array of Gauss distributed values based on the previously used Gaussian kernel

color = (0,0,0) // output color

for i between 0 and weights.length:
  color += texture.pixelAt(blurUV[i]) * weights[i]
```

### Capturing screenshots with stblib
OpenGL provides a function called glReadPixels which copies a specified region of pixels from the video card memory to the RAM. RGB images require width * height * 3 bytes per pixel. Since a char requires exactly one byte of space it is commonly used to represent color channel information. In fact a library called stblib written by Sean T. Barret et al is perfectly capable of saving these kinds of character arrays in various file formats like .jpg or .png and was used for this purpose.

### Video recording with ffmpeg
To efficiently record a video it is necessary to allocate enough space beforehand. Reallocating space during the capturing process leads to noticeable stuttering and is not an option. The following formula was used to calculate the amount of required space:
width * height * 3 * fps * seconds bytes
Using OpenGL's glReadPixels function it was possible to save each frame into an array of type char. Once all the frames are captured they need to be encoded using ffmpeg's MPEG2 encoding algorithm. Therefore each frame has to be converted from RGB to YCbCr color space using ffmpeg library. For some reason ffmpeg's YCbCr color conversion algorithm flips the frame vertically during conversion. After flipping the frame back it is ready to be encoded by ffmpeg. Once all frames are encoded, ffmpeg will save the video as .mpg file on the harddisk. The file format .mpg can be played with almost any video player and does not require too much space due to MPEG2 encoding algorithm provided by ffmpeg.

# 4. results
...
