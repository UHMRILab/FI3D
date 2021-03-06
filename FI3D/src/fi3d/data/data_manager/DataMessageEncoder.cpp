#include <fi3d/data/data_manager/DataMessageEncoder.h>

#include <fi3d/logger/Logger.h>

#include <fi3d/data/DataManager.h>
#include <fi3d/data/EData.h>

#include <fi3d/server/message_keys/MessageKeys.h>

#include <QJsonArray>

using namespace fi3d;

/// @brief Inserts the common values for a response.
void prepareDataResponse(QJsonObject& jsonObject, 
	const EResponseStatus& status = EResponseStatus::SUCCESS, 
	const QString& message = "")
{
	jsonObject.insert(RESPONSE_STATUS, status.toInt());
	jsonObject.insert(MESSAGE_TYPE, EMessage::DATA);
	jsonObject.insert(MESSAGE, message);
}

DataMessageEncoder::DataMessageEncoder(DataManager* manager)
	: MessageEncoder(EMessage::DATA),
	mDataManager(manager)
{
}

DataMessageEncoder::~DataMessageEncoder() {}

void DataMessageEncoder::parseRequest(const QJsonObject& request, const QString& clientID, const QString& message) {
	qDebug() << "Enter - Parsing request from" << clientID;
	qDebug() << "Request:\n" << request;

	EData dataType = request.value(DATA_TYPE).toInt();
	switch (dataType.toInt()) {
		case EData::IMAGE:
			this->parseImageDataRequest(request, clientID);
			break;
		case EData::STUDY:
			this->parseStudyDataRequest(request, clientID);
			break;
		case EData::MODEL:
			this->parseModelDataRequest(request, clientID);
			break;
		default:
			qWarning() << "Received data request for an unknown data type";
			break;
	}

	qDebug() << "Exit";
}

void DataMessageEncoder::parseImageDataRequest(const QJsonObject& request, const QString& clientID) {
	qDebug() << "Enter";

	DataID dataID(request.value(DATA_ID).toString());
	int sliceIndex = request.value(SLICE_INDEX).toInt();
	ESliceOrientation orientation = request.value(SLICE_ORIENTATION).toInt();

	RegisteredImagePtr regImage = mDataManager->mRegisteredImages.value(dataID);

	if (regImage.isNull()) {
		QJsonObject response;
		QString message = tr("ImageData %1 was not found").arg(dataID.getDataName());
		prepareDataResponse(response, EResponseStatus::ERROR_RESPONSE, message);
		this->sendMessage(response, clientID);
		return;
	}

	MessagePtr dataMessage = regImage->getMessage(sliceIndex, orientation);
	if (dataMessage.isNull()) {
		QString message = tr("ImageData %1 slice was not found").arg(dataID.getDataName());
		QJsonObject response;
		this->prepareDataErrorResponse(response, message);
		this->sendMessage(response, clientID);
		return;
	}

	if (!dataMessage->isMessageValid()) {
		DataMessageEncoder::toMessage(regImage->getImageData(), sliceIndex, orientation, dataMessage);

		QSharedPointer<QJsonObject> infoJson(new QJsonObject());
		prepareDataResponse(*infoJson.data());
		infoJson->insert(DATA, *dataMessage->getInfo());

		dataMessage->setInfoAndPayload(infoJson, dataMessage->getPayload());
	}

	this->sendMessage(dataMessage, clientID);
	
	// If the ImageData is not cacheable, get rid of the message data.
	if (!regImage->getImageData()->getCacheable()) {
		dataMessage->invalidate();
	}

	qDebug() << "Exit";
}

