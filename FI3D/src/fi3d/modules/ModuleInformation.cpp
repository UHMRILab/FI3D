#include <fi3d/modules/ModuleInformation.h>

#include <fi3d/logger/Logger.h>

#include <fi3d/modules/ModuleMessageEncoder.h>

using namespace fi3d;

ModuleInformation::ModuleInformation(const QString& moduleName, const QString& moduleAcronym, const QString& moduleID)
	: QObject(Q_NULLPTR),
	mModuleName(moduleName),
	mModuleAcronym(moduleAcronym),
	mModuleID(moduleID),
	mModuleHandler(Q_NULLPTR),
	mMainScene(Q_NULLPTR),
	mMessageEncoder(Q_NULLPTR),
	mCommandParser(Q_NULLPTR),
	mModuleWindow(Q_NULLPTR),
	mInteractions(), mInteractionsMap()
{}

ModuleInformation::~ModuleInformation() {}

QString ModuleInformation::getModuleName() const {
	return mModuleName;
}

QString ModuleInformation::getModuleAcronym() const {
	return mModuleAcronym;
}

QString ModuleInformation::getModuleID() const {
	return mModuleID;
}

bool ModuleInformation::operator==(const ModuleInformation& other) const {
	return (*this == other.mModuleID);
}

bool ModuleInformation::operator==(const QString& other) const {
	return mModuleID == other;
}

bool ModuleInformation::operator!=(const ModuleInformation& other) const {
	return !(*this == other);
}

bool ModuleInformation::operator!=(const QString& other) const {
	return !(*this == other);
}

ModuleHandler* ModuleInformation::getModuleHandler() const {
	return mModuleHandler;
}

QSharedPointer<Scene> ModuleInformation::getMainScene() const {
	return mMainScene;
}

QSharedPointer<ModuleMessageEncoder> ModuleInformation::getMessageEncoder() const {
	return mMessageEncoder;
}

QSharedPointer<CommandParser> ModuleInformation::getCommandParser()  const {
	return mCommandParser;
}

QSharedPointer<ModuleWindow> ModuleInformation::getModuleWindow()  const {
	return mModuleWindow;
}

QList<ModuleInteractionPtr> ModuleInformation::getModuleInteractions()  const {
	return mInteractions;
}

ModuleInteractionPtr ModuleInformation::getModuleInteraction(const QString& interactionID)  const {
	int index = mInteractionsMap.value(interactionID, -1);
	if (index == -1) {
		return Q_NULLPTR;
	} else {
		return mInteractions.at(index);
	}
}

void ModuleInformation::setModuleID(const QString & newID) {
	QString oldID = mModuleID;
	mModuleID = newID;

	emit changedModuleID(oldID, mModuleID);
}

void ModuleInformation::setModuleHandler(ModuleHandler* moduleHandler) {
	mModuleHandler = moduleHandler;
}

void ModuleInformation::setMainScene(QSharedPointer<Scene> scene) {
	mMainScene = scene;
	mMessageEncoder->setupScene();
	emit changedMainScene();
}

void ModuleInformation::setMessageEncoder(QSharedPointer<ModuleMessageEncoder> messageEncoder) {
	mMessageEncoder = messageEncoder;
	mMessageEncoder->setupScene();
}

void ModuleInformation::setCommandParser(QSharedPointer<CommandParser> commandParser) {
	mCommandParser = commandParser;
}

void ModuleInformation::setModuleWindow(QSharedPointer<ModuleWindow> moduleWindow) {
	mModuleWindow = moduleWindow;
}

void ModuleInformation::addModuleInteraction(ModuleInteractionPtr moduleInteraction) {
	if (moduleInteraction.isNull()) {
		return;
	}

	if (!mInteractionsMap.contains(moduleInteraction->getInteractionID())) {
		int index = mInteractions.count();
		mInteractions.append(moduleInteraction);
		mInteractionsMap.insert(moduleInteraction->getInteractionID(), index);
		emit changedAddedInteraction(moduleInteraction->getInteractionID());
	} else {
		qWarning() << "Failed to add interaction because an interaction with the ID:" <<
			moduleInteraction->getInteractionID() << "is already present.";
	}
}

void ModuleInformation::removeModuleInteraction(const QString& interactionID) {
	if (mInteractionsMap.contains(interactionID)) {
		mInteractionsMap.remove(interactionID);
		emit changedRemovedInteraction(interactionID);
	}
}
