#include <fi3d/rendering/visuals/3D/slices/ESliceOrientation.h>

#include <QHash>
#include <QString>
#include <QSharedPointer>

using namespace fi3d;

QHash<int, QSharedPointer<ESliceOrientation>> ESliceOrientation::ENUM_TYPES{
	{
		ESliceOrientation::UNKNOWN, 
		QSharedPointer<ESliceOrientation>(new ESliceOrientation(Q_NULLPTR))
	},
	{
		ESliceOrientation::XY,
		QSharedPointer<ESliceOrientation>(new ESliceOrientation(Q_NULLPTR))
	},
	{
		ESliceOrientation::YZ,
		QSharedPointer<ESliceOrientation>(new ESliceOrientation(Q_NULLPTR))
	},
	{
		ESliceOrientation::XZ,
		QSharedPointer<ESliceOrientation>(new ESliceOrientation(Q_NULLPTR))
	}
};

QHash<ESliceOrientation*, int> ESliceOrientation::ENUM_VALUES{
	{
		ESliceOrientation::ENUM_TYPES.value(ESliceOrientation::UNKNOWN).data(),
		ESliceOrientation::UNKNOWN
	},
	{
		ESliceOrientation::ENUM_TYPES.value(ESliceOrientation::XY).data(),
		ESliceOrientation::XY
	},
	{
		ESliceOrientation::ENUM_TYPES.value(ESliceOrientation::YZ).data(),
		ESliceOrientation::YZ
	},
	{
		ESliceOrientation::ENUM_TYPES.value(ESliceOrientation::XZ).data(),
		ESliceOrientation::XZ
	}
};

QHash<ESliceOrientation*, QString> ESliceOrientation::ENUM_NAMES{
	{
		ESliceOrientation::ENUM_TYPES.value(ESliceOrientation::UNKNOWN).data(),
		"Unknown Orientation"
	},
	{
		ESliceOrientation::ENUM_TYPES.value(ESliceOrientation::XY).data(),
		"Transverse"
	},
	{
		ESliceOrientation::ENUM_TYPES.value(ESliceOrientation::YZ).data(),
		"Sagittal"
	},
	{
		ESliceOrientation::ENUM_TYPES.value(ESliceOrientation::XZ).data(),
		"Coronal"
	}
};

ESliceOrientation::ESliceOrientation(ESliceOrientation* type) {
	mValue = type;
}

QList<int> ESliceOrientation::keys() {
	return ENUM_TYPES.keys();
}

ESliceOrientation::ESliceOrientation() 
	: ESliceOrientation(ESliceOrientation::UNKNOWN) 
{
}

ESliceOrientation::ESliceOrientation(const ESliceOrientation& mtype) {
	mValue = mtype.mValue;
}

ESliceOrientation::ESliceOrientation(const int& valueType) {
	if (ENUM_TYPES.contains(valueType))
		mValue = ENUM_TYPES.value(valueType).data();
	else
		mValue = ENUM_TYPES.value(ESliceOrientation::UNKNOWN).data();
}

ESliceOrientation::~ESliceOrientation() {}

ESliceOrientation& ESliceOrientation::operator=(const int& valueType) {
	if (ENUM_TYPES.contains(valueType))
		mValue = ENUM_TYPES.value(valueType).data();
	else
		mValue = ENUM_TYPES.value(ESliceOrientation::UNKNOWN).data();
	return *this;
}

ESliceOrientation& ESliceOrientation::operator=(
	const ESliceOrientation& other) 
{
	if (this != &other) {
		mValue = other.mValue;
	}
	return *this;
}

bool ESliceOrientation::operator==(const int& valueType) const {
	if (this->toInt() == valueType) {
		return true;
	} else {
		return false;
	}
}

bool ESliceOrientation::operator==(const ESliceOrientation& other) const {
	if (mValue == other.mValue) {
		return true;
	}
	return false;
}

bool ESliceOrientation::operator!=(const int& valueType) const {
	return !(*this == valueType);
}

bool ESliceOrientation::operator!=(const ESliceOrientation& other) const {
	return !(*this == other);
}

QString ESliceOrientation::getName() const {
	return ENUM_NAMES.value(mValue);
}

int ESliceOrientation::toInt() const {
	return ENUM_VALUES.value(mValue);
}
