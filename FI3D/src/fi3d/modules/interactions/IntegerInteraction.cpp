#include <fi3d/modules/interactions/IntegerInteraction.h>

#include <fi3d/logger/Logger.h>

using namespace fi3d;

IntegerInteraction::IntegerInteraction(const QString& id, const int& value, const QString& information)
	: ModuleInteraction(id, value, information),
	mConstraint(EModuleInteractionConstraint::UNKNOWN),
	mMin(0),
	mMax(0)
{}

IntegerInteraction::~IntegerInteraction() {}

EModuleInteraction IntegerInteraction::getInteractionType() const {
	return EModuleInteraction::INTEGER;
}

EModuleInteractionConstraint IntegerInteraction::getConstraintType() const {
	return mConstraint;
}

void IntegerInteraction::setValueAsJson(const QJsonValue& value) {
	this->setValue(value.toInt());
}

int IntegerInteraction::getValue() const {
	return this->getValueAsJson().toInt();
}

void IntegerInteraction::setValue(const int& value) {
	qDebug() << "Enter";
	qDebug() << "Setting" << this->getInteractionID() << "value:" << value;
	if (value == this->getValue()) {
		qDebug() << "Exit - Value is already set.";
		return;
	}

	int newValue = value;

	switch (mConstraint.toInt()) {
		case EModuleInteractionConstraint::UNKNOWN:
			break;
		case EModuleInteractionConstraint::MIN:
			if (value < mMin) {
				newValue = mMin;
			}
			break;
		case EModuleInteractionConstraint::MAX:
			if (value > mMax) {
				newValue = mMax;
			}
			break;
		case EModuleInteractionConstraint::RANGE:
			if (value < mMin) {
				newValue = mMin;
			} else if (value > mMax) {
				newValue = mMax;
			}
			break;
		default:
			qDebug() << "Exit - interaction constraint is invalid";
			return;
	}

	this->setJsonValue(newValue);
	emit changedValue(newValue);
	qDebug() << "Exit";
}

void IntegerInteraction::setMin(const int& min) {
	if (mMin == min) {
		return;
	}

	mMin = min;
	emit changedMinimum(mMin);
	emit changedInteractionConstraint();

	if (this->getValue() < mMin) {
		this->setValue(mMin);
	}
}

void IntegerInteraction::setMax(const int& max) {
	if (mMax == max) {
		return;
	}

	mMax = max;
	emit changedMaximum(mMax);
	emit changedInteractionConstraint();

	if (this->getValue() > mMax) {
		this->setValue(mMax);
	}
}

void IntegerInteraction::setRange(const int& min, const int& max) {
	if (mMin == min && mMax == max) {
		return;
	}

	mMin = min;
	mMax = max;
	emit changedRange(mMin, mMax);
	emit changedInteractionConstraint();

	if (this->getValue() < mMin) {
		this->setValue(mMin);
	} else if (this->getValue() > mMax) {
		this->setValue(mMax);
	}
}

int IntegerInteraction::getMin() const {
	return mMin;
}

int IntegerInteraction::getMax() const {
	return mMax;
}

IntegerInteractionPtr IntegerInteraction::createMinInteraction(
	const QString& id, const int& value, const int& min, const QString& info) 
{
	int val = value;
	if (val < min) {
		val = min;
	}

	IntegerInteractionPtr interaction(new IntegerInteraction(id, val, info));
	interaction->mConstraint = EModuleInteractionConstraint::MIN;
	interaction->setMin(min);

	return interaction;
}

IntegerInteractionPtr IntegerInteraction::createMaxInteraction(
	const QString& id, const int& value, const int& max, const QString& info) 
{
	int val = value;
	if (val > max) {
		val = max;
	}

	IntegerInteractionPtr interaction(new IntegerInteraction(id, val, info));
	interaction->mConstraint = EModuleInteractionConstraint::MAX;
	interaction->setMax(max);

	return interaction;
}

IntegerInteractionPtr IntegerInteraction::createRangeInteraction(
	const QString& id, const int& value, const int& min, const int& max,
	const QString & info) 
{
	int val = value;
	if (val < min) {
		val = min;
	} else if (val > max) {
		val = max;
	}

	IntegerInteractionPtr interaction(new IntegerInteraction(id, val, info));
	interaction->mConstraint = EModuleInteractionConstraint::RANGE;
	interaction->setRange(min, max);

	return interaction;
}
