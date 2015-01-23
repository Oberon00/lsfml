Title:  LSFML Readme  
Author: Christian Neumüller  

Lua SFML
========

**Lua Simple and Fast Multimedia Library** (or **LSFML** for short) is a binding
of [SFML][] to [Lua][].



Building
--------

### Ingredients ###

The following libraries are needed:

- **[SFML][]** in the latest (git) version, available at
  <http://github.com/LaurentGomila/SFML>.
- **[Boost][]** in version 1.53 (later versions may/should also work).
- **[Lua][]** in the latest version of the 5.2.x branch. You may want to apply
  the patches listed at <http://www.lua.org/bugs.html>. Please compile as C
  (jd was not tested with the C++ build of Lua which throws exceptinons).
- **[Apollo][]**: <https://github.com/Oberon00/apollo>

The build system used is **[CMake][]** in a recent 3.0.x version.



### Setup ###

Especially on Windows, make sure to set any environment variables neccessary to
let CMake find the librarys are set correctly: `SFML_ROOT`, `BOOST_ROOT`,
`LUA_DIR`, `APOLLO_DIR`.

As an alternative to setting the variables you can also install the libraries
to the (CMake) standard locations or add the containing directories to
`CMAKE_PREFIX_PATH`.


#### `Find*.cmake` ####
The `CMakeLists.txt` for apollo calls `find_package()` with some libraries where
the corresponding `Find*.cmake` does not ship with CMake:

* `FindLua52.cmake` and `FindApollo.cmake` are contained in the apollo library.
* `FindSFML.cmake` comes with SFML

To make CMake find them you have two options:

* Copy the files to your CMake installation's module directory. This is the
  directory where e.g. the `AddFileDependencies.cmake` module is located. It
  usually lies in `<prefix>/share/cmake-2.8/Modules`, where `<prefix>` is the
  installation directory on Windows (usually
  `C:\Program Files (x86)\CMake 2.8`) and usually simply `/usr` on Linux.
* <a name=setcmakevar></a>Specify the directories where the files are located
  as a semicolon `;` separated list in the `CMAKE_MODULE_PATH` CMake cache
  variable: *Add entry*, type string in the Windows GUI;
  or `-DCMAKE_MODULE_PATH=<path-list>` on the command line.

### Actual build ###

What's left to do is a standard CMake build. The following contains nothing
special, so if you are familiar with CMake, you can just skip the rest.

#### Unix-like ####

Navigate to the LSFML root directory in your shell, then execute the following
commands.

    mkdir build # Name basically arbitrary, but build is already in .gitignore
    cd build    #
    cmake .. # You may need to add the module path modifications here.
    make
    sudo make install # Optional.
    
#### Visual Studio ####

Use the Visual Studio command prompt as your shell and do as in Unix, with the
following modifications: You may need to add `-G "Visual Studio 11"` (or 12 for
VS 2013) to the cmake command line. Then use `msbuild ALL_BUILD.vcxproj` instead
of `make` and `msbuild INSTALL.vcxproj` instead of `sudo make install` if you
want to install the Jade Engine. As in Unix, you will need administrative rights
for installing, but because Windows has no `sudo` equivalent, you may need to
e.g.  launch a new VS command prompt as administrator.


> LSFML -- Copyright © Christian Neumüller 2015
> This file is subject to the terms of the BSD 2-Clause License.
> See LICENSE.txt or http://opensource.org/licenses/BSD-2-Clause.

[Lua]: http://www.lua.org/
[SFML]: http:/sfml-dev.org/
[Boost]: http://www.boost.org/
[Luabind]: http://www.rasterbar.com/products/luabind.html
[CMake]: http://www.cmake.org/
