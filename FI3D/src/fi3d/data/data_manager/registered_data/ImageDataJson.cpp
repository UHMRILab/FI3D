#include <fi3d/data/data_manager/registered_data/ImageDataJson.h>

using namespace fi3d;

ImageDataJson::ImageDataJson(ImageDataVPtr imageData) 
	: mImageData(imageData),
	mTransverseJson(),
	mSagittalJson(),
	mCoronalJson()
{
	if (mImageData.Get() == Q_NULLPTR) {
		return;
	}

	int* dims = mImageData->GetDimensions();
	for (int i = 0; i < dims[2]; i++) {
		mTransverseJson.append(MessagePtr(new Message()));
	}
	for (int i = 0; i < dims[0]; i++) {
		mSagittalJson.append(MessagePtr(new Message()));
	}
	for (int i = 0; i < dims[1]; i++) {
		mCoronalJson.append(MessagePtr(new Message()));
	}
}

ImageDataJson::~ImageDataJson() {}

ImageDataVPtr ImageDataJson::getImageData() {
	return mImageData;
}

MessagePtr ImageDataJson::getMessage(const int& sliceIndex, const ESliceOrientation & orientation) {
	if (mImageData.Get() == Q_NULLPTR || sliceIndex < 0) {
		return Q_NULLPTR;
	}

	int* dims = mImageData->GetDimensions();
	switch (orientation.toInt()) {
		case ESliceOrientation::XY:
			if (sliceIndex >= dims[2]) {
				return Q_NULLPTR;
			}
			return mTransverseJson[sliceIndex];
		case ESliceOrientation::YZ:
			if (sliceIndex >= dims[0]) {
				return Q_NULLPTR;
			}
			return mSagittalJson[sliceIndex];
		case ESliceOrientation::XZ:
			if (sliceIndex >= dims[1]) {
				return Q_NULLPTR;
			}
			return mCoronalJson[sliceIndex];
		default:
			return Q_NULLPTR;
	}
}

void ImageDataJson::setMessage(const int& sliceIndex, const ESliceOrientation& orientation, MessagePtr data) {
	if (mImageData.Get() == Q_NULLPTR || sliceIndex < 0) {
		return;
	}

	int* dims = mImageData->GetDimensions();
	switch (orientation.toInt()) {
		case ESliceOrientation::XY:
			if (sliceIndex >= dims[2]) {
				return;
			}
			mTransverseJson[sliceIndex] = data;
		case ESliceOrientation::YZ:
			if (sliceIndex >= dims[0]) {
				return;
			}
			mSagittalJson[sliceIndex] = data;
		case ESliceOrientation::XZ:
			if (sliceIndex >= dims[1]) {
				return;
			}
			mCoronalJson[sliceIndex] = data;
		default:
			break;
	}
}