  <div align="center">
    <h1>
      <img src="https://kostrykin.github.io/LibCarna/html/logo.png" style="height: 5rem"><br>
      <a href="https://github.com/kostrykin/LibCarna/actions/workflows/build.yml"><img src="https://github.com/kostrykin/LibCarna/actions/workflows/build.yml/badge.svg" /></a>
      <a href="https://anaconda.org/bioconda/libcarna"><img src="https://img.shields.io/conda/v/bioconda/libcarna.svg?label=Bioconda%20Version" /></a>
      <a href="https://anaconda.org/bioconda/libcarna"><img src="https://img.shields.io/conda/dn/bioconda/libcarna.svg?label=Bioconda%20Downloads" /></a>
    </h1>
  </div>

LibCarna is a fork of [Carna](https://github.com/RWTHmediTEC/Carna) that provides classes for simple real-time 3D visualization of biomedical data and beyond. It is named after the greek god of organs (yup, they really did have even one for organs). It is based on OpenGL 3.3 and Eigen 3.

Go to: [Library Documentation](https://kostrykin.github.io/LibCarna/html)

---
## Contents

* [Installation](#1-installation)
* [Build instructions](#2-build-instructions)
* [Including in your project](#3-including-in-your-project)
* [See also](#4-see-also)

---
## 1. Installation

For 64bit Linux, there is a prebuilt [Bioconda package](https://anaconda.org/bioconda/libcarna) that is can be installed in any Conda environment:

```bash
conda install bioconda::libcarna
```

---
## 2. Build instructions

On Linux you can run either of the two build scripts:
- `linux_build-default.bash`: Runs the default build, which includes the demo and the test suite. Also runs the tests. LibCarna is built without EGL support for offscreen rendering.
- `linux_build-egl.bash`: Builds LibCarna with EGL support, but skips the demo and the test suite. This is because the demo and the test suite are built with Qt, which cannot be initialized when LibCarna is built with EGL support.

### Dependencies:

* **OpenGL ≥ 3.3** (tested with up to 4.6)
* **Eigen** ≥ 3.0.5

In addition, building the demo or the test suite also requires the following libraries:

* **libboost-iostreams** (tested with 1.83.0, older versions might work too)
* **Qt ≥ 4** (tested with up to Qt 5.15.13)

Compilation process has been tested with following tool chains:

* **GCC 7.5** (tested 2021)
* **GCC 13.3 + CMake 3.28.3** (tested 2025)

---
## 3. Including in your project

See *test_cmake/CMakeLists.txt* as a starting point.

---
## 4. See also

* [LibCarna-Python](https://github.com/kostrykin/LibCarna-Python) provides Python bindings.
