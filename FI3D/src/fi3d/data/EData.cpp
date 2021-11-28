#include <fi3d/data/EData.h>

#include <QHash>
#include <QString>
#include <QSharedPointer>

using namespace fi3d;

QHash<int, QSharedPointer<EData>> EData::ENUM_TYPES{
	{
		EData::UNKNOWN, QSharedPointer<EData>(new EData(Q_NULLPTR))
	},
	{
		EData::IMAGE, QSharedPointer<EData>(new EData(Q_NULLPTR))
	},
	{
		EData::SERIES, QSharedPointer<EData>(new EData(Q_NULLPTR))
	},
	{
		EData::STUDY, QSharedPointer<EData>(new EData(Q_NULLPTR))
	},
	{
		EData::MODEL, QSharedPointer<EData>(new EData(Q_NULLPTR))
	},
	{
		EData::ANIMATED_MODEL, QSharedPointer<EData>(new EData(Q_NULLPTR))
	},
};

QHash<EData*, int> EData::ENUM_VALUES{
	{
		EData::ENUM_TYPES.value(EData::UNKNOWN).data(),
		EData::UNKNOWN
	},
	{
		EData::ENUM_TYPES.value(EData::IMAGE).data(), 
		EData::IMAGE
	},
	{
		EData::ENUM_TYPES.value(EData::SERIES).data(), 
		EData::SERIES
	},
	{
		EData::ENUM_TYPES.value(EData::STUDY).data(), 
		EData::STUDY
	},
	{
		EData::ENUM_TYPES.value(EData::MODEL).data(), 
		EData::MODEL
	},
	{
		EData::ENUM_TYPES.value(EData::ANIMATED_MODEL).data(), 
		EData::ANIMATED_MODEL
	},
};

QHash<EData*, QString> EData::ENUM_NAMES{
	{
		EData::ENUM_TYPES.value(EData::UNKNOWN).data(), 
		"Unknown Data Type"
	},
	{
		EData::ENUM_TYPES.value(EData::IMAGE).data(), 
		"Image"
	},
	{
		EData::ENUM_TYPES.value(EData::SERIES).data(),
		"Series"
	},
	{
		EData::ENUM_TYPES.value(EData::STUDY).data(), 
		"Study"
	},
	{
		EData::ENUM_TYPES.value(EData::MODEL).data(),
		"Model"
	},
	{
		EData::ENUM_TYPES.value(EData::ANIMATED_MODEL).data(),
		"Animated Model"
	},
};

EData::EData(EData* type) {
	mValue = type;
}

QList<int> EData::keys() {
	return ENUM_TYPES.keys();
}

EData::EData() : EData(EData::UNKNOWN) {}

EData::EData(const EData& mtype) {
	mValue = mtype.mValue;
}

EData::EData(const int& valueType) {
	if (ENUM_TYPES.contains(valueType))
		mValue = ENUM_TYPES.value(valueType).data();
	else
		mValue = ENUM_TYPES.value(EData::UNKNOWN).data();
}

EData::~EData() {}

EData& EData::operator=(const int& valueType) {
	if (ENUM_TYPES.contains(valueType))
		mValue = ENUM_TYPES.value(valueType).data();
	else
		mValue = ENUM_TYPES.value(EData::UNKNOWN).data();
	return *this;
}

EData& EData::operator=(const EData& other) {
	if (this != &other) {
		mValue = other.mValue;
	}
	return *this;
}

bool EData::operator==(const int& valueType) const {
	if (this->toInt() == valueType) {
		return true;
	} else {
		return false;
	}
}

bool EData::operator==(const EData& other) const {
	if (mValue == other.mValue) {
		return true;
	}
	return false;
}

bool EData::operator!=(const int& valueType) const {
	return !(*this == valueType);
}

bool EData::operator!=(const EData& other) const {
	return !(*this == other);
}

QString EData::getName() const {
	return ENUM_NAMES.value(mValue);
}

int EData::toInt() const {
	return ENUM_VALUES.value(mValue);
}
