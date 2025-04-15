Carna
=====

Carna provides classes for simple and fast visualization of CT data.
It is named after the greek god of organs (yup, they really did have even one for organs).
It is based on OpenGL 3.3 and Eigen 3.

Go to: [Library Documentation](https://kostrykin.github.io/Carna/html)

---
## Contents

* [Dependencies](#1-dependencies)
* [Build instructions](#2-build-instructions)
* [Including in your project](#3-including-in-your-project)
* [See also](#4-see-also)
 
---
## 1. Dependencies

* [Eigen](http://eigen.tuxfamily.org/) ≥ 3.0.5
* OpenGL ≥ 3.3, tested with up to 4.6
* GLEW ≥ 1.7

In addition, building the demo or the test suite also requires the following libraries:

* libboost-iostreams (tested with 1.83.0, older versions might work too)
* Qt ≥ 4 (tested with up to Qt 5.15.13)

Compilation process has been tested with following tool chains:

* **GCC 7.5** (tested 2021)
* **GCC 13.3 + CMake 3.28.3** (tested 2025)

---
## 2. Build instructions

On Linux you can run either of the two build scripts:
- `linux_build-default.sh`: Runs the default build, which includes the demo and the test suite. Also runs the tests. Carna is built without EGL support for offscreen rendering.
- `linux_build-egl.sh`: Builds Carna with EGL support, but skips the demo and the test suite. This is becaue the demo and the test suite are built with Qt, which cannot be initialized when Carna is built with EGL support.

---
## 3. Including in your project

Add a `find_package` directive to your project's `CMakeLists.txt` file, e.g.:

```CMake
find_package( Carna REQUIRED )
include_directories( ${CARNA_INCLUDE_DIR} )
```

If you need to put a constraint on the version, use `find_package(Carna 3.1.0 REQUIRED)`
to pick a package with a version *compatible* to 3.1.0,
or use `find_package(Carna 3.1.0 EXACT REQUIRED)` to pick a package by the exact version.

You also need to add the headers (usually *only* the headers) from Eigen:

```CMake
find_package( Eigen3 REQUIRED )
include_directories( ${EIGEN3_INCLUDE_DIR} )
```

Finally add Carna to the linking stage:

```CMake
target_link_libraries( ${TARGET_NAME} ${SOME_OTHER_LIBRARIES} ${CARNA_LIBRARIES} )
```

---
## 4. See also

* The additional [CarnaQt module](https://github.com/RWTHmediTEC/CarnaQt) provides auxiliary classes for using Carna ≥3.0.0 with Qt ≥4.8.
* The [CarnaDICOM module](https://github.com/RWTHmediTEC/CarnaDICOM) provides classes for loading DICOM datasets with Carna ≥3.0.1.
* [CarnaPy](https://github.com/kostrykin/CarnaPy) provides Python bindings.
