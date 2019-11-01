![CPP](https://img.shields.io/badge/C++-11-blue.svg)
[![License](https://img.shields.io/badge/license-MIT-blue.svg)](https://opensource.org/licenses/MIT)

# OpenGL
OpenGL 3D renderer for learning and testing. Tutorials used : https://learnopengl.com/

## Getting Started

### Prerequisites

Windows only.

Visual studio 2017/2019.

### How to use it

Open OpenGL.sln
Be sure to select x86 as Target Platform (x64 will not work)
Launch

You may need to restart the program sometimes beacaue some tests will not work properly. This is due to a lack of abstraction of renderer state betweem tests. Some tests use differents states than others and it doesn't go well when you navigate between them.


## Tests

Test1:  Simple triangle rendering <br/>
Test2:  Mix two textures <br/>
Test3:  Transformations (rotation, scale, translation) <br/>
Test4:  Camera (FOV, aspect ratio, translation) <br/>
Test5:  Map camera translation and rotation to keyboard (w,a,s,d and arrows) + zoom to mouse -> use in all further tests. <br/>
Test6:  Point light with Phong lighting model <br/>
Test7:  Material + change light color over time <br/>
Test8:  Light map <br/>
Test9:  Directional, point and spot lights <br/>
Test10: 3D model loading + lighting <br/>
Test11: Depth buffer visualization with Sponza <br/>
Test12: Outline using stencil buffer (need to fix imgui on this one) <br/>
Test13: Blending <br/>
Test14: Backface culling <br/>
Test15: Postprocessing <br/>
Test16: Cubemap (skybox + environment mapping) <br/>
Test17: Normal vector visualization + exploding object using geometry shader <br/>
Test18: Asteroid field instancing <br/>
Test19: Multisample anti-aliasing (MSAA) <br/>
Test20: Shadow mapping <br/>
Test21: Broken for the moment... <br/>
