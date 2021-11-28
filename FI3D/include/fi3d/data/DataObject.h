#pragma once
/*!
* @author	VelazcoJD
* @file		DataObject.h
* @class	fi3d::DataObject
* @brief	Base class with information of every data object.
*/

#include <QObject>

#include <fi3d/data/DataID.h>
#include <fi3d/data/EData.h>

#include <QJsonValue>

namespace fi3d {
class DataManager;
class DataObject : public QObject {

	Q_OBJECT
	friend class DataManager;

signals:
	/*! 
	 * @brief Emmitted when the data contents have changed.
	 *
	 * This signal should be emitted when the internal data changes. 
	 * Alternatively, call the dataUpdated function which will emit the signal.
	 */
	void changedData();

	/*!
	 * @brief Emitted when the cacheable state has changed.
	 */
	void changedCacheable(const bool& mIsCacheable);

private:
	/// @brief The ID of the object.
	DataID mID;

	/*!
	 * @brief Whether this data benefits from being cached, defaults to true.
	 * 
	 * If this data object changes frequently, disable this so that those
	 * who request the data know they shouldn't cache this data for future use.
	 * This will ensure they receive up to date information rather than using
	 * old cached data. 
	 */
	bool mIsCacheable;

public:
	/// @brief Constructor.
	DataObject();

	/// @brief Destructor.
	~DataObject();

	/// @brief Gets the DataID of the data object.
	DataID getDataID() const;

	/// @brief Sets the data name.
	void setDataName(const QString& name);

	/// @brief Gets the data name.
	QString getDataName() const;

	/// @brief Gets data type.
	virtual EData getDataType() const = 0;

	virtual bool getCacheable() const;

	virtual void setCacheable(const bool& isCacheable);


	/*! 
	 * @brief Emits the changedData signal.
	 *
	 * This funciton should be called when the internal data is changed.
	 * This is useful to notify objects about changes to the data. This is
	 * used in the DataManager. 
	 */
	void dataUpdated();

private:
	/// @brief Sets the QUuID (data ID), called by the DataManager.
	void setDataQUuID(const QUuid id);

	/// @brief Gets the QUuID as a string.
	QString getQUuIDAsString();
};
}