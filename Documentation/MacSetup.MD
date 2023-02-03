# Mac FI3D Development Setup

<b>Note:</b> Deployable version for Mac is currently non-functional. Deployable feature is only functional on Windows.

## Qt v6.4.2

* [Download Qt unified setup](https://www.qt.io/download)
* run it
  * hit next
  * create an account (or login if already have one)
  * Select where to install Qt (remember this directory)
  * Selecting components
    * Select Archive and LTS from the left pane
    * Click Refresh
    * Under Qt -> Qt 6.4.2
      * Select gcc 64 bit.
      * Select the following as well
        * Sources
        * Qt Quick 3D
        * Qt 5 Compatibility Module
        * Qt Shader Tools
        * Qt Debug Information Files
        * Qt Quick Timeline
        * Additional Libraries
          * This shoud include them all
    * Under `Developer and Designer Tools`
      * Select `OpenSSL 1.1.1j Toolkit`
      * <b>Note:</b> OpenSSL features have not been tested on Mac.
    * Click next
  * Accept agreement and click next
  * click next
  * click install

## VTK_v9.2.5

Download VTK source code from [here](https://www.vtk.org/download/). Then, follow these steps:

* Unzip the contents of the download
* Open CMake
* Click `Browse Source` and select the unzipped directory
* Click `Browse Build`
  * Go to the same unzipped directory
  * Create a new folder called `build`
  * Select the `build` folder
* Click `Configure`
* Select Unix Makefiles
* Set `VTK_GROUP_ENABLE_Qt` to `WANT`
* Set `VTK_GROUP_ENABLE_Imaging` to `WANT`
* Click `Configure`
* Check the `Advanced` option
* Search for `vtkDICOM`
* Set `VTK_MODULE_ENABLE_VTK_vtkDICOM` to `WANT`
* Set `CMAKE_BUILD_TYPE` to `Release`
* Set the `CMAKE_INSTALL_PREFIX` to the location where VTK should be installed
  * <b>Note</b> I recommend to leave the default value.
* Click `Configure` again
* Click `Generate`
* Open the `build` directory with a terminal
* run the command `make` to compile VTK libraries
* run the command `sudo make install` to install VTK library

## vtkDICOM
<b>Note</b>: Leaving this here for documentation. We used to build this library from scrach and include it. However, we now include it as part of the VTK installation (as shown in the prervious steps with the vtkDICOM module).

Repository for module can be found [here](https://github.com/dgobbi/vtk-dicom).

## CUDA_v10.0 - OPTIONAL

<b>Note:</b> CUDA has not been tested on Mac.

The project can be enabled with CUDA. However, at the moment it is not being used for anything. Skip this installation until further instructions.

## Setting up FI3D

<b>Note: </b> If you don't want to use QtCreator, try the [Linux instructions](LinuxSetup.MD) instead (untested).

To setup FI3D, do the following:

Open the CMake file with Qt Creator. It'll run CMake on it and build and setup all dependencies. If it fails to find any of the libraries, give them the path as it was done with the previous libraries.

* Open Qt Creator
* Click `Projects`
* Click `Open Project`
* Select the FI3D CMake file
* Select `Release` on the left bottom pane (has a Monitor icon)
  * <b>Note:</b> can select `Release with Debug Information` when debugging is needed.
* Click `Configure Project`
* Select any component to include in the build
  * Components are identified by their variable options `Component_*_ENABLE`
* Select any module to include in the build
  * Modules are identified by their variable options `Module_*_ENABLE`
* Click `Run` (The green Triangle on the left pane, under Monitor icon)
