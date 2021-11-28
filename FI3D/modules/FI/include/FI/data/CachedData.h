#pragma once
/*!
* @author	VelazcoJD
* @file		CachedData.h
* @class	fi::CachedData
* @brief	Information about a data object that was cached.
*/

#include <QSharedPointer>

namespace fi {
class CachedData {
private:
	/// @brief The ID of the data object.
	QString mDataID;

	/// @brief Whether the data object's data is already cached.
	bool mIsCached;

public:
	/// @brief Constructor.
	CachedData();

	/// @brief Destructor.
	~CachedData();

	/// @brief Sets the data ID.
	void setFI3DDataID(const QString& dataID);

	/// @brief Constructor.
	QString getFI3DDataID() const;

	/// @brief Set cached or not.
	void setCached(const bool& isCached);

	/// @brief Whether the data object's data is cached.
	bool isCached() const;

	/// @brief Gets called when the data object should delete the data.
	virtual void release() = 0;
};

/// @brief Alias for a smart pointer of this class.
using CachedDataPtr = QSharedPointer<CachedData>;

}