#include <FI/data/CachedStudy.h>

using namespace fi;
using namespace fi3d;

CachedStudy::CachedStudy() {}

CachedStudy::~CachedStudy() {}

void CachedStudy::release() {
	// TODO: Release the underlying Series.
}

bool CachedStudy::isSliceCached(const int& sliceIndex, const ESliceOrientation& orientation, const int& seriesIndex) {
	if (seriesIndex < 0 || seriesIndex >= mSeriesStates.count()) {
		return false;
	}

	CacheSeriesState series = mSeriesStates[seriesIndex];
	switch (orientation.toInt()) {
		case ESliceOrientation::XY:
			return series.TransverseSlices.value(sliceIndex, false);
		case ESliceOrientation::YZ:
			return series.SagittalSlices.value(sliceIndex, false);
		case ESliceOrientation::XZ:
			return series.CoronalSlices.value(sliceIndex, false);
		default:
			return false;
	}

	return false;
}
