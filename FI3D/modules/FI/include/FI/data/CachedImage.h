#pragma once
/*!
* @author	VelazcoJD
* @file		CachedImage.h
* @class	fi::CachedImage
* @brief	An ImageData that is being cached.
*/

#include <fi3d/data/ImageData.h>
#include <FI/data/CachedData.h>

#include <fi3d/rendering/visuals/3D/slices/ESliceOrientation.h>

#include <QVector>

namespace fi {
class DataCache;
class CachedImage : public fi3d::ImageData, public CachedData {

	Q_OBJECT
	friend class DataCache;

private:
	/*!
	*	@name Slice Status
	*	@brief Holds status of each slice (cached or not).
	*/
	/// @{
	QVector<bool> mTransverseSlices;
	QVector<bool> mSagittalSlices;
	QVector<bool> mCoronalSlices;
	/// @}

public:
	/// @brief VTK object dependencies.
	static CachedImage* New();
	vtkTypeMacro(CachedImage, fi3d::ImageData)

	/// @brief Constructor.
	CachedImage();

	/// @brief Destructor.
	~CachedImage();

	/// @brief Deletes the underlying 3D array.
	virtual void release() override;

	/// @brief Checks whether the slice is cached.
	bool isSliceCached(const int& index, const fi3d::ESliceOrientation& orientation);
};

/// @brief Alias for a smart pointer of this class.
using CachedImageVPtr = vtkSmartPointer<CachedImage>;

}