#include <fi3d/server/message_keys/EModuleRequest.h>

#include <QHash>
#include <QString>
#include <QSharedPointer>

using namespace fi3d;

QHash<int, QSharedPointer<EModuleRequest>> EModuleRequest::ENUM_TYPES = {
	{
		EModuleRequest::UNKNOWN,
		QSharedPointer<EModuleRequest>(new EModuleRequest(Q_NULLPTR))
	},
	{
		EModuleRequest::MODULE_INTERACTION, 
		QSharedPointer<EModuleRequest>(new EModuleRequest(Q_NULLPTR))
	},
	{
		EModuleRequest::SUBSCRIBE_TO_MODULE, 
		QSharedPointer<EModuleRequest>(new EModuleRequest(Q_NULLPTR))
	},
	{
		EModuleRequest::UNSUBSCRIBE_TO_MODULE,
		QSharedPointer<EModuleRequest>(new EModuleRequest(Q_NULLPTR))
	},
	{
		EModuleRequest::GET_SCENE, 
		QSharedPointer<EModuleRequest>(new EModuleRequest(Q_NULLPTR))
	},
	{
		EModuleRequest::GET_VISUAL,
		QSharedPointer<EModuleRequest>(new EModuleRequest(Q_NULLPTR))
	},
	{
		EModuleRequest::HIDE_VISUAL, 
		QSharedPointer<EModuleRequest>(new EModuleRequest(Q_NULLPTR))
	},
	{
		EModuleRequest::TRANSLATE_VISUAL,
		QSharedPointer<EModuleRequest>(new EModuleRequest(Q_NULLPTR))
	},
	{
		EModuleRequest::ROTATE_VISUAL,
		QSharedPointer<EModuleRequest>(new EModuleRequest(Q_NULLPTR))
	},
	{
		EModuleRequest::SELECT_SLICE,
		QSharedPointer<EModuleRequest>(new EModuleRequest(Q_NULLPTR))
	},
	{
		EModuleRequest::SELECT_ORIENTATION, 
		QSharedPointer<EModuleRequest>(new EModuleRequest(Q_NULLPTR))
	},
	{
		EModuleRequest::SELECT_SERIES,
		QSharedPointer<EModuleRequest>(new EModuleRequest(Q_NULLPTR))
	}
};

QHash<EModuleRequest*, int> EModuleRequest::ENUM_VALUES{
	{
		EModuleRequest::ENUM_TYPES.value(EModuleRequest::UNKNOWN).data(),
		EModuleRequest::UNKNOWN
	},
	{
		EModuleRequest::ENUM_TYPES.value(EModuleRequest::MODULE_INTERACTION).data(),
		EModuleRequest::MODULE_INTERACTION
	},
	{
		EModuleRequest::ENUM_TYPES.value(EModuleRequest::SUBSCRIBE_TO_MODULE).data(),
		EModuleRequest::SUBSCRIBE_TO_MODULE
	},
	{
		EModuleRequest::ENUM_TYPES.value(EModuleRequest::UNSUBSCRIBE_TO_MODULE).data(), 
		EModuleRequest::UNSUBSCRIBE_TO_MODULE
	},
	{
		EModuleRequest::ENUM_TYPES.value(EModuleRequest::GET_SCENE).data(), 
		EModuleRequest::GET_SCENE
	},
	{
		EModuleRequest::ENUM_TYPES.value(EModuleRequest::GET_VISUAL).data(),
		EModuleRequest::GET_VISUAL
	},
	{
		EModuleRequest::ENUM_TYPES.value(EModuleRequest::HIDE_VISUAL).data(),
		EModuleRequest::HIDE_VISUAL
	},
	{
		EModuleRequest::ENUM_TYPES.value(EModuleRequest::TRANSLATE_VISUAL).data(),
		EModuleRequest::TRANSLATE_VISUAL
	},
	{
		EModuleRequest::ENUM_TYPES.value(EModuleRequest::ROTATE_VISUAL).data(), 
		EModuleRequest::ROTATE_VISUAL
	},
	{
		EModuleRequest::ENUM_TYPES.value(EModuleRequest::SELECT_SLICE).data(),
		EModuleRequest::SELECT_SLICE
	},
	{
		EModuleRequest::ENUM_TYPES.value(EModuleRequest::SELECT_ORIENTATION).data(), 
		EModuleRequest::SELECT_ORIENTATION
	},
	{
		EModuleRequest::ENUM_TYPES.value(EModuleRequest::SELECT_SERIES).data(),
		EModuleRequest::SELECT_SERIES
	}
};

