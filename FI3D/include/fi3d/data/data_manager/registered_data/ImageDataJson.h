#pragma once
/*!
* @author	VelazcoJD
* @file		ImageDataJson.h
* @class	fi3d::ImageDataJson
* @brief	Contains data about an ImageData in Json format.
*/

#include <fi3d/rendering/visuals/3D/slices/ESliceOrientation.h>

#include <fi3d/data/ImageData.h>

#include <fi3d/server/network/Message.h>

#include <QVector>
#include <QJsonValue>

namespace fi3d {
class ImageDataJson {
private:
	/// @brief The ImageData itself.
	ImageDataVPtr mImageData;

	/// @brief The ImageData slices in their JSON format.
	QVector<MessagePtr> mTransverseJson, mSagittalJson, mCoronalJson;

public:
	ImageDataJson(){};

	/// @brief Constructor.
	ImageDataJson(ImageDataVPtr imageData);

	/// @brief Destructor.
	~ImageDataJson();

	/// @brief Gets the registered ImageData object.
	ImageDataVPtr getImageData();

	/// @brief Gets the JSON data of a slice.
	MessagePtr getMessage(const int& sliceIndex, const ESliceOrientation& orientation);

	/// @brief Sets the JSON data of a slice.
	void setMessage(const int& sliceIndex, const ESliceOrientation& orientation, MessagePtr data);
};
}