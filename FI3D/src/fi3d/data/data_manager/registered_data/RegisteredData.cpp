#include <fi3d/data/data_manager/registered_data/RegisteredData.h>

using namespace fi3d;

RegisteredData::RegisteredData() 
	: mIsRegistered(false),
	mIsPersistent(false),
	mIsDataLoaded(false)
{}

RegisteredData::~RegisteredData() {}

void RegisteredData::setRegistered(const bool& isRegistered) {
	mIsRegistered = isRegistered;
}

bool RegisteredData::IsRegistered() {
	return mIsRegistered;
}

void RegisteredData::setPersistent(const bool& isPersistent) {
	mIsPersistent = isPersistent;
}

bool RegisteredData::isPersistent() {
	return mIsPersistent;
}

void RegisteredData::setDataLoaded(const bool& isDataLoaded) {
	mIsDataLoaded = isDataLoaded;
}

bool RegisteredData::isDataLoaded() {
	return mIsDataLoaded;
}
