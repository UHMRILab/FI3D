#include <fi3d/server/message_keys/EApplicationRequest.h>

#include <QHash>
#include <QString>
#include <QSharedPointer>

using namespace fi3d;

QHash<int, QSharedPointer<EApplicationRequest>> EApplicationRequest::ENUM_TYPES {
	{
		EApplicationRequest::UNKNOWN,
		QSharedPointer<EApplicationRequest>(new EApplicationRequest(Q_NULLPTR))
	},
	{
		EApplicationRequest::START_MODULE,
		QSharedPointer<EApplicationRequest>(new EApplicationRequest(Q_NULLPTR))
	},
	{
		EApplicationRequest::STOP_MODULE, 
		QSharedPointer<EApplicationRequest>(new EApplicationRequest(Q_NULLPTR))
	}
	,
	{
		EApplicationRequest::LOG,
		QSharedPointer<EApplicationRequest>(new EApplicationRequest(Q_NULLPTR))
	}
};

QHash<EApplicationRequest*, int> EApplicationRequest::ENUM_VALUES {
	{
		EApplicationRequest::ENUM_TYPES.value(EApplicationRequest::UNKNOWN).data(),
		EApplicationRequest::UNKNOWN
	},
	{
		EApplicationRequest::ENUM_TYPES.value(EApplicationRequest::START_MODULE).data(),
		EApplicationRequest::START_MODULE
	},
	{
		EApplicationRequest::ENUM_TYPES.value(EApplicationRequest::STOP_MODULE).data(), 
		EApplicationRequest::STOP_MODULE
	}
	,
	{
		EApplicationRequest::ENUM_TYPES.value(EApplicationRequest::LOG).data(),
		EApplicationRequest::LOG
	}
};

QHash<EApplicationRequest*, QString> EApplicationRequest::ENUM_NAMES {
	{
		EApplicationRequest::ENUM_TYPES.value(EApplicationRequest::UNKNOWN).data(),
		"Unknown Application Action"
	},
	{
		EApplicationRequest::ENUM_TYPES.value(EApplicationRequest::START_MODULE).data(),
		"Activate Module"
	},
	{
		EApplicationRequest::ENUM_TYPES.value(EApplicationRequest::STOP_MODULE).data(),
		"Stop Module"
	}
	,
	{
		EApplicationRequest::ENUM_TYPES.value(EApplicationRequest::LOG).data(),
		"Log"
	}
};

EApplicationRequest::EApplicationRequest(EApplicationRequest* type) {
	mValue = type;
}

QList<int> EApplicationRequest::keys() {
	return ENUM_TYPES.keys();
}

EApplicationRequest::EApplicationRequest() 
	: EApplicationRequest(EApplicationRequest::UNKNOWN) {}

EApplicationRequest::EApplicationRequest(const EApplicationRequest& mtype) {
	mValue = mtype.mValue;
}

EApplicationRequest::EApplicationRequest(const int& valueType) {
	if (ENUM_TYPES.contains(valueType))
		mValue = ENUM_TYPES.value(valueType).data();
	else
		mValue = ENUM_TYPES.value(EApplicationRequest::UNKNOWN).data();
}

EApplicationRequest::~EApplicationRequest() {}

EApplicationRequest& EApplicationRequest::operator=(const int& valueType) {
	if (ENUM_TYPES.contains(valueType))
		mValue = ENUM_TYPES.value(valueType).data();
	else
		mValue = ENUM_TYPES.value(EApplicationRequest::UNKNOWN).data();
	return *this;
}

EApplicationRequest& EApplicationRequest::operator=(
	const EApplicationRequest& other) 
{
	if (this != &other) {
		mValue = other.mValue;
	}
	return *this;
}

bool EApplicationRequest::operator==(const int& valueType) const {
	if (this->toInt() == valueType) {
		return true;
	} else {
		return false;
	}
}

bool EApplicationRequest::operator==(const EApplicationRequest& other) const {
	if (mValue == other.mValue) {
		return true;
	}
	return false;
}

bool EApplicationRequest::operator!=(const int& valueType) const {
	return !(*this == valueType);
}

bool EApplicationRequest::operator!=(const EApplicationRequest& other) const {
	return !(*this == other);
}

QString EApplicationRequest::getName() const {
	return ENUM_NAMES.value(mValue);
}

int EApplicationRequest::toInt() const {
	return ENUM_VALUES.value(mValue);
}
