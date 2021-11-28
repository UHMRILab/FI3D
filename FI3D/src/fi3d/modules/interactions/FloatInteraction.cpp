#include <fi3d/modules/interactions/FloatInteraction.h>

using namespace fi3d;

FloatInteraction::FloatInteraction(const QString& id, const double& value, const QString& information)
	: ModuleInteraction(id, value, information),
	mConstraint(EModuleInteractionConstraint::UNKNOWN),
	mMin(0),
	mMax(0)
{}

FloatInteraction::~FloatInteraction() {}

EModuleInteraction FloatInteraction::getInteractionType() const {
	return EModuleInteraction::FLOAT;
}

EModuleInteractionConstraint FloatInteraction::getConstraintType() const {
	return mConstraint;
}

void FloatInteraction::setValueAsJson(const QJsonValue& value) {
	this->setValue(value.toDouble());
}

double FloatInteraction::getValue() const {
	return this->getValueAsJson().toInt();
}

void FloatInteraction::setValue(const double& value) {
	if (this->getValue() == value) {
		return;
	}

	double newValue = value;

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
			return;
	}

	this->setJsonValue(newValue);
	emit changedValue(newValue);
}

void FloatInteraction::setMin(const double& min) {
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

void FloatInteraction::setMax(const double& max) {
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

void FloatInteraction::setRange(const double& min, const double& max) {
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

double FloatInteraction::getMin() const {
	return mMin;
}

double FloatInteraction::getMax() const {
	return mMax;
}

FloatInteractionPtr FloatInteraction::createMinInteraction(const QString& id, 
	const double& value, const double& min, const QString& info) 
{
	double val = value;
	if (val < min) {
		val = min;
	}

	FloatInteractionPtr interaction(new FloatInteraction(id, val, info));
	interaction->mConstraint = EModuleInteractionConstraint::MIN;
	interaction->setMin(min);

	return interaction;
}

FloatInteractionPtr FloatInteraction::createMaxInteraction(const QString& id, 
	const double& value, const double& max, const QString& info) 
{
	double val = value;
	if (val > max) {
		val = max;
	}

	FloatInteractionPtr interaction(new FloatInteraction(id, val, info));
	interaction->mConstraint = EModuleInteractionConstraint::MAX;
	interaction->setMax(max);

	return interaction;
}

FloatInteractionPtr FloatInteraction::createRangeInteraction(const QString& id,
	const double& value, const double& min, const double& max, 
	const QString & info) 
{
	double val = value;
	if (val < min) {
		val = min;
	} else if (val > max) {
		val = max;
	}

	FloatInteractionPtr interaction(new FloatInteraction(id, val, info));
	interaction->mConstraint = EModuleInteractionConstraint::RANGE;
	interaction->setRange(min, max);

	return interaction;
}
