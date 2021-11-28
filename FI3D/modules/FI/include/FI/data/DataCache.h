#pragma once
/*!
* @author	VelazcoJD
* @file		DataCache.h
* @class	fi::DataCache
* @brief	Contains all the cached data in the FI module.
*/

#include <QObject>

#include <fi3d/rendering/visuals/3D/slices/ESliceOrientation.h>

#include <FI/data/CachedImage.h>
#include <FI/data/CachedModel.h>
#include <FI/data/CachedStudy.h>
#include <FI/data/ImagePromise.h>
#include <FI/data/ModelPromise.h>
#include <FI/data/StudyPromise.h>

#include <fi3d/server/network/Message.h>

#include <QHash>
#include <QJsonObject>

namespace fi {

/// @brief ImageSlice data request.
typedef struct ImageSliceRequestKey {
	QString DataID;
	int SliceIndex;
	fi3d::ESliceOrientation SliceOrientation;
} ImageSliceRequestKey;

/// @brief Compares two ImageSliceRequestKey instances, needed for QHash.
inline bool operator==(const ImageSliceRequestKey& k1, const ImageSliceRequestKey& k2) {
	return k1.DataID == k2.DataID && k1.SliceIndex == k2.SliceIndex && k1.SliceOrientation == k2.SliceOrientation;
}

/// @brief Hashes the ImageSliceRequestKey, needed for QHash.
inline uint qHash(const ImageSliceRequestKey& key, uint seed) {
	return qHash(key.DataID, seed) ^ key.SliceIndex ^ key.SliceOrientation.toInt();
}

/// @brief ImageSlice data request.
typedef struct StudySliceRequestKey {
	QString DataID;
	int SliceIndex;
	fi3d::ESliceOrientation SliceOrientation;
	int SeriesIndex;
} StudySliceRequestKey;

/// @brief Compares two StudySliceRequestKey instances, needed for QHash.
inline bool operator==(const StudySliceRequestKey& k1, const StudySliceRequestKey& k2) {
	return 
		k1.DataID == k2.DataID && 
		k1.SliceIndex == k2.SliceIndex &&
		k1.SliceOrientation == k2.SliceOrientation && 
		k1.SeriesIndex == k2.SeriesIndex;
}

/// @brief Hashes the StudySliceRequestKey, needed for QHash.
inline uint qHash(const StudySliceRequestKey& key, uint seed) {
	return qHash(key.DataID, seed) ^ key.SliceIndex ^ key.SliceOrientation.toInt() ^ key.SeriesIndex;
}

class DataCache : public QObject {

	Q_OBJECT

signals:
	/// @brief Emitted when a new data request is available.
	void dataRequest(QJsonObject& request, const QString& message);

private:
	/// @brief Cached Images.
	QHash<QString, CachedImageVPtr> mImages;

	/// @brief Cached Models.
	QHash<QString, CachedModelVPtr> mModels;

	/// @brief Cached Studies.
	QHash<QString, CachedStudyPtr> mStudies;

	/// @brief Active ImageData requests.
	QHash<ImageSliceRequestKey, ImagePromisePtr> mImageSliceRequests;

	/// @brief Active ModelData requests.
	QHash<QString, ModelPromisePtr> mModelRequests;

	/// @brief Active Study requests.
	QHash<StudySliceRequestKey, StudyPromisePtr> mStudyRequests;

public:
	/// @brief Constructor.
	DataCache();

	/// @brief Destructor.
	~DataCache();

	/// @brief Requests an ImageData slice.
	ImagePromisePtr getImageData(const QString& dataID, const int& index, const fi3d::ESliceOrientation& orientation);

	/// @brief Requests a Model.
	ModelPromisePtr getModelData(const QString& dataID);

	/// @brief Requests a Study slice.
	StudyPromisePtr getStudy(const QString& dataID, const int& index, 
		const fi3d::ESliceOrientation& orientation, const int& series);

public slots:
	/// @brief Handles a model data response.
	void handleDataMessage(fi3d::MessagePtr message);
};

/// @brief Alias for a smart pointer of this class.
using DataCachePtr = QSharedPointer<DataCache>;

}