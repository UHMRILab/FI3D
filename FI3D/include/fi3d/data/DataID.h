#pragma once
/*!
* @author	VelazcoJD
* @file		DataID.h
* @class	fi3d::DataID
* @brief	Used to uniquely identify a data object.
*/

#include <QUuid>

namespace fi3d {
class DataID : public QUuid {
private:
	/// @brief The name of the data object.
	QString mName;

public:
	/// @brief Constructor based on a QUuID.
	DataID(const QUuid& id = QUuid(), const QString& name = "");

	/// @brief Destructor.
	~DataID();

	/// @brief Gets the QUuID (data ID) of the data object.
	QString getDataName() const;

	/// @brief Sets the data name.
	void setDataName(const QString& name);

	/// @brief Returns true if this ID is valid (returns !isNull() from QUuID).
	bool isIDValid() const;

	/// @brief Copys the ID and Name.
	DataID& operator=(const DataID& other);
};
}