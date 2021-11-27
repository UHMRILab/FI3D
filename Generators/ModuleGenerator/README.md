# Module Instructions
The python v3 script `CreateModule.py` is available to assist with the creation of your module. Simply run the script and provide the following four inputs: module name, module acronym, and user ID (the ID is used as part of the documentation).

## Instructions
The module name is used as a unique identifier of the module and should be unique (no other module should have this name or it would interfere with such module). The acronym serves the same purpose but with a shorter name. Every file pertaining to the module uses this acronym as part of the directory and file names. This acronym should also be unique. The user ID  is used for documentation purposes in all the header files.

Once you run the script and provide the inputs, simply copy the created folder into the FI3D/modules directory and re-configure your project using CMake.

There are some commented `TODOs` in the generated files to help you start coding in the right place.

## Setting GUI
To modify the GUI, simply open the .ui file in the module's forms directory with the QtCreator application.
