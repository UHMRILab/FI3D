# Windows FI3D Development Setup

## Visual Studio 2017/2019

The following instructions are under the assumption you are using Visual Studio 2017/2019/2022. If using VS2019/2022, ensure the C++ v141 toolset is installed with it.

## Qt v5.14.2

<b>Note: </b>Follow these exact steps even if you are using Visual Studio 2019/2022. There is no Qt v5.14.2 for VS2019/2022, but its VS2017 version works for VS2019/2022 when using the v141 toolset.

* [Download Qt unified setup](https://www.qt.io/download)
* run it
  * hit next
  * create an account (or login if already have one)
  * Select where to install Qt (remember this directory)
  * Selecting components
    * Under Qt -> Qt 5.14.2
      * Select MSVC 2017 64-bit
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
    * Click next
  * Accept agreement and click next
  * click next
  * click install
* Add Qt to the path
  * Open System (windows key + Pause/Break)
  * Click Advanced System Settings (on left panel)
  * Click Environment Variables
  * Create a new variable called `QTDIR` with the value: `(Qt install path)\5.14.2\msvc2017_64`
    * e. g., `F:\Qt\5.14.2\msvc2017_64`
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
  * Qt VS Tools -> Qt Options (top menu bar)
  * Check that Qt 2017 x64 is added
    * Name: msvc2017_64, Path: `(Qt install path)\5.14.2\msvc_2017_64`
    * If it's not listed, click add, and put those values.

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
  * select your version of VS with x64
  * For VS2019, type `v141` in the `Optional toolset` option
* Check the options `VTK_Group_Qt` and `VTK_Group_Imaging`
* Set the `CMAKE_INSTALL_PREFIX` to the location where VTK should be installed
* Click `Configure` again
* If CMake failed to find Qt
  * Set `Qt5Dir` to where you installed Qt: `(Qt5 Install Path)\5.14.1\msvc2017_64\lib\cmake\Qt5`
  * Click `Configure`
* Click `Generate`
* Click `Open Project`, this will open the VS project
* Select the `Release` configuration
* Right click on the `ALL_BUILD` solution
* Click `Build`, wait for it to finish
* Right click on the `INSTALL`
* Click `Build`, wait for it to finish
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

Clone the source code from [here](https://github.com/dgobbi/vtk-dicom). Then, follow these steps:

* Open CMake
* Click `Browse Source` and select the cloned directory
* Click `Browse Build`
  * Go to the same cloned directory
  * Create a new folder called `build`
  * Select the `build` folder
* Click `Configure`
* Select your version of VS with x64
* For VS2019, type "v141" in the "Optional toolset" option
* If CMake fails to find VTK
  * Set the directory to: `(VTK Install Path)/lib/cmake/vtk-8.2`
* Select `BUILD_SHARED_LIBRARIES`
* Click `Configure`
* Click `Generate`
* Click `Open Project`, this will open the VS project
* Select the `Release` configuration
* Right click on the `ALL_BUILD` solution
* Click `Build`, wait for it to finish
* Select the `RelWithDebInfo` configuration
* Right click on the `ALL_BUILD` solution
* Click `Build`, wait for it to finish

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
* If Qt DIRs not found
  * Locate the dir in `<Qt Install path>/5.14.2/msvc2017_64/lib/cmake/<QtComponent>`
* If DICOM_DIR is not found
  * Select the vtkDICOM library build directory you made in the [vtkDICOM instructions](#vtkDICOM_Dec23-2018)
  * Click `Configure`
* Select any module to include in the build
  * Modules are identified by their variable options `Module_*_ENABLE`
* Click `Configure`
* Click `Generate`
* Click `Open Project`
* On the Solution Explorer, do the following:
  * Right click on the `FI3D` target
  * Click on `Set as StartUp Project`

This visual studio solution is the one you'll use to develop in FI3D. When files are added/removed, the CMake `Configure` and `Generate` steps must be executed again.
