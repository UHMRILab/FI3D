#include <fi3d/modules/ModuleElement.h>

#include <fi3d/FI3D/FI3D.h>

#include <fi3d/modules/CommandParser.h>
#include <fi3d/modules/IFeedbackEmitter.h>
#include <fi3d/modules/ModuleHandler.h>

#include <fi3d/rendering/scenes/Scene.h>

using namespace fi3d;

ModuleElement::ModuleElement()
	: mModuleInformation(new ModuleInformation("", "", ""))
{
}

ModuleElement::~ModuleElement() {}

QString ModuleElement::getModuleName() const {
	return mModuleInformation->getModuleName();
}

QString ModuleElement::getModuleAcronym() const {
	return mModuleInformation->getModuleAcronym();
}

QString ModuleElement::getModuleID() const {
	return mModuleInformation->getModuleID();
}

QString ModuleElement::getModuleAssetsPath() const {
	return QObject::tr("%1/%2/assets").arg(FI3D::MODULES_ASSETS_DIRECTORY).arg(this->getModuleAcronym());
}

QString ModuleElement::getModuleDataPath() const {
	return QObject::tr("%1/%2/data").arg(FI3D::MODULES_DATA_DIRECTORY).arg(this->getModuleAcronym());
}

ModuleHandler* ModuleElement::getModuleHandler() {
	return mModuleInformation->getModuleHandler();
}

QSharedPointer<Scene> ModuleElement::getMainScene() {
	return mModuleInformation->getMainScene();
}

QSharedPointer<ModuleMessageEncoder> ModuleElement::getMessageEncoder() {
	return mModuleInformation->getMessageEncoder();
}

QSharedPointer<CommandParser> ModuleElement::getCommandParser() {
	return mModuleInformation->getCommandParser();
}

QSharedPointer<ModuleWindow> ModuleElement::getModuleWindow() {
	return mModuleInformation->getModuleWindow();
}

QList<ModuleInteractionPtr> ModuleElement::getModuleInteractions() {
	return mModuleInformation->getModuleInteractions();
}

ModuleInteractionPtr ModuleElement::getModuleInteraction(const QString & id) {
	return mModuleInformation->getModuleInteraction(id);
}

ValuelessInteractionPtr ModuleElement::getValuelessModuleInteraction(const QString& id) {
	ModuleInteractionPtr inte = this->getModuleInteraction(id);
	return qobject_cast<ValuelessInteractionPtr>(inte);
}

BooleanInteractionPtr ModuleElement::getBooleanModuleInteraction(const QString& id) {
	ModuleInteractionPtr inte = this->getModuleInteraction(id);
	return qobject_cast<BooleanInteractionPtr>(inte);
}

IntegerInteractionPtr ModuleElement::getIntegerModuleInteraction(const QString& id) {
	ModuleInteractionPtr inte = this->getModuleInteraction(id);
	return qobject_cast<IntegerInteractionPtr>(inte);
}

FloatInteractionPtr ModuleElement::getFloatModuleInteraction(const QString& id) {
	ModuleInteractionPtr inte = this->getModuleInteraction(id);
	return qobject_cast<FloatInteractionPtr>(inte);
}

StringInteractionPtr ModuleElement::getStringModuleInteraction(const QString& id) {
	ModuleInteractionPtr inte = this->getModuleInteraction(id);
	return qobject_cast<StringInteractionPtr>(inte);
}

SelectInteractionPtr ModuleElement::getSelectModuleInteraction(const QString& id) {
	ModuleInteractionPtr inte = this->getModuleInteraction(id);
	return qobject_cast<SelectInteractionPtr>(inte);
}

bool ModuleElement::setModuleInteractionValue(const QString& interactionID, const QJsonValue& value) {
	ModuleInteractionPtr inte = this->getModuleInteraction(interactionID);
	if (inte.isNull()) {
		return false;
	} else {
		inte->setValueAsJson(value);
		return true;
	}
}

QJsonValue ModuleElement::getModuleInteractionValue(const QString& interactionID) {
	ModuleInteractionPtr inte = this->getModuleInteraction(interactionID);
	if (inte.isNull()) {
		return QJsonValue();
	} else {
		return inte->getValueAsJson();
	}
}

ModuleInformation* ModuleElement::getModuleInformationObject() const {
	return mModuleInformation.data();
}

void ModuleElement::moduleInformationAssigned(ModuleInformationPtr moduleInfo) {}

void ModuleElement::setModuleInformation(ModuleInformationPtr moduleInfo) {
	mModuleInformation = moduleInfo;
	this->moduleInformationAssigned(mModuleInformation);
}

void ModuleElement::clearModuleInformation() {
	mModuleInformation.reset(new ModuleInformation("", "", ""));
}
