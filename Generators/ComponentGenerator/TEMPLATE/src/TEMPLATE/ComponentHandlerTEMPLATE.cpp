#include <TEMPLATE/ComponentHandlerTEMPLATE.h>

#include <fi3d/logger/Logger.h>

using namespace fi3d;
using namespace template;

/************************ FI3D Component Members ************************/
// Decide which REGISTER to use. Then delete the other REGISTER and this comment.
REGISTER_FI3D_COMPONENT(template::ComponentHandlerTEMPLATE, "COMPONENT-NAME", "TEMPLATE")
REGISTER_FI3D_COMPONENT_WITH_GUI(template::ComponentHandlerTEMPLATE, "COMPONENT-NAME", "TEMPLATE")

// @todo: Needing more fixes for this template file.

ComponentHandlerTEMPLATE::ComponentHandlerTEMPLATE(const QString& componentID)
	: ComponentHandler(sComponentName, sComponentAcronym, componentID),
	mGUI(new ComponentWindowTEMPLATE())
{
	//Ensure the data folder is created
	Filer::checkAndCreateDirectory(this->getComponentDataPath());

	// Create and set a default scene.
	ScenePtr scene(new Scene(mGUI->getSceneWidget(), componentID));
	this->setScene(scene);

	// TODO: Add visuals

	// Set the GUI
	this->setComponentWindow(mGUI);

	// Set up interactions
	this->setUpInteractions();

	// TODO: Add other component elements

	// TODO: Add additional objects here

	// Set up connections
	this->setUpConnections();

	// TODO: Add any other initiation logic here

	// Load configuration.
	this->applyConfig();
}

ComponentHandlerTEMPLATE::~ComponentHandlerTEMPLATE() {}

void ComponentHandlerTEMPLATE::setUpInteractions() {
	// TODO: Set up your interactions here.
}

void ComponentHandlerTEMPLATE::setUpConnections() {
	// TODO: Set up all the event connections here.
}

void ComponentHandlerTEMPLATE::applyConfig() {
	QJsonObject config = this->loadConfig();

	// TODO: Use the loaded config to load default/saved configuration.
}

void ComponentHandlerTEMPLATE::makeConfig() {
	QJsonObject config;

	// TODO: Save configuration to config object to save.
	
	this->saveConfig(config);
}