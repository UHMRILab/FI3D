#include <fi3d/data/data_manager/EDM_State.h>

#include <QHash>
#include <QString>
#include <QSharedPointer>

using namespace fi3d;

QHash<int, QSharedPointer<EDM_State>> EDM_State::ENUM_TYPES{
	{
		EDM_State::UNKNOWN,
		QSharedPointer<EDM_State>(new EDM_State(Q_NULLPTR))
	},
	{
		EDM_State::SUCCESS, 
		QSharedPointer<EDM_State>(new EDM_State(Q_NULLPTR))
	},
	{
		EDM_State::DATA_NOT_FOUND,
		QSharedPointer<EDM_State>(new EDM_State(Q_NULLPTR))
	},
	{
		EDM_State::ALREADY_REGISTERED,
		QSharedPointer<EDM_State>(new EDM_State(Q_NULLPTR))
	},
};

QHash<EDM_State*, int> EDM_State::ENUM_VALUES{
	{
		EDM_State::ENUM_TYPES.value(EDM_State::UNKNOWN).data(),
		EDM_State::UNKNOWN
	},
	{
		EDM_State::ENUM_TYPES.value(EDM_State::SUCCESS).data(),
		EDM_State::SUCCESS
	},
	{
		EDM_State::ENUM_TYPES.value(EDM_State::DATA_NOT_FOUND).data(),
		EDM_State::DATA_NOT_FOUND
	},
	{
		EDM_State::ENUM_TYPES.value(EDM_State::ALREADY_REGISTERED).data(), 
		EDM_State::ALREADY_REGISTERED
	},
};

QHash<EDM_State*, QString> EDM_State::ENUM_NAMES{
	{
		EDM_State::ENUM_TYPES.value(EDM_State::UNKNOWN).data(),
		"Unknown Scene Object Type"
	},
	{
		EDM_State::ENUM_TYPES.value(EDM_State::SUCCESS).data(), 
		"Success"
	},
	{
		EDM_State::ENUM_TYPES.value(EDM_State::DATA_NOT_FOUND).data(), 
		"Data not found"
	},
	{
		EDM_State::ENUM_TYPES.value(EDM_State::ALREADY_REGISTERED).data(), 
		"Already Registered"
	},
};

EDM_State::EDM_State(EDM_State* type) {
	mValue = type;
}

QList<int> EDM_State::keys() {
	return ENUM_TYPES.keys();
}

EDM_State::EDM_State() : EDM_State(EDM_State::UNKNOWN) {}

EDM_State::EDM_State(const EDM_State& mtype) {
	mValue = mtype.mValue;
}

EDM_State::EDM_State(const int& valueType) {
	if (ENUM_TYPES.contains(valueType))
		mValue = ENUM_TYPES.value(valueType).data();
	else
		mValue = ENUM_TYPES.value(EDM_State::UNKNOWN).data();
}

EDM_State::~EDM_State() {}

EDM_State& EDM_State::operator=(const int& valueType) {
	if (ENUM_TYPES.contains(valueType))
		mValue = ENUM_TYPES.value(valueType).data();
	else
		mValue = ENUM_TYPES.value(EDM_State::UNKNOWN).data();
	return *this;
}

EDM_State& EDM_State::operator=(const EDM_State& other) {
	if (this != &other) {
		mValue = other.mValue;
	}
	return *this;
}

bool EDM_State::operator==(const int& valueType) const {
	if (this->toInt() == valueType) {
		return true;
	} else {
		return false;
	}
}

bool EDM_State::operator==(const EDM_State& other) const {
	if (mValue == other.mValue) {
		return true;
	}
	return false;
}

bool EDM_State::operator!=(const int& valueType) const {
	return !(*this == valueType);
}

bool EDM_State::operator!=(const EDM_State& other) const {
	return !(*this == other);
}

QString EDM_State::getName() const {
	return ENUM_NAMES.value(mValue);
}

int EDM_State::toInt() const {
	return ENUM_VALUES.value(mValue);
}

bool EDM_State::isSuccess() {
	return mValue == ENUM_TYPES.value((EDM_State::SUCCESS)).data();
}
