Carna
========

Carna provides classes for simple and fast visualization of CT data.
It is named after the greek god of organs (yup, they really did have even one for organs).

Go to: [Library Documentation](https://rwthmeditec.github.io/Carna/)

<img src="https://rwthmeditec.github.io/Carna/XRay01.png">

## Contents

* [Dependencies](#1-dependencies)
* [Build instructions](#2-build-instructions)
	* [Creating Visual Studio project files](#21-creating-visual-studio-project-files)
	* [Building directly](#22-building-directly)
	* [The MediTEC-way](#23-the-meditec-way)
* [Including in your project](#3-including-in-your-project)
	* [The CMake-way](#31-the-cmake-way)
	* [Manually](#32-manually)
* [See also](#4-see-also)
    
## 1. Dependencies

Carna is supposed to work on Linux and Windows,
although it has only been tested on Windows lately.

* Qt ≥ 4.8
* GLEW ≥ 1.7
* [TRTK](https://github.com/Haenisch/TRTK) ≥ 0.13.1
* [Eigen](http://eigen.tuxfamily.org/) ≥ 3.0.5
* OpenGL 2.1

**Currently there are serious issues with non-NVIDIA graphics hardware.**
This is going to be addressed in the near future.

## 2. Build instructions

The default build process requires CMake ≥ 3.0.2.

This section explains three ways of building Carna:

1. Creating Visual Studio project files and building it from the IDE
2. Building Carna directly through CMake from command line
3. If you are a colleague from MediTEC, you can use the batch script.

Regardless which build method you pick,
first thing you need to do is to fetch the latest stable version.
If you are using git from command line,
you can simply run following command
from within the directory where you want to download the sources:

	git clone https://github.com/RWTHmediTEC/Carna.git
    
After a few seconds there should be a new folder named `Carna`
at your current working directory.

### 2.1. Creating Visual Studio project files

First create the directory where the Visual Studio files should go to:

	cd Carna
    
	mkdir build
	mkdir build\VisualStudio2010

	cd build\VisualStudio2010
    
Then initialize the Visual Studio environment:

	call "%VS100COMNTOOLS%\vsvars32.bat"
    
And finnaly invoke CMake like this:

	cmake -G"Visual Studio 10" -DCMAKE_INSTALL_PREFIX="C:\Libs" ..\..
    
You may also leave out the `-DCMAKE_INSTALL_PREFIX` parameter
if you do not want to specify any particular installation directory.
In this case the default installation directory will be set,
which is the value of the environmental variable `%ProgramFiles%` on Windows.

At this point the Visual Studio project files are ready.
You can proceed by opening the solution file `Carna.sln`
that was created in `Carna\build\VisualStudio2010`.
Note that building the `INSTALL` project from the solution
actually triggers the installation routine
to the destination you configured via `-DCMAKE_INSTALL_PREFIX`.

### 2.2. Building directly

The first step is to create the directories
where the results of the building process will be stored.
You can use any names you like for the directories,
it's only important to distinguish between "debug" and "release" files:

	cd Carna
    
	mkdir build
	mkdir build\debug
	mkdir build\release

Then initialize the building enviroment.
Use the command below if you are going to use Visual Studio for compilation:

    call "%VS100COMNTOOLS%\vsvars32.bat"
    
Now it's time to run the build process.
Lets build the "debug" version first:

	cd build\debug
	cmake -G"NMake Makefiles" -DCMAKE_INSTALL_PREFIX="C:\Libs" -DBUILD_DOC=OFF ..\..
	nmake
    
You may also leave out the `-DCMAKE_INSTALL_PREFIX` parameter
if you do not want to specify any particular installation directory.
In this case the default installation directory will be set,
which is the value of the environmental variable `%ProgramFiles%` on Windows.

The parameter `-DBUILD_DOC=OFF` forces CMake to skip running Doxygen, if it is installed at all. The idea is to build the documentation only once. Since we are going to build the "release" version next, leaving out this parameter at this point would lead to Doxygen being triggered twice.

Run `nmake install` now if you wish to install the "debug" version on your system.

If everything went well,
run the build for the "release" version:

	cd ..\nmake_release
	cmake -G"NMake Makefiles" -DCMAKE_BUILD_TYPE=Release ..\..
	nmake

Run `nmake install` (again) if you want to install the "release" version.

### 2.3. The MediTEC-way

Make sure you have the environmental variable `%MEDITEC_LIBS%` set
and it is poiting to an existing directory that you have permissions to write to,
than

* run `create_msvc10_project.bat` to create the Visual Studio project files, or
* run `win32_msvc10_build.bat` to build and install the whole package.

## 3. Including in your project

It is assumed at this point that you either have built and installed Carna with CMake,
or you have fetched the binaries and the corresponding headers from somewhere.

### 3.1. The CMake-way

*yet to be written*

### 3.2. Manually

*yet to be written*

## 4. See also

The additional [Carna-DICOM module](https://github.com/RWTHmediTEC/Carna-DICOM)
provides classes for loading DICOM data sets.
