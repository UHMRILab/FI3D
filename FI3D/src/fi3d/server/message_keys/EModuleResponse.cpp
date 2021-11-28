#include <fi3d/server/message_keys/EModuleResponse.h>

using namespace fi3d;

QHash<int, QSharedPointer<EModuleResponse>> EModuleResponse::ENUM_TYPES{
	{
		EModuleResponse::UNKNOWN,
		QSharedPointer<EModuleResponse>(new EModuleResponse(Q_NULLPTR))
	},
	{
		EModuleResponse::ADD_MODULE_INTERACTION,
		QSharedPointer<EModuleResponse>(new EModuleResponse(Q_NULLPTR))
	},
	{
		EModuleResponse::UPDATE_MODULE_INTERACTION,
		QSharedPointer<EModuleResponse>(new EModuleResponse(Q_NULLPTR))
	},
	{
		EModuleResponse::REMOVE_MODULE_INTERACTION,
		QSharedPointer<EModuleResponse>(new EModuleResponse(Q_NULLPTR))
	},
	{
		EModuleResponse::ADD_VISUAL,
		QSharedPointer<EModuleResponse>(new EModuleResponse(Q_NULLPTR))
	},
	{
		EModuleResponse::REFRESH_VISUAL,
		QSharedPointer<EModuleResponse>(new EModuleResponse(Q_NULLPTR))
	},
	{
		EModuleResponse::REMOVE_VISUAL,
		QSharedPointer<EModuleResponse>(new EModuleResponse(Q_NULLPTR))
	},
	{
		EModuleResponse::DATA_CHANGE,
		QSharedPointer<EModuleResponse>(new EModuleResponse(Q_NULLPTR))
	},
	{
		EModuleResponse::HIDE_VISUAL,
		QSharedPointer<EModuleResponse>(new EModuleResponse(Q_NULLPTR))
	},
	{
		EModuleResponse::TRANSFORM_VISUAL,
		QSharedPointer<EModuleResponse>(new EModuleResponse(Q_NULLPTR))
	},
	{
		EModuleResponse::PARENT_CHANGE,
		QSharedPointer<EModuleResponse>(new EModuleResponse(Q_NULLPTR))
	},
	{
		EModuleResponse::SET_VISUAL_OPACITY,
		QSharedPointer<EModuleResponse>(new EModuleResponse(Q_NULLPTR))
	},
	{
		EModuleResponse::SET_SLICE,
		QSharedPointer<EModuleResponse>(new EModuleResponse(Q_NULLPTR))
	},
	{
		EModuleResponse::SET_OBJECT_COLOR,
		QSharedPointer<EModuleResponse>(new EModuleResponse(Q_NULLPTR))
	},
};

QHash<EModuleResponse*, int> EModuleResponse::ENUM_VALUES{
	{
		EModuleResponse::ENUM_TYPES.value(EModuleResponse::UNKNOWN).data(),
		EModuleResponse::UNKNOWN
	},
	{
		EModuleResponse::ENUM_TYPES.value(EModuleResponse::ADD_MODULE_INTERACTION).data(),
		EModuleResponse::ADD_MODULE_INTERACTION
	},
	{
		EModuleResponse::ENUM_TYPES.value(EModuleResponse::UPDATE_MODULE_INTERACTION).data(),
		EModuleResponse::UPDATE_MODULE_INTERACTION
	},
	{
		EModuleResponse::ENUM_TYPES.value(EModuleResponse::REMOVE_MODULE_INTERACTION).data(),
		EModuleResponse::REMOVE_MODULE_INTERACTION
	},
	{
		EModuleResponse::ENUM_TYPES.value(EModuleResponse::ADD_VISUAL).data(), 
		EModuleResponse::ADD_VISUAL
	},
	{
		EModuleResponse::ENUM_TYPES.value(EModuleResponse::REFRESH_VISUAL).data(), 
		EModuleResponse::REFRESH_VISUAL
	},
	{
		EModuleResponse::ENUM_TYPES.value(EModuleResponse::REMOVE_VISUAL).data(), 
		EModuleResponse::REMOVE_VISUAL
	},
	{
		EModuleResponse::ENUM_TYPES.value(EModuleResponse::DATA_CHANGE).data(),
		EModuleResponse::DATA_CHANGE
	},
	{
		EModuleResponse::ENUM_TYPES.value(EModuleResponse::HIDE_VISUAL).data(),
		EModuleResponse::HIDE_VISUAL
	},
	{
		EModuleResponse::ENUM_TYPES.value(EModuleResponse::TRANSFORM_VISUAL).data(), 
		EModuleResponse::TRANSFORM_VISUAL
	},
	{
		EModuleResponse::ENUM_TYPES.value(EModuleResponse::PARENT_CHANGE).data(),
		EModuleResponse::PARENT_CHANGE
	},
	{
		EModuleResponse::ENUM_TYPES.value(EModuleResponse::SET_VISUAL_OPACITY).data(),
		EModuleResponse::SET_VISUAL_OPACITY
	},
	{
		EModuleResponse::ENUM_TYPES.value(EModuleResponse::SET_SLICE).data(), 
		EModuleResponse::SET_SLICE
	},
	{
		EModuleResponse::ENUM_TYPES.value(EModuleResponse::SET_OBJECT_COLOR).data(),
		EModuleResponse::SET_OBJECT_COLOR
	},
};