QHash<EModuleRequest*, QString> EModuleRequest::ENUM_NAMES{
	{
		EModuleRequest::ENUM_TYPES.value(EModuleRequest::UNKNOWN).data(), 
		"Unknown Application Action"
	},
	{
		EModuleRequest::ENUM_TYPES.value(EModuleRequest::MODULE_INTERACTION).data(),
		"Module Interaction"
	},
	{
		EModuleRequest::ENUM_TYPES.value(EModuleRequest::SUBSCRIBE_TO_MODULE).data(),
		"Subscribe to Module"
	},
	{
		EModuleRequest::ENUM_TYPES.value(EModuleRequest::UNSUBSCRIBE_TO_MODULE).data(),
		"Unsubscribe to Module"
	},
	{
		EModuleRequest::ENUM_TYPES.value(EModuleRequest::GET_SCENE).data(), 
		"Get Scene" 
	},
	{
		EModuleRequest::ENUM_TYPES.value(EModuleRequest::GET_VISUAL).data(),
		"Get Visual" 
	},
	{
		EModuleRequest::ENUM_TYPES.value(EModuleRequest::HIDE_VISUAL).data(), 
		"Hide Visual"
	},
	{
		EModuleRequest::ENUM_TYPES.value(EModuleRequest::TRANSLATE_VISUAL).data(),
		"Translate Visual"
	},
	{
		EModuleRequest::ENUM_TYPES.value(EModuleRequest::ROTATE_VISUAL).data(),
		"Rotate Visual"
	},
	{
		EModuleRequest::ENUM_TYPES.value(EModuleRequest::SELECT_SLICE).data(), 
		"Select Slice"
	},
	{
		EModuleRequest::ENUM_TYPES.value(EModuleRequest::SELECT_ORIENTATION).data(), 
		"Select Orientation"
	},
	{
		EModuleRequest::ENUM_TYPES.value(EModuleRequest::SELECT_SERIES).data(),
		"Select Series"
	}
};

EModuleRequest::EModuleRequest(EModuleRequest* type) {
	mValue = type;
}

QList<int> EModuleRequest::keys() {
	return ENUM_TYPES.keys();
}

EModuleRequest::EModuleRequest() : EModuleRequest(EModuleRequest::UNKNOWN) {}

EModuleRequest::EModuleRequest(const EModuleRequest& mtype) {
	mValue = mtype.mValue;
}

EModuleRequest::EModuleRequest(const int& valueType) {
	if (ENUM_TYPES.contains(valueType))
		mValue = ENUM_TYPES.value(valueType).data();
	else
		mValue = ENUM_TYPES.value(EModuleRequest::UNKNOWN).data();
}

EModuleRequest::~EModuleRequest() {}

EModuleRequest& EModuleRequest::operator=(const int& valueType) {
	if (ENUM_TYPES.contains(valueType))
		mValue = ENUM_TYPES.value(valueType).data();
	else
		mValue = ENUM_TYPES.value(EModuleRequest::UNKNOWN).data();
	return *this;
}

EModuleRequest& EModuleRequest::operator=(const EModuleRequest& other) {
	if (this != &other) {
		mValue = other.mValue;
	}
	return *this;
}

bool EModuleRequest::operator==(const int& valueType) const {
	if (this->toInt() == valueType) {
		return true;
	} else {
		return false;
	}
}

bool EModuleRequest::operator==(const EModuleRequest& other) const {
	if (mValue == other.mValue) {
		return true;
	}
	return false;
}

bool EModuleRequest::operator!=(const int& valueType) const {
	return !(*this == valueType);
}

bool EModuleRequest::operator!=(const EModuleRequest& other) const {
	return !(*this == other);
}

QString EModuleRequest::getName() const {
	return ENUM_NAMES.value(mValue);
}

int EModuleRequest::toInt() const {
	return ENUM_VALUES.value(mValue);
}
