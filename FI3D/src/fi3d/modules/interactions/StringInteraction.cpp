#include <fi3d/modules/interactions/StringInteraction.h>

using namespace fi3d;

StringInteraction::StringInteraction(const QString& id, const QString& value, const QString& information)
	: ModuleInteraction(id, value, information),
	mConstraint(EModuleInteractionConstraint::UNKNOWN)
{}

StringInteraction::~StringInteraction() {}

EModuleInteraction StringInteraction::getInteractionType() const {
	return EModuleInteraction::STRING;
}

EModuleInteractionConstraint StringInteraction::getConstraintType() const {
	return mConstraint;
}

void StringInteraction::setValueAsJson(const QJsonValue& value) {
	this->setValue(value.toString());
}

QString StringInteraction::getValue() const {
	return this->getValueAsJson().toString();
}

void StringInteraction::setValue(const QString& value) {
	if (this->getValue() == value) {
		return;
	}

	this->setJsonValue(value);
	emit changedValue(value);
}
