#pragma once
/*!
* @author	VelazcoJD
* @file		CachedModel.h
* @class	fi::CachedStudy
* @brief	A Study that is being cached.
*/

#include <fi3d/data/Study.h>
#include <FI/data/CachedData.h>

#include <fi3d/rendering/visuals/3D/slices/ESliceOrientation.h>

namespace fi {
class DataCache;
class CachedStudy : public fi3d::Study, public CachedData {

	Q_OBJECT
	friend class DataCache;

private:
	/// @brief Holds cache state of each slice in a series.
	typedef struct CacheSeriesState {
		QVector<bool> TransverseSlices;
		QVector<bool> SagittalSlices;
		QVector<bool> CoronalSlices;
	} CacheSeriesState;

	/// @brief State of the cached slices.
	QVector<CacheSeriesState> mSeriesStates;

public:
	/// @brief Constructor.
	CachedStudy();

	/// @brief Destructor.
	~CachedStudy();

	/// @brief Deletes the 3D image arrays comprising the study.
	virtual void release() override;

	/// @brief Checks whether the slice is cached.
	bool isSliceCached(const int& sliceIndex, const fi3d::ESliceOrientation& orientation, const int& seriesIndex);
};

/// @brief Alias for a smart pointer of this class.
using CachedStudyPtr = QSharedPointer<CachedStudy>;

}