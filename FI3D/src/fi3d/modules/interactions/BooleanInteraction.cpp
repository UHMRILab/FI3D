#include <fi3d/modules/interactions/BooleanInteraction.h>

using namespace fi3d;

BooleanInteraction::BooleanInteraction(const QString& id, const bool& value, const QString& information) 
	: ModuleInteraction(id, value, information)
{}

BooleanInteraction::~BooleanInteraction() {}

EModuleInteraction BooleanInteraction::getInteractionType() const {
	return EModuleInteraction::BOOL;
}

void BooleanInteraction::setValueAsJson(const QJsonValue& value) {
	this->setValue(value.toBool());
}

void BooleanInteraction::setValue(const bool& value) {
	if (this->getValue() == value) {
		return;
	}

	this->setJsonValue(value);
	emit changedValue(value);
}

bool BooleanInteraction::getValue() const {
	return this->getValueAsJson().toBool();
}
