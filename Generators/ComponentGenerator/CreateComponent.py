import os

# Get required information for creating component
componentName = input("Enter component name, (ensure it's unique): ")
componentAcronym = input("Enter component acronym (ensure it's unique): ")
user = input("Enter user creating this component (e.g., GitHub username): ")


print("Creating component \"" + componentName + "\" with acronym \"" + componentAcronym +
      "\" for user \"" + user + "\"")

# Create directories for new component
FORMS_DIRECTORY = componentAcronym + "/forms"
INC_DIRECTORY = componentAcronym + "/include/" + componentAcronym
SRC_DIRECTORY = componentAcronym + "/src/" + componentAcronym

os.mkdir(componentAcronym)
os.mkdir(componentAcronym + "/include")
os.mkdir(componentAcronym + "/src")

os.mkdir(FORMS_DIRECTORY)
os.mkdir(INC_DIRECTORY)
os.mkdir(SRC_DIRECTORY)

# Create the CMakeLists
print("Creating CMake list")

templateCmakeFile = open("TEMPLATE" + "/CMakeLists.txt", "r") 
templateCmake = templateCmakeFile.read()
templateCmakeFile.close()

templateCmake = templateCmake.replace("TEMPLATE", componentAcronym)
templateCmake = templateCmake.replace("COMPONENT-NAME", componentName)

cmakeFile = open(componentAcronym + "/CMakeLists.txt", "w")
cmakeFile.write(templateCmake)
cmakeFile.close()

# Create the UI form
print("Creating UI form")

templateUIFile = open("TEMPLATE" + "/forms/ComponentWindowTEMPLATE.ui", "r")
templateUI = templateUIFile.read()
templateUIFile.close()

templateUI = templateUI.replace("TEMPLATE", componentAcronym)

uiFile = open(FORMS_DIRECTORY + "/ComponentWindow" + componentAcronym + ".ui", "w")
uiFile.write(templateUI)
uiFile.close()

# Create the headers
headers = os.listdir("TEMPLATE" + "/include/TEMPLATE")
for templateHeaderFileName in headers:
      if ".h" not in templateHeaderFileName:
            continue
      
      headerFileName = templateHeaderFileName.replace("TEMPLATE", componentAcronym)

      print("Creating: " + INC_DIRECTORY + "/" + headerFileName)

      templateHeaderFile = open("TEMPLATE/include/TEMPLATE/" + templateHeaderFileName, 'r')
      templateHeader = templateHeaderFile.read()
      templateHeaderFile.close()

      templateHeader = templateHeader.replace("TEMPLATE", componentAcronym)
      templateHeader = templateHeader.replace("template", componentAcronym.lower())
      templateHeader = templateHeader.replace("COMPONENT-NAME", componentName)
      templateHeader = templateHeader.replace("AUTHOR", user)

      headerFile = open(INC_DIRECTORY + "/" + headerFileName, "w")
      headerFile.write(templateHeader)
      headerFile.close()

# Create the source
sources = os.listdir("TEMPLATE" + "/src/TEMPLATE")
for templateSourceFileName in sources:
      if ".cpp" not in templateSourceFileName:
            continue

      sourceFileName = templateSourceFileName.replace("TEMPLATE", componentAcronym)

      print("Creating: " + SRC_DIRECTORY + "/" + sourceFileName)

      templateSourceFile = open("TEMPLATE/src/TEMPLATE/" + templateSourceFileName, 'r')
      templateSource = templateSourceFile.read()
      templateSourceFile.close()

      templateSource = templateSource.replace("TEMPLATE", componentAcronym)
      templateSource = templateSource.replace("template", componentAcronym.lower())
      templateSource = templateSource.replace("COMPONENT-NAME", componentName)

      sourceFile = open(SRC_DIRECTORY + "/" + sourceFileName, "w")
      sourceFile.write(templateSource)
      sourceFile.close()
      
# Create the README
print("Creating README file")
templateReadmeFile = open("TEMPLATE/README.md", 'r')
templateReadme = templateReadmeFile.read()
templateReadmeFile.close()

templateReadme = templateReadme.replace("TEMPLATE", componentAcronym)
templateReadme = templateReadme.replace("COMPONENT-NAME", componentName)

readmeFIle = open(componentAcronym + "/README.md", "w")
readmeFIle.write(templateReadme)
readmeFIle.close()