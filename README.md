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

* **OpenGL ≥ 3.3** (tested with up to 4.6)
* **GLEW ≥ 1.7**
* **Eigen** ≥ 3.0.5

In addition, building the demo or the test suite also requires the following libraries:

* **libboost-iostreams** (tested with 1.83.0, older versions might work too)
* **Qt ≥ 4** (tested with up to Qt 5.15.13)

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

See *test_cmake/CMakeLists.txt* as a starting point.

---
## 4. See also

* The additional [CarnaQt module](https://github.com/RWTHmediTEC/CarnaQt) provides auxiliary classes for using Carna ≥3.0.0 with Qt ≥4.8.
* The [CarnaDICOM module](https://github.com/RWTHmediTEC/CarnaDICOM) provides classes for loading DICOM datasets with Carna ≥3.0.1.
* [CarnaPy](https://github.com/kostrykin/CarnaPy) provides Python bindings.
