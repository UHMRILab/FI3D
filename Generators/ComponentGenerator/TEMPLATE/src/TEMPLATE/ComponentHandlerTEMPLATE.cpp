#include <TEMPLATE/ModuleHandlerTEMPLATE.h>

#include <fi3d/logger/Logger.h>

#include <fi3d/FI3D/FI3D.h>
#include <fi3d/data/Filer.h>

using namespace fi3d;
using namespace template;

REGISTER_FI3D_MODULE(template::ModuleHandlerTEMPLATE, "MODULE-NAME", "TEMPLATE")

ModuleHandlerTEMPLATE::ModuleHandlerTEMPLATE(const QString& moduleID)
	: ModuleHandler(sModuleName, sModuleAcronym, moduleID),
	mGUI(new ModuleWindowTEMPLATE())
{
	//Ensure the data folder is created
	Filer::checkAndCreateDirectory(this->getModuleDataPath());

	// Create and set a default scene.
	ScenePtr scene(new Scene(mGUI->getSceneWidget(), moduleID));
	this->setScene(scene);

	// TODO: Add visuals

	// Set the GUI
	this->setModuleWindow(mGUI);

	// Set up interactions
	this->setUpInteractions();

	// TODO: Add other module elements

	// TODO: Add additional objects here

	// Set up connections
	this->setUpConnections();

	// TODO: Add any other initiation logic here

	// Load configuration.
	this->applyConfig();
}

ModuleHandlerTEMPLATE::~ModuleHandlerTEMPLATE() {}

void ModuleHandlerTEMPLATE::setUpInteractions() {
	// TODO: Set up your interactions here.
}

void ModuleHandlerTEMPLATE::setUpConnections() {
	// TODO: Set up all the event connections here.
}

void ModuleHandlerTEMPLATE::applyConfig() {
	QJsonObject config = this->loadConfig();

	// TODO: Use the loaded config to load default/saved configuration.
}

void ModuleHandlerTEMPLATE::makeConfig() {
	QJsonObject config;

	// TODO: Save configuration to config object to save.
	
	this->saveConfig(config);
}