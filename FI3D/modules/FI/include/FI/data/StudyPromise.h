#pragma once
/*!
* @author	VelazcoJD
* @file		StudyPromise.h
* @class	fi::StudyPromise
* @brief	A Promise that resolves to a CachedStudy.
*/

#include <fi3d/utilities/Promise.h>
#include <FI/data/CachedStudy.h>

namespace fi {
class StudyPromise : public fi3d::Promise<CachedStudyPtr> {

public:
	/// @brief Constructor.
	StudyPromise();

	/// @brief Destructor.
	~StudyPromise();
};

/// @brief Alias for a smart pointer of this class.
using StudyPromisePtr = QSharedPointer<StudyPromise>;

}