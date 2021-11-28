#include <FI/data/DataCache.h>

#include <fi3d/logger/Logger.h>

#include <fi3d/server/message_keys/MessageKeys.h>
#include <fi3d/data/EData.h>

#include <QJsonArray>

#include <vtkTriangle.h>

using namespace fi;
using namespace fi3d;

/// Helper function that parses JSON data into the corresponding slice.
inline void parseImage(QSharedPointer<QByteArray> bytes, ImageDataVPtr image, 
	const int& sliceIndex, const ESliceOrientation orientation) 
{
	qDebug() << "Enter";
	if (image.Get() == Q_NULLPTR) {
		qCritical() << "Failed to cache slice into given image because image is null.";
		qDebug() << "Exit - Null image";
		return;
	}

	float* values = (float*)bytes->data();
	int valueCount = bytes->count() / sizeof(float);

	int* dims = image->GetDimensions();
	if (orientation == ESliceOrientation::XY) {
		if (sliceIndex < 0 || sliceIndex >= dims[2]) {
			qWarning() << "Failed to cache slice with orientation" << orientation.getName() << 
				"because the given slice index is out of range.";
			qDebug() << "Exit - Index out of range";
			return;
		}

		if (valueCount != dims[0] * dims[1]) {
			qWarning() << "Failed to cache slice with orientation" << orientation.getName() <<
				"because the image dimensions do not match the given data.";
			qDebug() << "Exit - Dimensions and data missmatch";
			return;
		}

		for (int y = 0; y < dims[1]; y++) {
			for (int x = 0; x < dims[0]; x++) {
				image->SetScalarComponentFromDouble(x, y, sliceIndex, 0, values[y * dims[0] + x] * 255.0);
			}
		}
	} else if (orientation == ESliceOrientation::YZ) {
		if (sliceIndex < 0 || sliceIndex >= dims[0]) {
			qWarning() << "Failed to cache slice with orientation" << orientation.getName() <<
				"because the given slice index is out of range";
			qDebug() << "Exit - Index out of range";
			return;
		}

		if (valueCount != dims[1] * dims[2]) {
			qWarning() << "Failed to cache slice with orientation" << orientation.getName() <<
				"because the image dimensions do not match the given data.";
			qDebug() << "Exit - Dimensions and data missmatch";
			return;
		}

		for (int z = 0; z < dims[2]; z++) {
			for (int y = 0; y < dims[1]; y++) {
				image->SetScalarComponentFromDouble(sliceIndex, y, z, 0, values[z * dims[1] + y] * 255.0);
			}
		}
	} else if (orientation == ESliceOrientation::XZ) {
		if (sliceIndex < 0 || sliceIndex >= dims[1]) {
			qWarning() << "Failed to cache slice with orientation" << orientation.getName() <<
				"because the given slice index is out of range";
			qDebug() << "Exit - Index out of range";
			return;
		}

		if (valueCount != dims[0] * dims[2]) {
			qWarning() << "Failed to cache slice with orientation" << orientation.getName() <<
				"because the image dimensions do not match the given data.";
			qDebug() << "Exit - Dimensions and data missmatch";
			return;
		}

		for (int z = 0; z < dims[2]; z++) {
			for (int x = 0; x < dims[0]; x++) {
				image->SetScalarComponentFromDouble(x, sliceIndex, z, 0, values[z * dims[0] + x] * 255.0);
			}
		}
	} else {
		qWarning() << "Failed to cache slice because the given orientation is uknown.";
	}

	qDebug() << "Exit";
}

DataCache::DataCache()
	: QObject(),
	mImages(), mModels(), mStudies()
{}

DataCache::~DataCache() {}

ImagePromisePtr DataCache::getImageData(const QString& dataID, const int& index, const ESliceOrientation& orientation){
	qDebug() << "Enter";

	bool needData = false;
	ImagePromisePtr image;
	if (mImages.contains(dataID)) {
		CachedImageVPtr im = mImages.value(dataID);
		if (im->isSliceCached(index, orientation)) {
			image.reset(new ImagePromise());
			image->resolve(im);
		} else {
			needData = true;
		}
	} else {
		needData = true;
	}

	if (needData) {
		ImageSliceRequestKey request {
			dataID, index, orientation
		};

		image = mImageSliceRequests.value(request);
		if (image.isNull()) {
			image.reset(new ImagePromise());

			QJsonObject dataParams;
			dataParams.insert(DATA_TYPE, EData::IMAGE);
			dataParams.insert(DATA_ID, dataID);
			dataParams.insert(SLICE_INDEX, index);
			dataParams.insert(SLICE_ORIENTATION, orientation.toInt());

			mImageSliceRequests.insert(request, image);

			emit dataRequest(dataParams, "");
		}
	}

	qDebug() << "Exit";
	return image;
}

