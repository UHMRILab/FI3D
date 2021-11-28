#pragma once
/*!
* @author	VelazcoJD
* @file		RegisteredImage.h
* @class	fi3d::RegisteredImage
* @brief	Contains information about a registered ImageData.
*/

#include <fi3d/data/data_manager/registered_data/RegisteredData.h>
#include <fi3d/data/data_manager/registered_data/ImageDataJson.h>

#include <fi3d/data/ImageData.h>
#include <fi3d/rendering/visuals/3D/slices/ESliceOrientation.h>

namespace fi3d {
class RegisteredImage : public RegisteredData, public ImageDataJson {
private:
	/// @brief The path to the file if data is persistent.
	QString mPath;

public:
	/// @brief Constructor.
	RegisteredImage(ImageDataVPtr imageData, const QString& path = "");

	/// @brief Destructor.
	~RegisteredImage();

	/// @brief Gets the data ID.
	DataID getDataID() override;

	/// @brief Gets the path to where the data is stored, if persistent.
	QString getDataPath();

	/// @brief Sets the data path to where the data is stored, if persistent.
	void setDataPath(const QString& path);
};

/// @brief Alias for a smart pointer of this class.
using RegisteredImagePtr = QSharedPointer<RegisteredImage>;

}