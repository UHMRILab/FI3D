#pragma once
/*!
* @author	VelazcoJD
* @file		RegisteredModel.h
* @class	fi3d::RegisteredModel
* @brief	Contains information about a registered ModelData.
*/

#include <fi3d/data/data_manager/registered_data/RegisteredData.h>

#include <fi3d/server/network/Message.h>

#include <fi3d/data/ModelData.h>

namespace fi3d {
class RegisteredModel : public RegisteredData {
private:
	/// @brief The ModelData itself.
	ModelDataVPtr mModelData;

	/// @brief The path to the file if data is persistent.
	QString mPath;

	/// @brief The ModelData encoded as a Message ready for delivery.
	MessagePtr mMessage;

public:
	/// @brief Constructor.
	RegisteredModel(ModelDataVPtr modelData, const QString& path = "");

	/// @brief Destructor.
	~RegisteredModel();

	/// @brief Get the ModelData itself.
	ModelDataVPtr getModelData();

	/// @brief Gets the data ID.
	DataID getDataID() override;

	/// @brief Gets the path to where the data is stored, if persistent.
	QString getDataPath();

	/// @brief Sets the data path to where the data is stored, if persistent.
	void setDataPath(const QString& path);

	/// @brief Gets the ModelData in its encoded format as a DataMessage.
	MessagePtr getMessage();

	/// @brief Sets the ModelData encoded version as a Message.
	void setMessage(MessagePtr message);
};

/// @brief Alias for a smart pointer of this class.
using RegisteredModelPtr = QSharedPointer<RegisteredModel>;

}