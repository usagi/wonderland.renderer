wonderland.renderer
===================

C++ header only library of OpenGL rendering engine designed for wonderland game engine.

## Caution

This is alpha version under developing on master branch in currently.
Please wait for stable release version if you avoid unfortunately troubles.

## Dependancy

- [GLEW](http://glew.sourceforge.net/)
- [Open Asset Import Library](http://assimp.sourceforge.net/)

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

## Emscripten tips

Open Asset Import Library(assimp) can compile Emscripten-1.16.0.

For example build assimp with Emscripten:

```shell
. pwd
/home/you/repos
. git clone git@github.com:assimp/assimp.git
. cd assimp
. mkdir build.em
. cd build.em
. cmake -D Ninja .. -DCMAKE_CXX_COMPILER=em++ -DCMAKE_C_COMPILER=emcc -DCMAKE_BUILD_TYPE=Release
. ninja
```

Then, you can build your-app with link libassimp.so:

```shell
em++ -std=c++11 -O2 your-app.cxx /home/you/repos/assimp/build.em/code/libassimp.so -I/home/you/repos/assimp/include -o your-app.html
```

## License

[MIT](LICENSE)

## Author

Usagi Ito <usagi@WonderRabbitProject.net>
