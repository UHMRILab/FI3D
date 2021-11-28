#include <fi3d/modules/interactions/ValuelessInteraction.h>

using namespace fi3d;

ValuelessInteraction::ValuelessInteraction(const QString& id, const QString& information)
	: ModuleInteraction(id, QJsonValue::Null, information)
{
}

ValuelessInteraction::~ValuelessInteraction() {}

EModuleInteraction ValuelessInteraction::getInteractionType() const {
	return EModuleInteraction::VALUELESS;
}

void ValuelessInteraction::setValueAsJson(const QJsonValue& value) {
	this->triggerInteraction();
}

void ValuelessInteraction::triggerInteraction() {
	emit triggered();
}
