wonderland.renderer
===================

C++ header only library of OpenGL rendering engine designed for wonderland game engine.

## Caution

This is alpha version under developing on master branch in currently.
Please wait for stable release version if you avoid unfortunately troubles.

## Dependancy

- [GLEW](http://glew.sourceforge.net/)

### note

this library is not depend on wonderland.subsystem
 or any other window management library or OS APIs
 but require OpenGL context at the run-time.
 Therefore, this library could not run standalone,
 its need to running with library or OS APIs
 as a window management and make OpenGL context
 any as you like.

library suggestion:

- wonderland.subsystem
- GLFW3 or GLFW2
- SDL2
- EGLplus or EGL
- OS native API directly

## License

[MIT](LICENSE)

## Author

Usagi Ito <usagi@WonderRabbitProject.net>