void DataMessageEncoder::parseStudyDataRequest(const QJsonObject& request, const QString& clientID) {
	qDebug() << "Enter";

	DataID dataID(request.value(DATA_ID).toString());
	int sliceIndex = request.value(SLICE_INDEX).toInt();
	ESliceOrientation orientation = request.value(SLICE_ORIENTATION).toInt();
	int seriesIndex = request.value(SERIES_INDEX).toInt();

	RegisteredStudyPtr regStudy = mDataManager->mRegisteredStudies.value(dataID);
	if (regStudy.isNull()) {
		QJsonObject response;
		QString message = tr("Study %1 was not found").arg(dataID.toString());
		this->prepareDataErrorResponse(response, message);
		this->sendMessage(response, clientID);
		return;
	}
	
	MessagePtr dataMessage = regStudy->getMessage(sliceIndex, orientation, seriesIndex);
	if (dataMessage.isNull()) {
		QString message = tr("Requested slice for Study %1 was not found").arg(dataID.toString());
		QJsonObject response;
		this->prepareDataErrorResponse(response, message);
		this->sendMessage(response, clientID);
		return;
	}

	if (!dataMessage->isMessageValid()) {
		DataMessageEncoder::toMessage(regStudy->getStudy().data(), sliceIndex, orientation, seriesIndex, dataMessage);
		
		QSharedPointer<QJsonObject> infoJson(new QJsonObject());
		prepareDataResponse(*infoJson.data());
		infoJson->insert(DATA, *dataMessage->getInfo());
		
		dataMessage->setInfoAndPayload(infoJson, dataMessage->getPayload());
	}

	this->sendMessage(dataMessage, clientID);

	// If the Study is not cacheable, get rid of the message data.
	if (!regStudy->getStudy()->getCacheable()) {
		dataMessage->invalidate();
	}

	qDebug() << "Exit";
}

void DataMessageEncoder::parseModelDataRequest(const QJsonObject& request, const QString& clientID) {
	qDebug() << "Enter";

	DataID dataID(request.value("DataID").toString());
	RegisteredModelPtr regModel = mDataManager->mRegisteredModels.value(dataID);
	if (regModel.isNull()) {
		QJsonObject response;
		QString message = tr("ModelData %1 was not found").arg(dataID.getDataName());
		prepareDataResponse(response, EResponseStatus::ERROR_RESPONSE, message);
		this->sendMessage(response, clientID);
		return;
	}

	MessagePtr dataMessage = regModel->getMessage();
	if (dataMessage.isNull()) {
		QString message = tr("ModelData %1 was not found").arg(dataID.getDataName());
		QJsonObject response;
		this->prepareDataErrorResponse(response, message);
		this->sendMessage(response, clientID);
		return;
	}

	if (!dataMessage->isMessageValid()) {
		DataMessageEncoder::toMessage(regModel->getModelData(), dataMessage);

		QSharedPointer<QJsonObject> infoJson(new QJsonObject());
		prepareDataResponse(*infoJson.data());
		infoJson->insert(DATA, *dataMessage->getInfo());

		dataMessage->setInfoAndPayload(infoJson, dataMessage->getPayload());
	}

	this->sendMessage(dataMessage, clientID);
	if (!regModel->getModelData()->getCacheable()) {
		dataMessage->invalidate();
	}

	qDebug() << "Exit";
}

void DataMessageEncoder::prepareDataErrorResponse(QJsonObject& jsonObject, const QString & message) {
	prepareDataResponse(jsonObject, EResponseStatus::ERROR_RESPONSE, message);
}

MessagePtr DataMessageEncoder::toMessage(ImageData* data, const int& sliceIndex, const ESliceOrientation& orientation) 
{
	MessagePtr dataMessage(new Message());
	DataMessageEncoder::toMessage(data, sliceIndex, orientation, dataMessage);
	return dataMessage;
}

