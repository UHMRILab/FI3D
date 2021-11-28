#include <FI/data/CachedData.h>

using namespace fi;

CachedData::CachedData() 
	: mDataID(""),
	mIsCached(false)
{}

CachedData::~CachedData() {}

void CachedData::setFI3DDataID(const QString& dataID) {
	mDataID = dataID;
}

QString CachedData::getFI3DDataID() const {
	return mDataID;
}

void CachedData::setCached(const bool& isCached) {
	mIsCached = isCached;
}

bool CachedData::isCached() const {
	return mIsCached;
}