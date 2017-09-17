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
        * lighting has to be enabled for the shader to be acivated
    * texturing [*quad with a texture*](hydra/texturing)
        * Trying to use **pxr::GlfTextureRegistry::GetInstance().GetTextureHandle(pxr::TfToken("default.png"));**
        * looks like the plugin required for texture loading isn't loaded
        * What is the difference between **GetTextureResourceID** & **GetTextureResource**
        * How are textures bound to shaders?
        
        
## Questions & Todo

Why doesn't `template<T> HdRenderIndex::InsertTask` return the newly created Task as `boost::shared_ptr<T>`?

What are working definitions of the imaging libraries: Hd, Hdx & HdSt?


Hydra Engine include introduce a ptyhon dependency?

~~~
#include "pxr/imaging/hd/engine.h"
~~~~
