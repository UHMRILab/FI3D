#include <fi3d/utilities/EIcon.h>

#include <QHash>
#include <QString>
#include <QSharedPointer>

using namespace fi3d;

QHash<int, QSharedPointer<EIcon>> EIcon::ENUM_TYPES{
	{
		EIcon::UNKNOWN,
		QSharedPointer<EIcon>(new EIcon(Q_NULLPTR))
	},
	{
		EIcon::BRAND,
		QSharedPointer<EIcon>(new EIcon(Q_NULLPTR))
	},
	{
		EIcon::SETTINGS,
		QSharedPointer<EIcon>(new EIcon(Q_NULLPTR))
	},
	{
		EIcon::REFRESH,
		QSharedPointer<EIcon>(new EIcon(Q_NULLPTR))
	},
	{
		EIcon::SYNC,
		QSharedPointer<EIcon>(new EIcon(Q_NULLPTR))
	},
	{
		EIcon::SHUTDOWN,
		QSharedPointer<EIcon>(new EIcon(Q_NULLPTR))
	},
	{
		EIcon::BACKWARD,
		QSharedPointer<EIcon>(new EIcon(Q_NULLPTR))
	},
	{
		EIcon::FORWARD,
		QSharedPointer<EIcon>(new EIcon(Q_NULLPTR))
	},
	{
		EIcon::BEGINNING,
		QSharedPointer<EIcon>(new EIcon(Q_NULLPTR))
	},
	{
		EIcon::ENDING,
		QSharedPointer<EIcon>(new EIcon(Q_NULLPTR))
	},
	{
		EIcon::PREVIOUS,
		QSharedPointer<EIcon>(new EIcon(Q_NULLPTR))
	},
	{
		EIcon::NEXT,
		QSharedPointer<EIcon>(new EIcon(Q_NULLPTR))
	},
	{
		EIcon::FIRST,
		QSharedPointer<EIcon>(new EIcon(Q_NULLPTR))
	},
	{
		EIcon::LAST,
		QSharedPointer<EIcon>(new EIcon(Q_NULLPTR))
	},
	{
		EIcon::TRANSVERSE,
		QSharedPointer<EIcon>(new EIcon(Q_NULLPTR))
	},
	{
		EIcon::CORONAL,
		QSharedPointer<EIcon>(new EIcon(Q_NULLPTR))
	},
	{
		EIcon::SAGITTAL,
		QSharedPointer<EIcon>(new EIcon(Q_NULLPTR))
	},
	{
		EIcon::CUT,
		QSharedPointer<EIcon>(new EIcon(Q_NULLPTR))
	},
	{
		EIcon::RECONSTRUCT_SURFACE,
		QSharedPointer<EIcon>(new EIcon(Q_NULLPTR))
	},
	{
		EIcon::POP_UP,
		QSharedPointer<EIcon>(new EIcon(Q_NULLPTR))
	},
};

QHash<EIcon*, int> EIcon::ENUM_VALUES{
	{
		EIcon::ENUM_TYPES.value(EIcon::UNKNOWN).data(),
		EIcon::UNKNOWN
	},
	{
		EIcon::ENUM_TYPES.value(EIcon::BRAND).data(),
		EIcon::BRAND
	},
	{
		EIcon::ENUM_TYPES.value(EIcon::SETTINGS).data(),
		EIcon::SETTINGS
	},
	{
		EIcon::ENUM_TYPES.value(EIcon::REFRESH).data(),
		EIcon::REFRESH
	},
	{
		EIcon::ENUM_TYPES.value(EIcon::SYNC).data(),
		EIcon::SYNC 
	},
	{
		EIcon::ENUM_TYPES.value(EIcon::SHUTDOWN).data(),
		EIcon::SHUTDOWN
	},
	{
		EIcon::ENUM_TYPES.value(EIcon::BACKWARD).data(),
		EIcon::BACKWARD
	},
	{
		EIcon::ENUM_TYPES.value(EIcon::FORWARD).data(),
		EIcon::FORWARD
	},
	{
		EIcon::ENUM_TYPES.value(EIcon::BEGINNING).data(),
		EIcon::BEGINNING
	},
	{
		EIcon::ENUM_TYPES.value(EIcon::ENDING).data(),
		EIcon::ENDING
	},
	{
		EIcon::ENUM_TYPES.value(EIcon::PREVIOUS).data(),
		EIcon::PREVIOUS
	},
	{
		EIcon::ENUM_TYPES.value(EIcon::NEXT).data(),
		EIcon::NEXT
	},
	{
		EIcon::ENUM_TYPES.value(EIcon::FIRST).data(),
		EIcon::FIRST
	},
	{
		EIcon::ENUM_TYPES.value(EIcon::LAST).data(),
		EIcon::LAST
	},
	{
		EIcon::ENUM_TYPES.value(EIcon::TRANSVERSE).data(),
		EIcon::TRANSVERSE
	},
	{
		EIcon::ENUM_TYPES.value(EIcon::CORONAL).data(),
		EIcon::CORONAL
	},
	{
		EIcon::ENUM_TYPES.value(EIcon::SAGITTAL).data(),
		EIcon::SAGITTAL
	},
	{
		EIcon::ENUM_TYPES.value(EIcon::CUT).data(),
		EIcon::CUT
	},
	{
		EIcon::ENUM_TYPES.value(EIcon::RECONSTRUCT_SURFACE).data(),
		EIcon::RECONSTRUCT_SURFACE
	},
	{
		EIcon::ENUM_TYPES.value(EIcon::POP_UP).data(),
		EIcon::POP_UP
	},
};

