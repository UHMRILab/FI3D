# Component Instructions
The python v3 script `CreateComponent.py` is available to assist with the creation of your component. Simply run the script and provide the following four inputs: component name, component acronym, and user ID (the ID is used as part of the documentation).

## Instructions
The component name is used as a unique identifier of the component and should be unique (no other component should have this name or it would interfere with such component). The acronym serves the same purpose but with a shorter name. Every file pertaining to the component uses this acronym as part of the directory and file names. This acronym should also be unique. The user ID  is used for documentation purposes in all the header files.

Once you run the script and provide the inputs, simply copy the created folder into the FI3D/components directory and re-configure your project using CMake.

There are some commented `TODOs` in the generated files to help you start coding in the right place.

<!-- ## Setting GUI
To modify the GUI, simply open the .ui file in the component's forms directory with the QtCreator application.
 -->