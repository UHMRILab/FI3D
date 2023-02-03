# Windows FI3D Development Setup

## Visual Studio 2017/2019

The following instructions are under the assumption you are using Visual Studio 2019/2022. If using 2022, ensure the C++ v142 toolset is installed with it.

## Qt v6.4.2

<b>Note: </b>Follow these exact steps even if you are using Visual Studio 2022. There is no Qt v6.4.2 for VS2022, but its VS2019 version works for VS2022 when using the v142 toolset. If using VS2022, ensure v142 toolset is installed with it and add `v142` in CMake Configure window in the option field when building VTK and the FI3D.

* [Download Qt unified setup](https://www.qt.io/download)
* run it
  * hit next
  * create an account (or login if already have one)
  * Select where to install Qt (remember this directory)
  * Selecting components
    * Under Qt -> Qt 6.4.2
      * Select MSVC 2019 64-bit
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
    * Click next
  * Accept agreement and click next
  * click next
  * click install
* Add Qt to the path
  * Open System (windows key + Pause/Break)
  * Click Advanced System Settings (on left panel)
  * Click Environment Variables
  * Create a new variable called `QTDIR` with the value: `(Qt install path)\6.4.2\msvc2019_64`
    * e. g., `F:\Qt\6.4.2\msvc2019_64`
  * Add binaries and library to Path
    * edit the PATH variable
    * Add following Entry: `%QTDIR%\bin`
    * Add following Entry: `%QTDIR%\lib`
  * Click OK
* Add OpenSSL to the path
  * Open System (windows key + Pause/Break)
  * Click Advanced System Settings (on left panel)
  * Click Environment Variables
  * Create a new variable called `OPENSSL_DIR` with the value: `(Qt install path)\Tools\OpenSSL\Win_x64`
    * e. g., `F:\Qt\Tools\OpenSSL\Win_x64`
  * Add binaries and library to Path
    * edit the PATH variable
    * Add following Entry: `%OPENSSL_DIR%\bin`
  * Click OK
* Open Visual Studio
* Click `Tools` -> `Extension and Updates`
* Select Online (left panel)
* Search for Qt
* Download `Qt Visual Studio Tools`
* Close Visual studio
* Click Modify on new window
* Open Visual Studio
* Setup
  * Extensions -> Qt VS Tools -> Qt Options (top menu bar)
  * Click the `+` symbol
  * Under `Path` click on the folder icon
  * Navigate and select `(Qt install path)\6.4.2\msvc_2019_64\bin\qmake.exe`
  * Click OK

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
  * Select VS2019 with x64 platform
  * If using VS2022, put `v142` in the `Optional toolset` field
  * Click `Finish`
* Set `VTK_GROUP_ENABLE_Qt` to `WANT`
* Set `VTK_GROUP_ENABLE_Imaging` to `WANT`
* Click `Configure`
  * if CMake failed to find Qt, set `Qt6Dir` to where you installed Qt
    * Example: `(Qt6 Install Path)\6.4.2\msvc2019_64\lib\cmake\Qt6`
  * Click `Configure`
* Check the `Advanced` option
* Search for `vtkDICOM`
* Set `VTK_MODULE_ENABLE_VTK_vtkDICOM` to `WANT`
* Set the `CMAKE_INSTALL_PREFIX` to the location where VTK should be installed
  * Example: `C:/Program Files/VTK`
  * Note, if you choose `Program Files` as installation directory, create the `VTK` directory first or you'll need to run VS2019 as admin to install it properly.
* Click `Configure` again
* Click `Generate`
* Click `Open Project`, this will open the VS project
* Select the `Release` configuration
* Right click on the `ALL_BUILD` solution
* Click `Build`, wait for it to finish
* Right click on the `INSTALL`
* Click `Build`, wait for it to finish
  * If this step fails, you may need to reun Visual Studio with admin rights.
* Add VTK to the path
  * Open System (windows key + Pause/Break)
  * Click Advanced System Settings (on left panel)
  * Click Environment Variables
  * Create a new variable called `VTK_DIR` with VTK installation directory
    * e. g., F:\VTK
  * Add binaries to Path
    * edit the PATH variable
    * Add following Entry: `%VTK_DIR%\bin`
  * Click OK

## vtkDICOM
<b>Note</b>: Leaving this here for documentation. We used to build this library from scrach and include it. However, we now include it as part of the VTK installation (as shown in the prervious steps with the vtkDICOM module).

Repository for module can be found [here](https://github.com/dgobbi/vtk-dicom).

## CUDA_v10.0 - OPTIONAL

The project can be enabled with CUDA. However, at the moment it is not being used for anything. Skip this installation until further instructions.

## Setting up FI3D

To setup FI3D, do the following:

* Open CMake
* Click `Browse Source` and select `(FI3D Repo)/FI3D`
* Click `Browse Build`
  * Go to the same folder as in last step
  * Create a new folder called `build`
  * Select the `build` folder
* Click `Configure`
  * Select VS2019 with x64 platform
  * If using VS2022, put `v142` in the `Optional toolset` field
  * Click `Finish`
* If Qt DIRs not found
  * Locate the dir in `<Qt Install path>/6.4.2/msvc2019_64/lib/cmake/<QtComponent>`
* Select any component to include in the build
  * Components are identified by their variable options `Component_*_ENABLE`
* Select any module to include in the build
  * Modules are identified by their variable options `Module_*_ENABLE`
* Click `Configure`
* Click `Generate`
* Click `Open Project`
* On the Solution Explorer, do the following:
  * Right click on the `FI3D` target
  * Click on `Set as StartUp Project`

This visual studio solution is the one you'll use to develop in FI3D. When files are added/removed, the CMake `Configure` and `Generate` steps must be executed again.
