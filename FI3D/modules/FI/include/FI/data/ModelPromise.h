#pragma once
/*!
* @author	VelazcoJD
* @file		ModelPromise.h
* @class	fi::ModelPromise
* @brief	A Promise that resolves to a CachedModel.
*/

#include <fi3d/utilities/Promise.h>
#include <FI/data/CachedModel.h>

namespace fi {
class ModelPromise : public fi3d::Promise<CachedModelVPtr> {

public:
	/// @brief Constructor.
	ModelPromise();

	/// @brief Destructor.
	~ModelPromise();
};

/// @brief Alias for a smart pointer of this class.
using ModelPromisePtr = QSharedPointer<ModelPromise>;

}