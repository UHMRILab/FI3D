#include <fi3d/modules/interactions/ModuleInteraction.h>

#include <fi3d/logger/Logger.h>

using namespace fi3d;

ModuleInteraction::ModuleInteraction(const QString& id, const QJsonValue& value, const QString& information)
	: QObject(),
	mID(id),
	mValue(value),
	mInfo(information),
	mIsEnabledForFrameworkInterface(true)
{
}

ModuleInteraction::~ModuleInteraction() {}

QString ModuleInteraction::getInteractionID() const {
	return mID;
}

EModuleInteractionConstraint ModuleInteraction::getConstraintType() const {
	return EModuleInteractionConstraint::UNKNOWN;
}

void ModuleInteraction::setJsonValue(const QJsonValue& value) {
	qDebug() << "Enter";
	qDebug() << "Changing value of" << mID << "to" << value;
	
	mValue = value;
	emit changedInteractionValue(mValue);
	
	qDebug() << "Exit";
}

QJsonValue ModuleInteraction::getValueAsJson() const {
	return mValue;
}

QString ModuleInteraction::getInformation() const {
	return mInfo;
}

bool ModuleInteraction::isEnabledForFrameworkInterfaces() const {
	return mIsEnabledForFrameworkInterface;
}

void ModuleInteraction::disableForFrameworkInterfaces() {
	mIsEnabledForFrameworkInterface = false;
}

void ModuleInteraction::setInformation(const QString& information) {
	mInfo = information;
}
