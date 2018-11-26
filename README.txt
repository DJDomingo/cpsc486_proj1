Program Name: Cull & Clip Project#1
By:     Daniel Domingo (djdomingo@csu.fullertin.edu), 
	Jordan Harvey (jpharvey@csu.fullerton.edu), 
	Patrick Myers (pmyers@csu.fullerton.edu),
	Kim-Lan Hoang (khoangl@csu.fullerton.edu)
	 
Team Name: ZZZ
Class #: 486

Files: 486_proj1_Source.cpp, math.h, test.txt, inputoutput.cpp

Intro: The task of this project is to input a scene and camera, and output a list of unculled whole
	triangles along with new clipped triangles obtained from partially included triangles.
External Req: none (all files used are provided)

Build: Used Visual Studio and coded with C++.  
Usage:  Currently project has:
	- Vector class to handle points and vector information
	- Calculating of Frustrum Vertices and Panels
	- Txt file is read into the project and split into appropriate sections of data which can be grabbed.
	Still needs:
	- Calculating azimuth and elevation for points when moving objects into world space
	- Putting Triangles into bags to make objects
	- Clipping and Culling of Objects using the Frustrum calculated

Extra Features: none
Bugs: With what is currently working, no major bugs.  Calculations need altering.
