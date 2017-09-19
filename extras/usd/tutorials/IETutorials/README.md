# USD-tests


A bunch of examples to learn USD & Hydra C++ APIs


## Build Instructions


* Update top level CMakeLists.txt to point to USD installation
* ```mkdir USD-tests.build```
* ```cd USD-tests.build```
* ```cmake USD-tests```
* ```make```


## Examples

* hydra [*hydra renderer examples*](hydra)
    * helloWorld [*Simple single triangle rendered using Hydra*](hydra/helloWorld)
    * simpleObject *simple .obj - lambert lit solid color object*
        * [tinyObj](https://github.com/syoyo/tinyobjloader)
        * [teapot](http://g3d.cs.williams.edu/g3d/data10/common/model/teapot/teapot.zip) 
    * shader [*triangle with a solid color shader assigned*](hydra/shader)
        * lighting has to be enabled for the shader to be activated
        * animated shader constant 
    * texturing [*quad with a texture*](hydra/texturing)
        * creating a primvar results in GLSL functions for reading and they're automatically synced
        * demonstrate creation & rendering of UVs
    * lighting [simple light moving around a static object](hydra/lighting)
        * create a procedural cube
        * point, spot, area, directional?
    * shadowing [spot light shadow](hydra/shadowing)
        * point, spot, area, directional?
    * subd
        *
    * displacement
        *
    * instancing
        *
    * deformation
        *
    * points
        *
    * curves
        *
    * picking
        *
    * shading
        *    
    * render targets
        * 
    * embree
        *

     
### Hello World
### Simple Object     
### Shader
### Lighting
### Shadowing
### SubD
### Displacement
### Instancing
### Deformation
### Points
### Curves
### Picking
### Embree
        
## Questions & Todo

Why doesn't `template<T> HdRenderIndex::InsertTask` return the newly created Task as `boost::shared_ptr<T>`?

What are working definitions of the imaging libraries: Hd, Hdx & HdSt?


Hydra Engine include introduce a ptyhon dependency?

~~~
#include "pxr/imaging/hd/engine.h"
~~~~
