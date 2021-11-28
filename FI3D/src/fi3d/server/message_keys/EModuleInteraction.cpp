#include <fi3d/server/message_keys/EModuleInteraction.h>

#include <QHash>
#include <QString>
#include <QSharedPointer>

using namespace fi3d;

QHash<int, QSharedPointer<EModuleInteraction>> EModuleInteraction::ENUM_TYPES{
	{
		EModuleInteraction::UNKNOWN,
		QSharedPointer<EModuleInteraction>(new EModuleInteraction(Q_NULLPTR))
	},
	{
		EModuleInteraction::VALUELESS,
		QSharedPointer<EModuleInteraction>(new EModuleInteraction(Q_NULLPTR))
	},
	{
		EModuleInteraction::BOOL,
		QSharedPointer<EModuleInteraction>(new EModuleInteraction(Q_NULLPTR))
	},
	{
		EModuleInteraction::INTEGER,
		QSharedPointer<EModuleInteraction>(new EModuleInteraction(Q_NULLPTR))
	},
	{
		EModuleInteraction::FLOAT,
		QSharedPointer<EModuleInteraction>(new EModuleInteraction(Q_NULLPTR))
	},
	{
		EModuleInteraction::STRING, 
		QSharedPointer<EModuleInteraction>(new EModuleInteraction(Q_NULLPTR))
	},
	{
		EModuleInteraction::SELECT,
		QSharedPointer<EModuleInteraction>(new EModuleInteraction(Q_NULLPTR))
	},
	{
		EModuleInteraction::POINT_2D, 
		QSharedPointer<EModuleInteraction>(new EModuleInteraction(Q_NULLPTR))
	},
	{
		EModuleInteraction::POINT_3D, 
		QSharedPointer<EModuleInteraction>(new EModuleInteraction(Q_NULLPTR))
	}
};

QHash<EModuleInteraction*, int> EModuleInteraction::ENUM_VALUES{
	{
		EModuleInteraction::ENUM_TYPES.value(EModuleInteraction::UNKNOWN).data(),
		EModuleInteraction::UNKNOWN
	},
	{
		EModuleInteraction::ENUM_TYPES.value(EModuleInteraction::VALUELESS).data(), 
		EModuleInteraction::VALUELESS
	},
	{
		EModuleInteraction::ENUM_TYPES.value(EModuleInteraction::BOOL).data(), 
		EModuleInteraction::BOOL
	},
	{
		EModuleInteraction::ENUM_TYPES.value(EModuleInteraction::INTEGER).data(), 
		EModuleInteraction::INTEGER
	},
	{
		EModuleInteraction::ENUM_TYPES.value(EModuleInteraction::FLOAT).data(),
		EModuleInteraction::FLOAT
	},
	{
		EModuleInteraction::ENUM_TYPES.value(EModuleInteraction::STRING).data(), 
		EModuleInteraction::STRING
	},
	{
		EModuleInteraction::ENUM_TYPES.value(EModuleInteraction::SELECT).data(),
		EModuleInteraction::SELECT
	},
	{
		EModuleInteraction::ENUM_TYPES.value(EModuleInteraction::POINT_2D).data(), 
		EModuleInteraction::POINT_2D
	},
	{
		EModuleInteraction::ENUM_TYPES.value(EModuleInteraction::POINT_3D).data(), 
		EModuleInteraction::POINT_3D
	}
};

QHash<EModuleInteraction*, QString> EModuleInteraction::ENUM_NAMES{
	{
		EModuleInteraction::ENUM_TYPES.value(EModuleInteraction::UNKNOWN).data(),
		"Unknown Module Interaction"
	},
	{
		EModuleInteraction::ENUM_TYPES.value(EModuleInteraction::VALUELESS).data(), 
		"Valueless"
	},
	{
		EModuleInteraction::ENUM_TYPES.value(EModuleInteraction::BOOL).data(),
		"Boolean"
	},
	{
		EModuleInteraction::ENUM_TYPES.value(EModuleInteraction::INTEGER).data(), 
		"Integer"
	},
	{
		EModuleInteraction::ENUM_TYPES.value(EModuleInteraction::FLOAT).data(),
		"Float"
	},
	{
		EModuleInteraction::ENUM_TYPES.value(EModuleInteraction::STRING).data(), 
		"String"
	},
	{
		EModuleInteraction::ENUM_TYPES.value(EModuleInteraction::SELECT).data(),
		"Select"
	},
	{
		EModuleInteraction::ENUM_TYPES.value(EModuleInteraction::POINT_2D).data(), 
		"2D Point"
	},
	{
		EModuleInteraction::ENUM_TYPES.value(EModuleInteraction::POINT_3D).data(), 
		"3D Point"
	}
};

EModuleInteraction::EModuleInteraction(EModuleInteraction* type) {
	mValue = type;
}

QList<int> EModuleInteraction::keys() {
	return ENUM_TYPES.keys();
}

EModuleInteraction::EModuleInteraction() 
	: EModuleInteraction(EModuleInteraction::UNKNOWN) {}

EModuleInteraction::EModuleInteraction(const EModuleInteraction& mtype) {
	mValue = mtype.mValue;
}

EModuleInteraction::EModuleInteraction(const int& valueType) {
	if (ENUM_TYPES.contains(valueType))
		mValue = ENUM_TYPES.value(valueType).data();
	else
		mValue = ENUM_TYPES.value(EModuleInteraction::UNKNOWN).data();
}

EModuleInteraction::~EModuleInteraction() {}

EModuleInteraction& EModuleInteraction::operator=(const int& valueType) {
	if (ENUM_TYPES.contains(valueType))
		mValue = ENUM_TYPES.value(valueType).data();
	else
		mValue = ENUM_TYPES.value(EModuleInteraction::UNKNOWN).data();
	return *this;
}

EModuleInteraction& EModuleInteraction::operator=(
	const EModuleInteraction& other) 
{
	if (this != &other) {
		mValue = other.mValue;
	}
	return *this;
}

bool EModuleInteraction::operator==(const int& valueType) const {
	if (this->toInt() == valueType) {
		return true;
	} else {
		return false;
	}
}

bool EModuleInteraction::operator==(const EModuleInteraction& other) const {
	if (mValue == other.mValue) {
		return true;
	}
	return false;
}

bool EModuleInteraction::operator!=(const int& valueType) const {
	return !(*this == valueType);
}

bool EModuleInteraction::operator!=(const EModuleInteraction& other) const {
	return !(*this == other);
}

QString EModuleInteraction::getName() const {
	return ENUM_NAMES.value(mValue);
}

int EModuleInteraction::toInt() const {
	return ENUM_VALUES.value(mValue);
}