bool DataMessageEncoder::toMessage(ImageData* data, const int& sliceIndex,
	const ESliceOrientation& orientation, MessagePtr dataMessage)
{
	qDebug() << "Enter - Converting ImageSlice: SliceIndex=" << sliceIndex <<
		"Orientation=" << orientation.getName();

	if (data == Q_NULLPTR) {
		qWarning() << "Failed to convert ImageSlice to JSON: data is null";
		qDebug() << "Exit - Null data";
		return false;
	}

	int dims[3];
	double orig[3];
	double spac[3];
	data->GetDimensions(dims);
	data->GetOrigin(orig);
	data->GetSpacing(spac);

	QSharedPointer<QByteArray> payload(new QByteArray());
	if (orientation == ESliceOrientation::XY) {
		int z = sliceIndex;
		if (sliceIndex < 0 || sliceIndex >= dims[2]) {
			qWarning() << "Failed to convert Study ImageSlice to JSON: slice index is out of range";
			qDebug() << "Exit - Slice out of range";
			return false;
		}
		payload->reserve(dims[0] * dims[1] * sizeof(float));
		for (int y = 0; y < dims[1]; y++) {
			for (int x = 0; x < dims[0]; x++) {
				unsigned char* val = static_cast<unsigned char*>(data->GetScalarPointer(x, y, z));

				float value = val[0] / 255.0f;
				payload->append((const char*)&value, sizeof(float));
			}
		}
	} else if (orientation == ESliceOrientation::YZ) {
		int x = sliceIndex;
		if (sliceIndex < 0 || sliceIndex >= dims[0]) {
			qWarning() << "Failed to convert Study ImageSlice to JSON: slice index is out of range";
			qDebug() << "Exit - Slice out of range";
			return false;
		}
		payload->reserve(dims[1] * dims[2] * sizeof(float));
		for (int z = 0; z < dims[2]; z++) {
			for (int y = 0; y < dims[1]; y++) {
				unsigned char* val = static_cast<unsigned char*>(data->GetScalarPointer(x, y, z));

				float value = val[0] / 255.0f;
				payload->append((const char*)&value, sizeof(float));
			}
		}
	} else if (orientation == ESliceOrientation::XZ) {
		int y = sliceIndex;
		if (sliceIndex < 0 || sliceIndex >= dims[1]) {
			qWarning() << "Failed to convert Study ImageSlice to JSON: slice index is out of range";
			qDebug() << "Exit - Slice out of range";
			return false;
		}
		payload->reserve(dims[0] * dims[2] * sizeof(float));
		for (int z = 0; z < dims[2]; z++) {
			for (int x = 0; x < dims[0]; x++) {
				unsigned char* val = static_cast<unsigned char*>(data->GetScalarPointer(x, y, z));
				
				float value = val[0] / 255.0f;
				payload->append((const char*)&value, sizeof(float));
			}
		}
	} else {
		qWarning() << "Failed to convert Study ImageSlice to JSON: orientation is unknown";
		qDebug() << "Exit - Unknown orientation";
		return false;
	}

	QSharedPointer<QJsonObject> imageInfo(new QJsonObject());
	imageInfo->insert(DATA_ID, data->getDataID().toString());
	imageInfo->insert(DATA_NAME, data->getDataName());
	imageInfo->insert(DATA_TYPE, EData::STUDY);
	imageInfo->insert(CACHEABLE, data->getCacheable());
	imageInfo->insert(SLICE_ORIENTATION, orientation.toInt());
	imageInfo->insert(SLICE_INDEX, sliceIndex);

	QJsonArray dimensions = {dims[0], dims[1], dims[2]};
	imageInfo->insert(DIMENSIONS, dimensions);

	QJsonArray origin = {orig[0], orig[1], orig[2]};
	imageInfo->insert(ORIGIN, origin);

	QJsonArray spacing = {spac[0], spac[1], spac[2]};
	imageInfo->insert(SPACING, spacing);

	// TODO: Need enumeration, 1 indicates gray scale.
	imageInfo->insert(DATA_FORMAT, 1);

	qDebug() << "Converted an Image to Message with" << payload->count() << "bytes";
	dataMessage->setInfoAndPayload(imageInfo, payload);

	qDebug() << "Exit";
	return true;
}

MessagePtr DataMessageEncoder::toMessage(Study* study, const int& sliceIndex,
	const ESliceOrientation& orientation, const int& seriesIndex) {
	MessagePtr dataMessage(new Message());
	DataMessageEncoder::toMessage(study, sliceIndex, orientation, seriesIndex, dataMessage);
	return dataMessage;
}