ModelPromisePtr DataCache::getModelData(const QString& dataID) {
	ModelPromisePtr model;
	if (mModels.contains(dataID)) {
		model.reset(new ModelPromise());
		CachedModelVPtr mod = mModels.value(dataID);
		model->resolve(mod);
	} else {
		if (mModelRequests.contains(dataID)) {
			model = mModelRequests.value(dataID);
		} else {
			model.reset(new ModelPromise());

			QJsonObject dataParams;
			dataParams.insert(DATA_TYPE, EData::MODEL);
			dataParams.insert(DATA_ID, dataID);

			mModelRequests.insert(dataID, model);

			emit dataRequest(dataParams, "");
		}
	}

	return model;
}

StudyPromisePtr DataCache::getStudy(const QString& dataID, const int& index,
	const ESliceOrientation& orientation, const int& series) 
{
	bool needData = false;
	StudyPromisePtr study;
	if (mStudies.contains(dataID)) {
		CachedStudyPtr stud = mStudies.value(dataID);
		if (stud->isSliceCached(index, orientation, series)) {
			study.reset(new StudyPromise());
			study->resolve(stud);
		} else {
			needData = true;
		}
	} else {
		needData = true;
	}

	if (needData) {
		StudySliceRequestKey request{
			dataID, index, orientation, series
		};

		study = mStudyRequests.value(request);
		if (study.isNull()) {
			study.reset(new StudyPromise());
			
			QJsonObject dataParams;
			dataParams.insert(DATA_TYPE, EData::STUDY);
			dataParams.insert(DATA_ID, dataID);
			dataParams.insert(SLICE_INDEX, index);
			dataParams.insert(SLICE_ORIENTATION, orientation.toInt());
			dataParams.insert(SERIES_INDEX, series);

			mStudyRequests.insert(request, study);

			emit dataRequest(dataParams, "");
		}
	}

	return study;
}

