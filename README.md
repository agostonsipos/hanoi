Towers of Hanoi simulation.

Was an examination task for undergraduate students in 2015.

In folder `Modeling` is the OpenSceneGraph project that is generating the models used in the simulation, which is in folder `Simulation`. The two projects are distinct with no cooperation (the generated models can be copied to the other directory)

Project `Modeling` is standard C++ (CXX11) and depends on the following:
 - OpenSceneGraph (>=3.2)

Project `Simulation` is standard C++ (CXX11) and depends on the following:
 - OpenGL (>=3.1)
 - GLEW
 - GLM (>=0.9.7)
 - SDL2 (>=2.0) with SDL2_image

On Windows you can find all dependencies correctly on my university's site: [OSGPack](http://cg.elte.hu/~bsc_cg/resources/OSGPack.zip) and [OGLPack](http://cg.elte.hu/~bsc_cg/resources/OGLPack_VS2015.zip)