bool DataMessageEncoder::toMessage(Study* study, const int& sliceIndex, 
	const ESliceOrientation& orientation,const int& seriesIndex, 
	MessagePtr dataMessage) 
{
	qDebug() << "Enter - Converting StudySlice: SliceIndex=" << sliceIndex << 
		"Orientation=" << orientation.getName() << "seriesIndex=" <<
		seriesIndex;

	if (study == Q_NULLPTR) {
		qWarning() << "Failed to convert study image slice: data is null";
		qDebug() << "Exit - Null data";
		return false;
	}

	qDebug() << "Study has" << study->getSeriesCount() << "series";

	ImageDataVPtr series = study->getSeries(seriesIndex);
	bool imageDataOk = DataMessageEncoder::toMessage(series, sliceIndex, orientation, dataMessage);
	if (imageDataOk) {
		QSharedPointer<QJsonObject> imageInfo = dataMessage->getInfo();

		// Replace the DataID, with the study's DataID.
		imageInfo->insert(DATA_ID, study->getDataID().toString());

		// Replace cacheable flag with the study's flag
		imageInfo->insert(CACHEABLE, study->getCacheable());
		imageInfo->insert(SERIES_INDEX, seriesIndex);
		imageInfo->insert(SERIES_COUNT, study->getSeriesCount());
	}

	return imageDataOk;
}

bool DataMessageEncoder::toMessage(ModelData* data, MessagePtr dataMessage) {
	qDebug() << "Enter";

	if (data == Q_NULLPTR) {
		qWarning() << "Failed to convert ModelData to JSON: data is null";
		qDebug() << "Exit - Null data";
		return false;
	}
	
	// TODO: Add vertices and lines to the data delivery.
	int pointCount = data->GetNumberOfPoints();
	int pointBytes = pointCount * 3 * sizeof(float);

	QSharedPointer<QByteArray> payload(new QByteArray());
	payload->reserve(pointBytes);
	for (int i = 0; i < data->GetNumberOfPoints(); i++) {
		double* point = data->GetPoints()->GetPoint(i);
		for (int j = 0; j < 3; j++) {
			float value = point[j];
			payload->append((const char*)&value, sizeof(float));
		}
	}
	
	// TODO: This can be further improved if we can determine the # of 
	// triangles there is in the ModelData. 
	int triangleCount = 0;
	QByteArray trianglesPayload;
	// TODO: If # of triangles can be determined, change this reserve to that.
	trianglesPayload.reserve(data->GetNumberOfCells());
	for (int i = 0; i < data->GetNumberOfCells(); i++) {
		vtkCell* cell = data->GetCell(i);
		vtkIdList* vertIds = cell->GetPointIds();
	
		if (cell->GetCellType() == VTK_TRIANGLE) {
			triangleCount++;
			for (int j = 0; j < 3; j++) {
				int value = vertIds->GetId(j);
				trianglesPayload.append((const char*)&value, sizeof(int));
			}
		}
	}
	int triangleBytes = trianglesPayload.count();
	payload->append(trianglesPayload);

	qDebug() << "Total bytes=" << payload->count();
	qDebug() << "Point Bytes=" << pointBytes << "Triangle Bytes=" << triangleBytes;
	qDebug() << "Points=" << pointCount << "Triangles=" << triangleCount;

	QSharedPointer<QJsonObject> modelInfo(new QJsonObject());
	modelInfo->insert(DATA_ID, data->getDataID().toString());
	modelInfo->insert(DATA_NAME, data->getDataName());
	modelInfo->insert(DATA_TYPE, EData::MODEL);
	modelInfo->insert(CACHEABLE, data->getCacheable());
	//TODO: Needs enumeration, 1 is XYZ (coordinates)
	modelInfo->insert(DATA_FORMAT, 1);
	modelInfo->insert(PAYLOAD_POINTS_LENGTH, pointBytes);
	modelInfo->insert(PAYLOAD_TRIANGLES_LENGTH, triangleBytes);

	dataMessage->setInfoAndPayload(modelInfo, payload);

	qDebug() << "Exit";
	return true;
}

MessagePtr DataMessageEncoder::toMessage(ModelData* data) {
	MessagePtr dataMessage(new Message());
	DataMessageEncoder::toMessage(data, dataMessage);
	return dataMessage;
}
