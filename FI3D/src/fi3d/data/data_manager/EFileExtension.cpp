#include <fi3d/data/data_manager/EFileExtension.h>

using namespace fi3d;

QHash<int, QSharedPointer<EFileExtension>> EFileExtension::ENUM_TYPES = {
	{
		EFileExtension::UNKNOWN,
		QSharedPointer<EFileExtension>(new EFileExtension(Q_NULLPTR))
	},
	{
		EFileExtension::FI3D,
		QSharedPointer<EFileExtension>(new EFileExtension(Q_NULLPTR))
	},
	{
		EFileExtension::JPEG,
		QSharedPointer<EFileExtension>(new EFileExtension(Q_NULLPTR))
	},
	{
		EFileExtension::JPG,
		QSharedPointer<EFileExtension>(new EFileExtension(Q_NULLPTR))
	},
	{
		EFileExtension::PNG,
		QSharedPointer<EFileExtension>(new EFileExtension(Q_NULLPTR))
	},
	{
		EFileExtension::STL,
		QSharedPointer<EFileExtension>(new EFileExtension(Q_NULLPTR))
	},
	{
		EFileExtension::OBJ, 
		QSharedPointer<EFileExtension>(new EFileExtension(Q_NULLPTR))
	},
	{
		EFileExtension::PLY, 
		QSharedPointer<EFileExtension>(new EFileExtension(Q_NULLPTR))
	},
	{
		EFileExtension::JSON,
		QSharedPointer<EFileExtension>(new EFileExtension(Q_NULLPTR))
	}
};

QHash<EFileExtension*, int> EFileExtension::ENUM_VALUES{
	{
		EFileExtension::ENUM_TYPES.value(EFileExtension::UNKNOWN).data(),
		EFileExtension::UNKNOWN
	},
	{
		EFileExtension::ENUM_TYPES.value(EFileExtension::FI3D).data(),
		EFileExtension::FI3D
	},
	{
		EFileExtension::ENUM_TYPES.value(EFileExtension::JPEG).data(),
		EFileExtension::JPEG
	},
	{
		EFileExtension::ENUM_TYPES.value(EFileExtension::JPG).data(),
		EFileExtension::JPG
	},
	{
		EFileExtension::ENUM_TYPES.value(EFileExtension::PNG).data(),
		EFileExtension::PNG
	},
	{
		EFileExtension::ENUM_TYPES.value(EFileExtension::STL).data(),
		EFileExtension::STL
	},
	{
		EFileExtension::ENUM_TYPES.value(EFileExtension::OBJ).data(),
		EFileExtension::OBJ
	},
	{
		EFileExtension::ENUM_TYPES.value(EFileExtension::PLY).data(),
		EFileExtension::PLY
	},
	{
		EFileExtension::ENUM_TYPES.value(EFileExtension::JSON).data(),
		EFileExtension::JSON
	}
};

QHash<EFileExtension*, QString> EFileExtension::ENUM_NAMES{
	{
		EFileExtension::ENUM_TYPES.value(EFileExtension::UNKNOWN).data(),
		"Unknown Module"
	},
	{
		EFileExtension::ENUM_TYPES.value(EFileExtension::FI3D).data(),
		"FI3D"
	},
	{
		EFileExtension::ENUM_TYPES.value(EFileExtension::STL).data(),
		"STL"
	},
	{
		EFileExtension::ENUM_TYPES.value(EFileExtension::OBJ).data(),
		"OBJ"
	},
	{
		EFileExtension::ENUM_TYPES.value(EFileExtension::PLY).data(), 
		"PLY"
	},
	{
		EFileExtension::ENUM_TYPES.value(EFileExtension::JPEG).data(),
		"JPEG"
	},
	{
		EFileExtension::ENUM_TYPES.value(EFileExtension::JPG).data(), 
		"JPG"
	},
	{
		EFileExtension::ENUM_TYPES.value(EFileExtension::PNG).data(),
		"PNG"
	},
	{
		EFileExtension::ENUM_TYPES.value(EFileExtension::JSON).data(),
		"JSON"
	}
};

EFileExtension::EFileExtension(EFileExtension* type) {
	mValue = type;
}

QList<int> EFileExtension::keys() {
	return ENUM_TYPES.keys();
}

EFileExtension::EFileExtension() : EFileExtension(EFileExtension::UNKNOWN) {}

EFileExtension::EFileExtension(const EFileExtension& mtype) {
	mValue = mtype.mValue;
}

