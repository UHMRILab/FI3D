#include <fi3d/server/message_keys/EModuleInteractionConstraint.h>

#include <QHash>
#include <QString>
#include <QSharedPointer>

using namespace fi3d;

QHash<int, QSharedPointer<EModuleInteractionConstraint>> EModuleInteractionConstraint::ENUM_TYPES{
	{
		EModuleInteractionConstraint::UNKNOWN,
		QSharedPointer<EModuleInteractionConstraint>(new EModuleInteractionConstraint(Q_NULLPTR))
	},
	{
		EModuleInteractionConstraint::MIN,
		QSharedPointer<EModuleInteractionConstraint>(new EModuleInteractionConstraint(Q_NULLPTR))
	},
	{
		EModuleInteractionConstraint::MAX,
		QSharedPointer<EModuleInteractionConstraint>(new EModuleInteractionConstraint(Q_NULLPTR))
	},
	{
		EModuleInteractionConstraint::RANGE,
		QSharedPointer<EModuleInteractionConstraint>(new EModuleInteractionConstraint(Q_NULLPTR))
	}
};

QHash<EModuleInteractionConstraint*, int> EModuleInteractionConstraint::ENUM_VALUES{
	{
		EModuleInteractionConstraint::ENUM_TYPES.value(EModuleInteractionConstraint::UNKNOWN).data(),
		EModuleInteractionConstraint::UNKNOWN
	},
	{
		EModuleInteractionConstraint::ENUM_TYPES.value(EModuleInteractionConstraint::MIN).data(),
		EModuleInteractionConstraint::MIN
	},
	{
		EModuleInteractionConstraint::ENUM_TYPES.value(EModuleInteractionConstraint::MAX).data(),
		EModuleInteractionConstraint::MAX
	},
	{
		EModuleInteractionConstraint::ENUM_TYPES.value(EModuleInteractionConstraint::RANGE).data(),
		EModuleInteractionConstraint::RANGE
	}
};

QHash<EModuleInteractionConstraint*, QString> EModuleInteractionConstraint::ENUM_NAMES{
	{
		EModuleInteractionConstraint::ENUM_TYPES.value(EModuleInteractionConstraint::UNKNOWN).data(),
		"Unknown Module Interaction Constraint"
	},
	{
		EModuleInteractionConstraint::ENUM_TYPES.value(EModuleInteractionConstraint::MIN).data(),
		"Minimum"
	},
	{
		EModuleInteractionConstraint::ENUM_TYPES.value(EModuleInteractionConstraint::MAX).data(),
		"Maximum"
	},
	{
		EModuleInteractionConstraint::ENUM_TYPES.value(EModuleInteractionConstraint::RANGE).data(), 
		"Range"
	}
};

EModuleInteractionConstraint::EModuleInteractionConstraint(EModuleInteractionConstraint* type) {
	mValue = type;
}

QList<int> EModuleInteractionConstraint::keys() {
	return ENUM_TYPES.keys();
}

EModuleInteractionConstraint::EModuleInteractionConstraint()
	: EModuleInteractionConstraint(EModuleInteractionConstraint::UNKNOWN) 
{}

EModuleInteractionConstraint::EModuleInteractionConstraint(const EModuleInteractionConstraint& mtype) {
	mValue = mtype.mValue;
}

EModuleInteractionConstraint::EModuleInteractionConstraint(const int& valueType) {
	if (ENUM_TYPES.contains(valueType))
		mValue = ENUM_TYPES.value(valueType).data();
	else
		mValue = ENUM_TYPES.value(EModuleInteractionConstraint::UNKNOWN).data();
}

EModuleInteractionConstraint::~EModuleInteractionConstraint() {}

EModuleInteractionConstraint& EModuleInteractionConstraint::operator=(const int& valueType) {
	if (ENUM_TYPES.contains(valueType))
		mValue = ENUM_TYPES.value(valueType).data();
	else
		mValue = ENUM_TYPES.value(EModuleInteractionConstraint::UNKNOWN).data();
	return *this;
}

EModuleInteractionConstraint& EModuleInteractionConstraint::operator=(const EModuleInteractionConstraint& other) {
	if (this != &other) {
		mValue = other.mValue;
	}
	return *this;
}

bool EModuleInteractionConstraint::operator==(const int& valueType) const {
	if (this->toInt() == valueType) {
		return true;
	} else {
		return false;
	}
}

bool EModuleInteractionConstraint::operator==(const EModuleInteractionConstraint& other) const {
	if (mValue == other.mValue) {
		return true;
	}
	return false;
}

bool EModuleInteractionConstraint::operator!=(const int& valueType) const {
	return !(*this == valueType);
}

bool EModuleInteractionConstraint::operator!=(
	const EModuleInteractionConstraint& other) const 
{
	return !(*this == other);
}

QString EModuleInteractionConstraint::getName() const {
	return ENUM_NAMES.value(mValue);
}

int EModuleInteractionConstraint::toInt() const {
	return ENUM_VALUES.value(mValue);
}
