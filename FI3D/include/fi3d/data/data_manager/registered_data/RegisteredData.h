#pragma once
/*!
* @author	VelazcoJD
* @file		RegisteredData.h
* @class	fi3d::RegisteredData
* @brief	Contains information about a registered data object.
*/

#include <fi3d/data/DataID.h>

#include <QSharedPointer>
#include <QString>

namespace fi3d {
class RegisteredData {
protected:
	/// @brief Whether this data object is registered in the DataManager.
	bool mIsRegistered;

	/// @brief Whether the data is persident.
	bool mIsPersistent;

	/// @brief Whether the data has been loaded (for persistent data only).
	bool mIsDataLoaded;

public:
	/// @brief Constructor.
	RegisteredData();

	/// @brief Destructor.
	~RegisteredData();

	/// @brief Gets the data ID.
	virtual DataID getDataID() = 0;

	/// @brief Sets data registration status.
	void setRegistered(const bool& isRegistered);

	/// @brief Whether the data is registered.
	bool IsRegistered();

	/// @brief Sets persistent status.
	void setPersistent(const bool& isPersistent);

	/// @brief Whether the data is persistent.
	bool isPersistent();

	/// @brief Sets the status on whether the data is loaded.
	void setDataLoaded(const bool& isDataLoaded);

	/// @brief Whether the data is loaded.
	bool isDataLoaded();
};

/// @brief Alias for a smart pointer of this class.
using RegisteredDataPtr = QSharedPointer<RegisteredData>;

}