void DataCache::handleDataMessage(MessagePtr message) {
	qDebug() << "Enter";

	QJsonObject dataParams = message->getInfo()->value("Data").toObject();

	QString dataID = dataParams.value(DATA_ID).toString();
	EData dataType = dataParams.value(DATA_TYPE).toInt();

	qDebug() << "Received data of type" << dataType.getName() << "with ID" << dataID;

	if (dataType == EData::IMAGE) {
		ImageSliceRequestKey key{
			dataID,
			dataParams.value(SLICE_INDEX).toInt(),
			dataParams.value(SLICE_ORIENTATION).toInt()
		};

		if (key.SliceOrientation == ESliceOrientation::UNKNOWN) {
			qWarning() << "Failed to handle Image data. Orientation is unknown.";
			qDebug() << "Exit - Unknown ImageSlice orientation";
			return;
		}
		
		CachedImageVPtr image = mImages.value(dataID, Q_NULLPTR);
		if (image.Get() == Q_NULLPTR) {
			image = CachedImageVPtr::New();
			image->setFI3DDataID(dataID);

			QJsonArray dims = dataParams.value(DIMENSIONS).toArray();
			QJsonArray spac = dataParams.value(SPACING).toArray();

			image->SetDimensions(dims[0].toInt(), dims[1].toInt(), dims[2].toInt());
			image->SetSpacing(spac[0].toDouble(), spac[1].toDouble(), spac[2].toDouble());
			image->AllocateScalars(VTK_UNSIGNED_CHAR, 1);

			image->mTransverseSlices.resize(dims[2].toInt());
			image->mSagittalSlices.resize(dims[0].toInt());
			image->mCoronalSlices.resize(dims[1].toInt());
		}

		parseImage(message->getPayload(), image, key.SliceIndex, key.SliceOrientation);
		
		switch (key.SliceOrientation.toInt()) {
			case ESliceOrientation::XY:
				image->mTransverseSlices[key.SliceIndex] = true;
				break;
			case ESliceOrientation::YZ:
				image->mSagittalSlices[key.SliceIndex] = true;
				break;
			case ESliceOrientation::XZ:
				image->mCoronalSlices[key.SliceIndex] = true;
				break;
		}

		ImagePromisePtr imPro = mImageSliceRequests.take(key);
		if (!imPro.isNull()) {
			qDebug() << "Resolving image promise";
			imPro->resolve(image);
		}

		mImages.insert(dataID, image);
	} else if (dataType == EData::MODEL) {
		CachedModelVPtr model = mModels.value(dataID, Q_NULLPTR);
		if (model.Get() == Q_NULLPTR) {
			model = CachedModelVPtr::New();
			model->setFI3DDataID(dataID);
		}
		// TODO: Does this function really resets the object to starting state?
		model->Initialize();

		int pointBytes = dataParams.value(PAYLOAD_POINTS_LENGTH).toInt();
		int triangleBytes = dataParams.value(PAYLOAD_TRIANGLES_LENGTH).toInt();

		char* bytes = message->getPayload()->data();
		float* pointValues = (float*)bytes;
		int* triangleValues = (int*)(&bytes[pointBytes]);

		int pointCount = pointBytes / 3 / sizeof(float);
		int triangleCount = triangleBytes / 3 / sizeof(int);
		
		qDebug() << "Caching new Model" << dataID;
		qDebug() << "Point Bytes=" << pointBytes << "Triangle Bytes=" << triangleBytes;
		qDebug() << "Points=" << pointCount << "Triangles=" << triangleCount;

		vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
		points->SetNumberOfPoints(pointCount);
		for (int i = 0; i < pointCount; i++) {
			int index = i * 3;

			double x = pointValues[index];
			double y = pointValues[index + 1];
			double z = pointValues[index + 2];

			points->SetPoint(i, x, y, z);
		}

		vtkSmartPointer<vtkCellArray> cells = vtkSmartPointer<vtkCellArray>::New();
		
		// TODO: add verts and lines
		
		for (int i = 0; i < triangleCount; i++) {
			vtkSmartPointer<vtkTriangle> triangle = vtkSmartPointer<vtkTriangle>::New();
			int index = i * 3;

			int p0 = triangleValues[index];
			int p1 = triangleValues[index + 1];
			int p2 = triangleValues[index + 2];

			triangle->GetPointIds()->SetId(0, p0);
			triangle->GetPointIds()->SetId(1, p1);
			triangle->GetPointIds()->SetId(2, p2);

			cells->InsertNextCell(triangle);
		}

		model->SetPoints(points);
		model->SetPolys(cells);
		
		ModelPromisePtr moPro = mModelRequests.take(dataID);
		if (!moPro.isNull()) {
			qDebug() << "Resolving model promise";
			moPro->resolve(model);
		}

		mModels.insert(dataID, model);
	} else if (dataType == EData::STUDY) {
		StudySliceRequestKey key {
			dataID,
			dataParams.value(SLICE_INDEX).toInt(),
			dataParams.value(SLICE_ORIENTATION).toInt(),
			dataParams.value(SERIES_INDEX).toInt()
		};

		if (key.SliceOrientation == ESliceOrientation::UNKNOWN) {
			qWarning() << "Failed to handle Study data. Orientation is unknown.";
			qDebug() << "Exit - Unknown StudySlice orientation";
			return;
		}

		CachedStudyPtr study = mStudies.value(dataID, Q_NULLPTR);
		if (study.isNull()) {
			study.reset(new CachedStudy());
			study->setFI3DDataID(dataID);

			int seriesCount = dataParams.value(SERIES_COUNT).toInt();
			QJsonArray dims = dataParams.value(DIMENSIONS).toArray();
			QJsonArray spac = dataParams.value(SPACING).toArray();

			study->mSeriesStates.resize(seriesCount);
			for (int i = 0; i < seriesCount; i++) {
				SeriesDataVPtr series = study->createAndAddSeries();

				series->SetDimensions(dims[0].toInt(), dims[1].toInt(), dims[2].toInt());
				series->SetSpacing(spac[0].toDouble(), spac[1].toDouble(), spac[2].toDouble());
				series->AllocateScalars(VTK_UNSIGNED_CHAR, 1);

				study->mSeriesStates[i].TransverseSlices.resize(dims[2].toInt());
				study->mSeriesStates[i].SagittalSlices.resize(dims[0].toInt());
				study->mSeriesStates[i].CoronalSlices.resize(dims[1].toInt());
			}
		}

		parseImage(message->getPayload(), study->getSeries(key.SeriesIndex), key.SliceIndex, key.SliceOrientation);
		
		switch (key.SliceOrientation.toInt()) {
			case ESliceOrientation::XY:
				study->mSeriesStates[key.SeriesIndex].TransverseSlices[key.SliceIndex] = true;
				break;
			case ESliceOrientation::YZ:
				study->mSeriesStates[key.SeriesIndex].SagittalSlices[key.SliceIndex] = true;
				break;
			case ESliceOrientation::XZ:
				study->mSeriesStates[key.SeriesIndex].CoronalSlices[key.SliceIndex] = true;
				break;
		}

		StudyPromisePtr stuPro = mStudyRequests.take(key);
		if (!stuPro.isNull()) {
			qDebug() << "Resolving study promise";
			stuPro->resolve(study);
		}

		mStudies.insert(dataID, study);
	} else {
		qWarning() << "Received a data response for an unknown data type.";
	}

	qDebug() << "Exit";
} 