EFileExtension::EFileExtension(const QString& value) {
	QString ext = value.toUpper();

	if (ext == ENUM_NAMES.value(ENUM_TYPES.value(EFileExtension::FI3D).data())) {
		mValue = ENUM_TYPES.value(EFileExtension::FI3D).data();
	} else if (ext == ENUM_NAMES.value(ENUM_TYPES.value(EFileExtension::JPEG).data())) {
		mValue = ENUM_TYPES.value(EFileExtension::JPEG).data();
	} else if (ext == ENUM_NAMES.value(ENUM_TYPES.value(EFileExtension::JPG).data())) {
		mValue = ENUM_TYPES.value(EFileExtension::JPG).data();
	} else if (ext == ENUM_NAMES.value(ENUM_TYPES.value(EFileExtension::PNG).data())) {
		mValue = ENUM_TYPES.value(EFileExtension::PNG).data();
	} else if (ext == ENUM_NAMES.value(ENUM_TYPES.value(EFileExtension::STL).data())) {
		mValue = ENUM_TYPES.value(EFileExtension::STL).data();
	} else if (ext == ENUM_NAMES.value(ENUM_TYPES.value(EFileExtension::OBJ).data())) {
		mValue = ENUM_TYPES.value(EFileExtension::OBJ).data();
	} else if (ext == ENUM_NAMES.value(ENUM_TYPES.value(EFileExtension::PLY).data())) {
		mValue = ENUM_TYPES.value(EFileExtension::PLY).data();
	} else if (ext == ENUM_NAMES.value(ENUM_TYPES.value(EFileExtension::JSON).data())) {
		mValue = ENUM_TYPES.value(EFileExtension::JSON).data();
	} else {
		mValue = ENUM_TYPES.value(EFileExtension::UNKNOWN).data();
	}
}

EFileExtension::EFileExtension(const int& valueType) {
	if (ENUM_TYPES.contains(valueType))
		mValue = ENUM_TYPES.value(valueType).data();
	else
		mValue = ENUM_TYPES.value(EFileExtension::UNKNOWN).data();
}

EFileExtension::~EFileExtension() {}

EFileExtension& EFileExtension::operator=(const int& valueType) {
	if (ENUM_TYPES.contains(valueType))
		mValue = ENUM_TYPES.value(valueType).data();
	else
		mValue = ENUM_TYPES.value(EFileExtension::UNKNOWN).data();
	return *this;
}

EFileExtension& EFileExtension::operator=(const QString& valueType) {
	if (valueType == ENUM_NAMES.value(ENUM_TYPES.value(EFileExtension::FI3D).data())) {
		mValue = ENUM_TYPES.value(EFileExtension::FI3D).data();
	} else if (valueType == ENUM_NAMES.value(ENUM_TYPES.value(EFileExtension::JPEG).data())) {
		mValue = ENUM_TYPES.value(EFileExtension::JPEG).data();
	} else if (valueType == ENUM_NAMES.value(ENUM_TYPES.value(EFileExtension::PNG).data())) {
		mValue = ENUM_TYPES.value(EFileExtension::PNG).data();
	} else if (valueType == ENUM_NAMES.value(ENUM_TYPES.value(EFileExtension::JSON).data())) {
		mValue = ENUM_TYPES.value(EFileExtension::JSON).data();
	} else if (valueType == ENUM_NAMES.value(ENUM_TYPES.value(EFileExtension::STL).data())) {
		mValue = ENUM_TYPES.value(EFileExtension::STL).data();
	} else if (valueType == ENUM_NAMES.value(ENUM_TYPES.value(EFileExtension::OBJ).data())) {
		mValue = ENUM_TYPES.value(EFileExtension::OBJ).data();
	} else if (valueType == ENUM_NAMES.value(ENUM_TYPES.value(EFileExtension::PLY).data())) {
		mValue = ENUM_TYPES.value(EFileExtension::PLY).data();
	} else {
		mValue = ENUM_TYPES.value(EFileExtension::UNKNOWN).data();
	}

	return *this;
}

EFileExtension& EFileExtension::operator=(const EFileExtension& other) {
	if (this != &other) {
		mValue = other.mValue;
	}
	return *this;
}

bool EFileExtension::operator==(const int& valueType) const {
	if (this->toInt() == valueType) {
		return true;
	} else {
		return false;
	}
}

bool EFileExtension::operator==(const QString& valueType) const {
	if (ENUM_NAMES.value(mValue) == valueType) {
		return true;
	} else {
		return false;
	}
}

bool EFileExtension::operator==(const EFileExtension& other) const {
	if (this == &other) {
		return true;
	}
	if (this->mValue == other.mValue) {
		return true;
	}
	return false;
}

bool EFileExtension::operator!=(const int& valueType) const {
	return !(*this == valueType);
}

bool EFileExtension::operator!=(const QString & valueType) const {
	return !(*this == valueType);
}

bool EFileExtension::operator!=(const EFileExtension& other) const {
	return !(*this == other);
}

QString EFileExtension::getName() const {
	return ENUM_NAMES.value(mValue);
}

int EFileExtension::toInt() const {
	return ENUM_VALUES.value(mValue);
}

bool EFileExtension::isFI3DFile() {
	return this->toInt() == EFileExtension::FI3D;
}

bool EFileExtension::isImage() {
	return (
		this->toInt() == EFileExtension::JPEG ||
		this->toInt() == EFileExtension::JPG ||
		this->toInt() == EFileExtension::PNG
		);
}

bool EFileExtension::isJPGImage() {
	return (
		this->toInt() == EFileExtension::JPEG ||
		this->toInt() == EFileExtension::JPG
		);
}

bool EFileExtension::isModel() {
	return (
		this->toInt() == EFileExtension::STL ||
		this->toInt() == EFileExtension::OBJ ||
		this->toInt() == EFileExtension::PLY
		);
}
