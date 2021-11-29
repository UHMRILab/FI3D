import os

# Get required information for creating module
moduleName = input("Enter module name, (ensure it's unique): ")
moduleAcronym = input("Enter module acronym (ensure it's unique): ")
user = input("Enter user creating this module (e.g., GitHub username): ")


print("Creating module \"" + moduleName + "\" with acronym \"" + moduleAcronym +
      "\" for user \"" + user)

# Create directories for new module
FORMS_DIRECTORY = moduleAcronym + "/forms"
INC_DIRECTORY = moduleAcronym + "/include/" + moduleAcronym
SRC_DIRECTORY = moduleAcronym + "/src/" + moduleAcronym

os.mkdir(moduleAcronym)
os.mkdir(moduleAcronym + "/include")
os.mkdir(moduleAcronym + "/src")

os.mkdir(FORMS_DIRECTORY)
os.mkdir(INC_DIRECTORY)
os.mkdir(SRC_DIRECTORY)

# Create the CMakeLists
print("Creating CMake list")

templateCmakeFile = open("TEMPLATE" + "/CMakeLists.txt", "r") 
templateCmake = templateCmakeFile.read()
templateCmakeFile.close()

templateCmake = templateCmake.replace("TEMPLATE", moduleAcronym)

cmakeFile = open(moduleAcronym + "/CMakeLists.txt", "w")
cmakeFile.write(templateCmake)
cmakeFile.close()

# Create the UI form
print("Creating UI form")

templateUIFile = open("TEMPLATE" + "/forms/ModuleWindowTEMPLATE.ui", "r")
templateUI = templateUIFile.read()
templateUIFile.close()

templateUI = templateUI.replace("TEMPLATE", moduleAcronym)

uiFile = open(FORMS_DIRECTORY + "/ModuleWindow" + moduleAcronym + ".ui", "w")
uiFile.write(templateUI)
uiFile.close()

# Create the headers
headers = os.listdir("TEMPLATE" + "/include/TEMPLATE")
for templateHeaderFileName in headers:
      if ".h" not in templateHeaderFileName:
            continue
      
      headerFileName = templateHeaderFileName.replace("TEMPLATE", moduleAcronym)

      print("Creating: " + INC_DIRECTORY + "/" + headerFileName)

      templateHeaderFile = open("TEMPLATE/include/TEMPLATE/" + templateHeaderFileName, 'r')
      templateHeader = templateHeaderFile.read()
      templateHeaderFile.close()

      templateHeader = templateHeader.replace("TEMPLATE", moduleAcronym)
      templateHeader = templateHeader.replace("template", moduleAcronym.lower())
      templateHeader = templateHeader.replace("MODULE-NAME", moduleName)
      templateHeader = templateHeader.replace("AUTHOR", user)

      headerFile = open(INC_DIRECTORY + "/" + headerFileName, "w")
      headerFile.write(templateHeader)
      headerFile.close()

# Crease the source
sources = os.listdir("TEMPLATE" + "/src/TEMPLATE")
for templateSourceFileName in sources:
      if ".cpp" not in templateSourceFileName:
            continue

      sourceFileName = templateSourceFileName.replace("TEMPLATE", moduleAcronym)

      print("Creating: " + SRC_DIRECTORY + "/" + sourceFileName)

      templateSourceFile = open("TEMPLATE/src/TEMPLATE/" + templateSourceFileName, 'r')
      templateSource = templateSourceFile.read()
      templateSourceFile.close()

      templateSource = templateSource.replace("TEMPLATE", moduleAcronym)
      templateSource = templateSource.replace("template", moduleAcronym.lower())
      templateSource = templateSource.replace("MODULE-NAME", moduleName)

      sourceFile = open(SRC_DIRECTORY + "/" + sourceFileName, "w")
      sourceFile.write(templateSource)
      sourceFile.close()
      
# Create the README
print("Creating README file")
templateReadmeFile = open("TEMPLATE/README.md", 'r')
templateReadme = templateReadmeFile.read()
templateReadmeFile.close()

templateReadme = templateReadme.replace("TEMPLATE", moduleAcronym)
templateReadme = templateReadme.replace("MODULE-NAME", moduleName)

readmeFIle = open(moduleAcronym + "/README.md", "w")
readmeFIle.write(templateReadme)
readmeFIle.close()