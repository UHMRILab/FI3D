# Linux FI3D Development Setup

## Linux Pre-setup

On a new system, make sure the following are applied/installed:

```
sudo apt-get install make               // this installs make
sudo apt-get install build-essential    // This installs c++ stuff
sudo apt-get install libgl1-mesa-dev    // This installs OpenGL
sudo apt-get install libxt-dev          // This installs some dependency called "x11_xt_lib"
```
<b>Note:</b> Deployable version for Linux is currently non-functional. Deplyable feature is only functional on Windows.

### Qt v5.14.2

To install Qt, follow these steps:

* [Download Qt unified setup](https://www.qt.io/download)
* run it
  * hit next
  * create an account (or login if already have one)
  * Select where to install Qt (remember this directory)
  * Selecting components
    * Select Archive and LTS from the left pane
    * Click Refresh
    * Under Qt -> Qt 5.14.1
      * Select gcc 64 bit.
      * Select the following as well
        * Qt Charts
        * Qt Data Visualization
        * Qt Purchasing
        * Qt Virtual Keyboard
        * Qt WebEngine
        * Qt Network Authorization
        * Qt WebGL Streaming Plugin
    * Under `Developer and Designer Tools`
      * Select `OpenSSL 1.1.1j Toolkit`
      * <b>Note:</b> OpenSSL features have not been tested on Linux.
    * Click next
  * Accept agreement and click next
  * click next
  * click install

## VTK_v8.2.0

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
* Check the options `VTK_Group_Qt` and `VTK_Group_Imaging`
* Set `CMAKE_BUILD_TYPE` to `Release`
* Set the `CMAKE_INSTALL_PREFIX` to the location where VTK should be installed
  * <b>Note</b> I recommend to leave the default value.
* Open the `build` directory with a terminal
* run the command `make` to compile VTK libraries
* run the command `sudo make install` to install VTK library

## vtkDICOM

Clone the source code from [here](https://github.com/dgobbi/vtk-dicom). Then, follow these steps:

* Open CMake
* Click `Browse Source` and select the cloned directory
* Click `Browse Build`
  * Go to the same cloned directory
  * Create a new folder called `build`
  * Select the `build` folder
* Click `Configure`
* Select Unix Makefiles
* If CMake fails to find VTK
  * Set the directory to: `(VTK Install Path)/lib/cmake/vtk-8.2`
* Select `BUILD_SHARED_LIBRARIES`
* Set `CMAKE_BUILD_TYPE` to `Release`
* Click `Configure`
* Click `Generate`
* Open the `build` directory with a terminal
* run the command `make` to compile VTKDICOM libraries
* run the command `sudo make install` to install VTKDICOM

### CUDA_v10.0 - OPTIONAL

<b>Note:</b> CUDA has not been tested on Linux.

The project can be enabled with CUDA. However, at the moment it is not being used for anything. Skip this installation until further instructions.

## Setting up FI3D

To setup FI3D, do the following:

* Open CMake
* Click `Browse Source` and select (HICS_Repo)/HICS
* Click `Browse Build`
  * Go to the same folder as in last step
  * Create a new folder called `build`
  * Select the `build` folder
* Click `Configure`
* If Qt DIRs not found
  * Locate the dir in `<Qt Install path>/5.14.2/msvc2017_64/lib/cmake/<QtComponent>`
* Set `CMAKE_BUILD_TYPE` to `Release`
* Select modules to include in the build
  * Modules are identified by their variable options `Module_*_ENABLE`
* Click `Configure`
* Click `Generate`
* Open the `build` directory with a terminal
* run the command `make` to compile FI3D

To run FI3D, open a terminal in the build directory and do the following:

``` Shell
LD_LIBRARY_PATH=/usr/local/lib:<Directory where Qt was installed>/5.14.2/gcc_64/lib
export LD_LIBRARY_PATH
./FI3D
```

This above script assumes that VTK was installed in the default location (/usr/local). If VTK was installed elsewhere, replace the first path to where it was installed.
