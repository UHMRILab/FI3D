#include <fi3d/rendering/visuals/EVisual.h>

#include <QHash>
#include <QString>
#include <QSharedPointer>

using namespace fi3d;

QHash<int, QSharedPointer<EVisual>> EVisual::ENUM_TYPES {
	{
		EVisual::UNKNOWN, 
		QSharedPointer<EVisual>(new EVisual(Q_NULLPTR))
	},
	{
		EVisual::IMAGE_SLICE, 
		QSharedPointer<EVisual>(new EVisual(Q_NULLPTR))
	},
	{
		EVisual::STUDY_IMAGE_SLICE, 
		QSharedPointer<EVisual>(new EVisual(Q_NULLPTR))
	},
	{
		EVisual::MODEL, 
		QSharedPointer<EVisual>(new EVisual(Q_NULLPTR))
	},
	{
		EVisual::ASSEMBLY,
		QSharedPointer<EVisual>(new EVisual(Q_NULLPTR))
	},
	{
		EVisual::SUBTITLE,
		QSharedPointer<EVisual>(new EVisual(Q_NULLPTR))
	},
	{
		EVisual::ANIMATED_STUDY_SLICE,
		QSharedPointer<EVisual>(new EVisual(Q_NULLPTR))
	},
	{
		EVisual::ANIMATED_MODEL,
		QSharedPointer<EVisual>(new EVisual(Q_NULLPTR))
	},
};

QHash<EVisual*, int> EVisual::ENUM_VALUES {
	{
		EVisual::ENUM_TYPES.value(EVisual::UNKNOWN).data(), 
		EVisual::UNKNOWN
	},
	{
		EVisual::ENUM_TYPES.value(EVisual::IMAGE_SLICE).data(),
		EVisual::IMAGE_SLICE
	},
	{
		EVisual::ENUM_TYPES.value(EVisual::STUDY_IMAGE_SLICE).data(), 
		EVisual::STUDY_IMAGE_SLICE
	},
	{
		EVisual::ENUM_TYPES.value(EVisual::MODEL).data(),
		EVisual::MODEL
	},
	{
		EVisual::ENUM_TYPES.value(EVisual::ASSEMBLY).data(),
		EVisual::ASSEMBLY
	},
	{
		EVisual::ENUM_TYPES.value(EVisual::SUBTITLE).data(),
		EVisual::SUBTITLE
	},
	{
		EVisual::ENUM_TYPES.value(EVisual::ANIMATED_STUDY_SLICE).data(),
		EVisual::ANIMATED_STUDY_SLICE
	},
	{
		EVisual::ENUM_TYPES.value(EVisual::ANIMATED_MODEL).data(),
		EVisual::ANIMATED_MODEL
	}
};

QHash<EVisual*, QString> EVisual::ENUM_NAMES{
	{
		EVisual::ENUM_TYPES.value(EVisual::UNKNOWN).data(), 
		"Unknown Visual Type"
	},
	{
		EVisual::ENUM_TYPES.value(EVisual::IMAGE_SLICE).data(),
		"Image Slice"
	},
	{
		EVisual::ENUM_TYPES.value(EVisual::STUDY_IMAGE_SLICE).data(), 
		"Study Image Slice"
	},
	{
		EVisual::ENUM_TYPES.value(EVisual::MODEL).data(),
		"Model"
	},
	{
		EVisual::ENUM_TYPES.value(EVisual::ASSEMBLY).data(),
		"Assembly"
	},
	{
		EVisual::ENUM_TYPES.value(EVisual::SUBTITLE).data(),
		"Subtitle"
	},
	{
		EVisual::ENUM_TYPES.value(EVisual::ANIMATED_STUDY_SLICE).data(),
		"Animated Study Slice"
	},
	{
		EVisual::ENUM_TYPES.value(EVisual::ANIMATED_MODEL).data(),
		"Animated Model"
	},
};

EVisual::EVisual(EVisual* type) {
	mValue = type;
}

QList<int> EVisual::keys() {
	return ENUM_TYPES.keys();
}

EVisual::EVisual() : EVisual(EVisual::UNKNOWN) {}

EVisual::EVisual(const EVisual& mtype) {
	mValue = mtype.mValue;
}

EVisual::EVisual(const int& valueType) {
	if (ENUM_TYPES.contains(valueType))
		mValue = ENUM_TYPES.value(valueType).data();
	else
		mValue = ENUM_TYPES.value(EVisual::UNKNOWN).data();
}

EVisual::~EVisual() {}

EVisual& EVisual::operator=(const int& valueType) {
	if (ENUM_TYPES.contains(valueType))
		mValue = ENUM_TYPES.value(valueType).data();
	else
		mValue = ENUM_TYPES.value(EVisual::UNKNOWN).data();
	return *this;
}

EVisual& EVisual::operator=(const EVisual& other) {
	if (this != &other) {
		mValue = other.mValue;
	}
	return *this;
}

bool EVisual::operator==(const int& valueType) const {
	if (this->toInt() == valueType) {
		return true;
	} else {
		return false;
	}
}

bool EVisual::operator==(const EVisual& other) const {
	if (mValue == other.mValue) {
		return true;
	}
	return false;
}

bool EVisual::operator!=(const int& valueType) const {
	return !(*this == valueType);
}

bool EVisual::operator!=(const EVisual& other) const {
	return !(*this == other);
}

QString EVisual::getName() const {
	return ENUM_NAMES.value(mValue);
}

int EVisual::toInt() const {
	return ENUM_VALUES.value(mValue);
}

bool EVisual::isSlice() {
	return (
		this->toInt() == EVisual::IMAGE_SLICE ||
		this->toInt() == EVisual::STUDY_IMAGE_SLICE ||
		this->toInt() == EVisual::ANIMATED_STUDY_SLICE
		);
}

bool EVisual::isStudySlice() {
	return (
		this->toInt() == EVisual::STUDY_IMAGE_SLICE ||
		this->toInt() == EVisual::ANIMATED_STUDY_SLICE
		);
}

bool EVisual::isModel() {
	return (
		this->toInt() == EVisual::MODEL ||
		this->toInt() == EVisual::ANIMATED_MODEL
		);
}
