#include <FI/data/CachedImage.h>

#include <fi3d/logger/Logger.h>

#include <vtkObjectFactory.h>

using namespace fi;
using namespace fi3d;

vtkStandardNewMacro(CachedImage)

CachedImage::CachedImage() 
	: ImageData(),
	mTransverseSlices(),
	mSagittalSlices(),
	mCoronalSlices()
{}

void CachedImage::release() {
	// TODO: Release the 3D data array.
}

bool CachedImage::isSliceCached(const int& index, const ESliceOrientation& orientation)  {
	switch (orientation.toInt()) {
		case ESliceOrientation::XY:
			return mTransverseSlices.value(index, false);
		case ESliceOrientation::YZ:
			return mSagittalSlices.value(index, false);
		case ESliceOrientation::XZ:
			return mCoronalSlices.value(index, false);
		default:
			return false;
	}
}