#include <fi3d/server/message_keys/EMessage.h>

#include <QHash>
#include <QString>
#include <QSharedPointer>

using namespace fi3d;

QHash<int, QSharedPointer<EMessage>> EMessage::ENUM_TYPES{
	{
		EMessage::UNKNOWN,
		QSharedPointer<EMessage>(new EMessage(Q_NULLPTR))
	},
	{
		EMessage::AUTHENTICATION,
		QSharedPointer<EMessage>(new EMessage(Q_NULLPTR))
	},
	{
		EMessage::APPLICATION,
		QSharedPointer<EMessage>(new EMessage(Q_NULLPTR))
	},
	{
		EMessage::MODULE,
		QSharedPointer<EMessage>(new EMessage(Q_NULLPTR))
	},
	{
		EMessage::DATA, 
		QSharedPointer<EMessage>(new EMessage(Q_NULLPTR))
	}
};

QHash<EMessage*, int> EMessage::ENUM_VALUES{
	{
		EMessage::ENUM_TYPES.value(EMessage::UNKNOWN).data(),
		EMessage::UNKNOWN
	},
	{
		EMessage::ENUM_TYPES.value(EMessage::AUTHENTICATION).data(), 
		EMessage::AUTHENTICATION
	},
	{
		EMessage::ENUM_TYPES.value(EMessage::APPLICATION).data(), 
		EMessage::APPLICATION
	},
	{
		EMessage::ENUM_TYPES.value(EMessage::MODULE).data(), 
		EMessage::MODULE
	},
	{
		EMessage::ENUM_TYPES.value(EMessage::DATA).data(),
		EMessage::DATA 
	}
};

QHash<EMessage*, QString> EMessage::ENUM_NAMES{
	{
		EMessage::ENUM_TYPES.value(EMessage::UNKNOWN).data(),
		"Unknown Message Type"
	},
	{
		EMessage::ENUM_TYPES.value(EMessage::AUTHENTICATION).data(),
		"Authentication"
	},
	{
		EMessage::ENUM_TYPES.value(EMessage::APPLICATION).data(),
		"Application"
	},
	{
		EMessage::ENUM_TYPES.value(EMessage::MODULE).data(), 
		"Module"
	},
	{
		EMessage::ENUM_TYPES.value(EMessage::DATA).data(), 
		"Data"
	}
};

EMessage::EMessage(EMessage* type) {
	mValue = type;
}

QList<int> EMessage::keys() {
	return ENUM_TYPES.keys();
}

EMessage::EMessage() : EMessage(EMessage::UNKNOWN) {}

EMessage::EMessage(const EMessage& mtype) {
	mValue = mtype.mValue;
}

EMessage::EMessage(const int& valueType) {
	if (ENUM_TYPES.contains(valueType))
		mValue = ENUM_TYPES.value(valueType).data();
	else
		mValue = ENUM_TYPES.value(EMessage::UNKNOWN).data();
}

EMessage::~EMessage() {}

EMessage& EMessage::operator=(const int& valueType) {
	if (ENUM_TYPES.contains(valueType))
		mValue = ENUM_TYPES.value(valueType).data();
	else
		mValue = ENUM_TYPES.value(EMessage::UNKNOWN).data();
	return *this;
}

EMessage& EMessage::operator=(const EMessage& other) {
	if (this != &other) {
		mValue = other.mValue;
	}
	return *this;
}

bool EMessage::operator==(const int& valueType) const {
	if (this->toInt() == valueType) {
		return true;
	} else {
		return false;
	}
}

bool EMessage::operator==(const EMessage& other) const {
	if (mValue == other.mValue) {
		return true;
	}
	return false;
}

bool EMessage::operator!=(const int& valueType) const {
	return !(*this == valueType);
}

bool EMessage::operator!=(const EMessage& other) const {
	return !(*this == other);
}

QString EMessage::getName() const {
	return ENUM_NAMES.value(mValue);
}

int EMessage::toInt() const {
	return ENUM_VALUES.value(mValue);
}
