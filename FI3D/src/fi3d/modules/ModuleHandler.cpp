#include <fi3d/modules/ModuleHandler.h>

#include <fi3d/logger/Logger.h>

#include <fi3d/data/Filer.h>

using namespace fi3d;

ModuleHandler::ModuleHandler(const QString& moduleName, const QString& moduleAcronym, const QString& moduleID)
	: QObject(),
	ModuleElement()
{
	qDebug() << "Enter";

	ModuleMessageEncoderPtr encoder(new ModuleMessageEncoder());
	CommandParserPtr parser(new CommandParser());

	ModuleInformationPtr modInfo(new ModuleInformation(moduleName, moduleAcronym, moduleID));
	modInfo->setModuleHandler(this);
	modInfo->setMessageEncoder(encoder);
	modInfo->setCommandParser(parser);
	this->setModuleInformation(modInfo);

	encoder->setModuleInformation(modInfo);
	encoder->addFeedbackPropagator(this);

	parser->setModuleInformation(modInfo);
	parser->addFeedbackPropagator(this);

	qDebug() << "Exit";
}

ModuleHandler::~ModuleHandler() {
	ModuleWindowPtr window = this->getModuleWindow();
	if (!window.isNull() && window->isVisible()) {
		window->close();
	}
}

void ModuleHandler::reset() {}

void ModuleHandler::pause() {}

void ModuleHandler::resume() {}

void ModuleHandler::closeModule() {
	emit requestCloseModule();
}

void ModuleHandler::showModuleWindow() {
	if (!this->getModuleWindow().isNull()) {
		this->getModuleWindow()->showMaximized();
	}
}

void ModuleHandler::attachModuleElement(ModuleElement* moduleElement) {
	if (moduleElement == Q_NULLPTR) {
		qWarning() << "Failed to attach element. Object is not compatible.";
		return;
	}

	moduleElement->setModuleInformation(mModuleInformation);
}

void ModuleHandler::saveConfig(const QJsonObject& config) {
	QString path = tr("%1/config.json").arg(this->getModuleDataPath());
	Filer::saveJSONFile(config, path);
}

QJsonObject ModuleHandler::loadConfig() {
	QString path = tr("%1/config.json").arg(this->getModuleDataPath());
	return Filer::readJSONFile(path);
}

void ModuleHandler::setScene(ScenePtr scene) {
	qDebug() << "Entry";

	ScenePtr currentScene = this->getMainScene();
	if (currentScene == scene) {
		return;
	}

	if (!currentScene.isNull()) {
		currentScene->removeFeedbackPropagator(this);
		currentScene->clearModuleInformation();
	}

	if (!scene.isNull()) {
		scene->setModuleInformation(mModuleInformation);
		scene->addFeedbackPropagator(this);
	}

	mModuleInformation->setMainScene(scene);

	qDebug() << "Exit";
}

void ModuleHandler::setModuleWindow(ModuleWindowPtr window) {
	qDebug() << "Enter";
	ModuleWindowPtr currentWindow = this->getModuleWindow();

	if (currentWindow == window) {
		qDebug() << "Exit - Re-assigning the same ModuleWindow.";
		return;
	}

	if (!currentWindow.isNull()) {
		currentWindow->disconnectFeedbackEmitter(this);
		currentWindow->clearModuleInformation();
	}

	if (!window.isNull()) {
		window->setModuleInformation(mModuleInformation);
		window->connectFeedbackEmitter(this);
		window->setWindowTitle(mModuleInformation->getModuleID());
	}

	mModuleInformation->setModuleWindow(window);

	qDebug() << "Exit";
}

void ModuleHandler::addModuleInteraction(ModuleInteractionPtr moduleInteraction) {
	mModuleInformation->addModuleInteraction(moduleInteraction);
}

void ModuleHandler::removeModuleInteraction(const QString& interactionID) {
	mModuleInformation->removeModuleInteraction(interactionID);
}