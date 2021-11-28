#include <fi3d/server/message_keys/EResponseStatus.h>

#include <QHash>
#include <QString>
#include <QSharedPointer>

using namespace fi3d;

QHash<int, QSharedPointer<EResponseStatus>> EResponseStatus::ENUM_TYPES{
	{
		EResponseStatus::UNKNOWN,
		QSharedPointer<EResponseStatus>(new EResponseStatus(Q_NULLPTR))
	},
	{
		EResponseStatus::SUCCESS,
		QSharedPointer<EResponseStatus>(new EResponseStatus(Q_NULLPTR))
	},
	{
		EResponseStatus::ERROR_RESPONSE,
		QSharedPointer<EResponseStatus>(new EResponseStatus(Q_NULLPTR))
	},
	{
		EResponseStatus::INFO_REQUIRED,
		QSharedPointer<EResponseStatus>(new EResponseStatus(Q_NULLPTR))
	}
};

QHash<EResponseStatus*, int> EResponseStatus::ENUM_VALUES{
	{
		EResponseStatus::ENUM_TYPES.value(EResponseStatus::UNKNOWN).data(),
		EResponseStatus::UNKNOWN
	},
	{
		EResponseStatus::ENUM_TYPES.value(EResponseStatus::SUCCESS).data(),
		EResponseStatus::SUCCESS
	},
	{
		EResponseStatus::ENUM_TYPES.value(EResponseStatus::ERROR_RESPONSE).data(),
		EResponseStatus::ERROR_RESPONSE
	},
	{
		EResponseStatus::ENUM_TYPES.value(EResponseStatus::INFO_REQUIRED).data(), 
		EResponseStatus::INFO_REQUIRED
	}
};

QHash<EResponseStatus*, QString> EResponseStatus::ENUM_NAMES{
	{
		EResponseStatus::ENUM_TYPES.value(EResponseStatus::UNKNOWN).data(), 
		"Unknown Response Status"
	},
	{
		EResponseStatus::ENUM_TYPES.value(EResponseStatus::SUCCESS).data(),
		"Success"
	},
	{
		EResponseStatus::ENUM_TYPES.value(EResponseStatus::ERROR_RESPONSE).data(), 
		"Error"
	},
	{
		EResponseStatus::ENUM_TYPES.value(EResponseStatus::INFO_REQUIRED).data(), 
		"Info Required"
	}
};

EResponseStatus::EResponseStatus(EResponseStatus* type) {
	mValue = type;
}

QList<int> EResponseStatus::keys() {
	return ENUM_TYPES.keys();
}

EResponseStatus::EResponseStatus() : EResponseStatus(EResponseStatus::UNKNOWN) {}

EResponseStatus::EResponseStatus(const EResponseStatus& mtype) {
	mValue = mtype.mValue;
}

EResponseStatus::EResponseStatus(const int& valueType) {
	if (ENUM_TYPES.contains(valueType))
		mValue = ENUM_TYPES.value(valueType).data();
	else
		mValue = ENUM_TYPES.value(EResponseStatus::UNKNOWN).data();
}

EResponseStatus::~EResponseStatus() {}

EResponseStatus& EResponseStatus::operator=(const int& valueType) {
	if (ENUM_TYPES.contains(valueType))
		mValue = ENUM_TYPES.value(valueType).data();
	else
		mValue = ENUM_TYPES.value(EResponseStatus::UNKNOWN).data();
	return *this;
}

EResponseStatus& EResponseStatus::operator=(const EResponseStatus& other) {
	if (this != &other) {
		mValue = other.mValue;
	}
	return *this;
}

bool EResponseStatus::operator==(const int& valueType) const {
	if (this->toInt() == valueType) {
		return true;
	} else {
		return false;
	}
}

bool EResponseStatus::operator==(const EResponseStatus& other) const {
	if (mValue == other.mValue) {
		return true;
	}
	return false;
}

bool EResponseStatus::operator!=(const int& valueType) const {
	return !(*this == valueType);
}

bool EResponseStatus::operator!=(const EResponseStatus& other) const {
	return !(*this == other);
}

QString EResponseStatus::getName() const {
	return ENUM_NAMES.value(mValue);
}

int EResponseStatus::toInt() const {
	return ENUM_VALUES.value(mValue);
}
