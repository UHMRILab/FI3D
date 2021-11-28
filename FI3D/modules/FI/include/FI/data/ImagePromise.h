#pragma once
/*!
* @author	VelazcoJD
* @file		ImagePromise.h
* @class	fi::ImagePromise
* @brief	A Promise that resolves to a CachedImage.
*/

#include <fi3d/utilities/Promise.h>
#include <FI/data/CachedImage.h>

namespace fi {
class ImagePromise : public fi3d::Promise<CachedImageVPtr> {

public:
	/// @brief Constructor.
	ImagePromise();

	/// @brief Destructor.
	~ImagePromise();
};

/// @brief Alias for a smart pointer of this class.
using ImagePromisePtr = QSharedPointer<ImagePromise>;

}