QHash<EModuleResponse*, QString> EModuleResponse::ENUM_NAMES{
	{
		EModuleResponse::ENUM_TYPES.value(EModuleResponse::UNKNOWN).data(), 
		"Unknown Object Action"
	},
	{
		EModuleResponse::ENUM_TYPES.value(EModuleResponse::ADD_MODULE_INTERACTION).data(), 
		"Add Module Interaction"
	},
	{
		EModuleResponse::ENUM_TYPES.value(EModuleResponse::UPDATE_MODULE_INTERACTION).data(),
		"Update Module Interaction"
	},
	{
		EModuleResponse::ENUM_TYPES.value(EModuleResponse::REMOVE_MODULE_INTERACTION).data(),
		"Remove Module Interaction"
	},
	{
		EModuleResponse::ENUM_TYPES.value(EModuleResponse::ADD_VISUAL).data(), 
		"Add Visual" 
	},
	{ 
		EModuleResponse::ENUM_TYPES.value(EModuleResponse::REFRESH_VISUAL).data(),
		"Refresh Visual" 
	},
	{
		EModuleResponse::ENUM_TYPES.value(EModuleResponse::REMOVE_VISUAL).data(), 
		"Remove Visual" 
	},
	{
		EModuleResponse::ENUM_TYPES.value(EModuleResponse::DATA_CHANGE).data(), 
		"Data Change" 
	},
	{
		EModuleResponse::ENUM_TYPES.value(EModuleResponse::HIDE_VISUAL).data(),
		"Hide Visual" 
	},
	{
		EModuleResponse::ENUM_TYPES.value(EModuleResponse::TRANSFORM_VISUAL).data(), 
		"Transform Visual"
	},
	{
		EModuleResponse::ENUM_TYPES.value(EModuleResponse::PARENT_CHANGE).data(), 
		"Parent Change"
	},
	{
		EModuleResponse::ENUM_TYPES.value(EModuleResponse::SET_VISUAL_OPACITY).data(), 
		"Set Visual Opacity"
	},
	{
		EModuleResponse::ENUM_TYPES.value(EModuleResponse::SET_SLICE).data(),
		"Set Slice"
	},
	{
		EModuleResponse::ENUM_TYPES.value(EModuleResponse::SET_OBJECT_COLOR).data(),
		"Set Object Color"
	},
};

EModuleResponse::EModuleResponse(EModuleResponse* type) {
	mValue = type;
}

QList<int> EModuleResponse::keys() {
	return ENUM_TYPES.keys();
}

EModuleResponse::EModuleResponse() : EModuleResponse(EModuleResponse::UNKNOWN) {}

EModuleResponse::EModuleResponse(const EModuleResponse& mtype) {
	mValue = mtype.mValue;
}

EModuleResponse::EModuleResponse(const int& valueType) {
	if (ENUM_TYPES.contains(valueType))
		mValue = ENUM_TYPES.value(valueType).data();
	else
		mValue = ENUM_TYPES.value(EModuleResponse::UNKNOWN).data();
}

EModuleResponse::~EModuleResponse() {}

EModuleResponse& EModuleResponse::operator=(const int& valueType) {
	if (ENUM_TYPES.contains(valueType))
		mValue = ENUM_TYPES.value(valueType).data();
	else
		mValue = ENUM_TYPES.value(EModuleResponse::UNKNOWN).data();
	return *this;
}

EModuleResponse& EModuleResponse::operator=(const EModuleResponse& other) {
	if (this != &other) {
		mValue = other.mValue;
	}
	return *this;
}

bool EModuleResponse::operator==(const int& valueType) const {
	if (this->toInt() == valueType) {
		return true;
	} else {
		return false;
	}
}

bool EModuleResponse::operator==(const EModuleResponse& other) const {
	if (mValue == other.mValue) {
		return true;
	}
	return false;
}

bool EModuleResponse::operator!=(const int& valueType) const {
	return !(*this == valueType);
}

bool EModuleResponse::operator!=(const EModuleResponse& other) const {
	return !(*this == other);
}

QString EModuleResponse::getName() const {
	return ENUM_NAMES.value(mValue);
}

int EModuleResponse::toInt() const {
	return ENUM_VALUES.value(mValue);
}
