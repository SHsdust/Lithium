Airplane Fight Game

A 2D game written in C++ using SDL2

- **Build**

  ```bash
  # already in the root dir of project
  cd build
  
  # puild is a perl script to generate Makefile
  ./puild ../src ---p c++1z debug sdl ---o lithium-app ---c -DFANCY_MACRO=1 > Makefile
  
  make
  
  ./lithium-app
  ```

  