QHash<EIcon*, QString> EIcon::ENUM_NAMES{
	{
		EIcon::ENUM_TYPES.value(EIcon::UNKNOWN).data(),
		"Unknown Message Type"
	},
	{
		EIcon::ENUM_TYPES.value(EIcon::BRAND).data(),
		"Brand"
	},
	{
		EIcon::ENUM_TYPES.value(EIcon::SETTINGS).data(),
		"Settings"
	},
	{
		EIcon::ENUM_TYPES.value(EIcon::REFRESH).data(),
		"Refresh"
	},
	{
		EIcon::ENUM_TYPES.value(EIcon::SYNC).data(),
		"Sync"
	},
	{
		EIcon::ENUM_TYPES.value(EIcon::SHUTDOWN).data(),
		"Shutdown"
	},
	{
		EIcon::ENUM_TYPES.value(EIcon::BACKWARD).data(),
		"Backward"
	},
	{
		EIcon::ENUM_TYPES.value(EIcon::FORWARD).data(),
		"Forward"
	},
	{
		EIcon::ENUM_TYPES.value(EIcon::BEGINNING).data(),
		"Beginning"
	},
	{
		EIcon::ENUM_TYPES.value(EIcon::ENDING).data(),
		"Ending"
	},
	{
		EIcon::ENUM_TYPES.value(EIcon::PREVIOUS).data(),
		"Previous"
	},
	{
		EIcon::ENUM_TYPES.value(EIcon::NEXT).data(),
		"Next"
	},
	{
		EIcon::ENUM_TYPES.value(EIcon::FIRST).data(),
		"First"
	},
	{
		EIcon::ENUM_TYPES.value(EIcon::LAST).data(),
		"Last"
	},
	{
		EIcon::ENUM_TYPES.value(EIcon::TRANSVERSE).data(),
		"Transverse"
	},
	{
		EIcon::ENUM_TYPES.value(EIcon::CORONAL).data(),
		"Coronal"
	},
	{
		EIcon::ENUM_TYPES.value(EIcon::SAGITTAL).data(),
		"Sagittal"
	},
	{
		EIcon::ENUM_TYPES.value(EIcon::CUT).data(),
		"Cut"
	},
	{
		EIcon::ENUM_TYPES.value(EIcon::RECONSTRUCT_SURFACE).data(),
		"Reconstruct Surface"
	},
	{
		EIcon::ENUM_TYPES.value(EIcon::POP_UP).data(),
		"Pop Up"
	},
};

EIcon::EIcon(EIcon* type) {
	mValue = type;
}

QList<int> EIcon::keys() {
	return ENUM_TYPES.keys();
}

EIcon::EIcon() : EIcon(EIcon::UNKNOWN) {}

EIcon::EIcon(const EIcon& mtype) {
	mValue = mtype.mValue;
}

EIcon::EIcon(const int& valueType) {
	if (ENUM_TYPES.contains(valueType))
		mValue = ENUM_TYPES.value(valueType).data();
	else
		mValue = ENUM_TYPES.value(EIcon::UNKNOWN).data();
}

EIcon::~EIcon() {}

EIcon& EIcon::operator=(const int& valueType) {
	if (ENUM_TYPES.contains(valueType))
		mValue = ENUM_TYPES.value(valueType).data();
	else
		mValue = ENUM_TYPES.value(EIcon::UNKNOWN).data();
	return *this;
}

EIcon& EIcon::operator=(const EIcon& other) {
	if (this != &other) {
		mValue = other.mValue;
	}
	return *this;
}

bool EIcon::operator==(const int& valueType) const {
	if (this->toInt() == valueType) {
		return true;
	} else {
		return false;
	}
}

bool EIcon::operator==(const EIcon& other) const {
	if (mValue == other.mValue) {
		return true;
	}
	return false;
}

bool EIcon::operator!=(const int& valueType) const {
	return !(*this == valueType);
}

bool EIcon::operator!=(const EIcon& other) const {
	return !(*this == other);
}

QString EIcon::getName() const {
	return ENUM_NAMES.value(mValue);
}

int EIcon::toInt() const {
	return ENUM_VALUES.value(mValue);
}
