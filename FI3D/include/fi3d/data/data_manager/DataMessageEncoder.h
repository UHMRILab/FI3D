#pragma once
/*!
*	@author		VelazcoJD
*   @file		DataMessageEncoder.h
*	@class		fi3d::DataMessageEncoder
*	@brief		This message encoder responds to data requests.
*/

#include <fi3d/server/MessageEncoder.h>

#include <fi3d/server/network/Message.h>
#include <fi3d/server/message_keys/EResponseStatus.h>

#include <fi3d/data/Study.h>
#include <fi3d/data/ModelData.h>

#include <fi3d/rendering/visuals/3D/slices/ESliceOrientation.h>

namespace fi3d {
class DataManager;
class DataMessageEncoder : public fi3d::MessageEncoder {

	Q_OBJECT

private:
	/// @brief Pointer to the data manger in charge of this instance.
	DataManager* mDataManager;

public:
	/// @brief Constructor.
	DataMessageEncoder(DataManager* dataManager);

	/// @brief Destructor.
	~DataMessageEncoder();

	/*!
	*	@name Request Parsers
	*/
	/// @{
	virtual void parseRequest(const QJsonObject& request, const QString& clientID, const QString& message) override;
	virtual void parseImageDataRequest(const QJsonObject& request, const QString& clientID);
	virtual void parseStudyDataRequest(const QJsonObject& request, const QString& clientID);
	virtual void parseModelDataRequest(const QJsonObject& request, const QString& clientID);
	/// @}

	/// @brief Sends an error response.
	virtual void prepareDataErrorResponse(QJsonObject& jsonObject, const QString& message = "");

	/// @brief Converts the selected slice to its Message format.
	static MessagePtr toMessage(ImageData* data, const int& sliceIndex, const ESliceOrientation& orientation);

	/// @brief Converts the selected slice to its Message format.
	static bool toMessage(ImageData* data, const int& sliceIndex, 
		const fi3d::ESliceOrientation& orientation, MessagePtr dataMessage);

	/// @brief Same as ImageData toMessage but for a Series.
	static MessagePtr toMessage(Study* data, const int& sliceIndex, 
		const ESliceOrientation& orientation, const int& seriesIndex);

	/// @brief Same as ImageData toMessage but for a Series.
	static bool toMessage(Study* data, const int& sliceIndex,
		const ESliceOrientation& orientation, 
		const int& seriesIndex, MessagePtr dataMessage);

	/// @brief Converts the model to its Message format.
	static bool toMessage(ModelData* data, MessagePtr dataMessage);

	/// @brief Converts the model to its Message format.
	static MessagePtr toMessage(ModelData* data);
};

/// @brief Alias for a smart pointer of this class.
using DataMessageEncoderPtr = QSharedPointer<DataMessageEncoder>;

}