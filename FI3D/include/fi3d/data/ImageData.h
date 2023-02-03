#pragma once
/*!
* @author	VelazcoJD
* @file		ImageData.h
* @class	fi3d::ImageData
* @brief	Same as vtkImageData but as a QObject as well.
*/

#include <fi3d/data/DataObject.h>

#include <vtkImageData.h>
#include <vtkSmartPointer.h>
#include <vtkMatrix4x4.h>

namespace fi3d {
class ImageData : public DataObject, public vtkImageData {

	Q_OBJECT

public:
	/*! @brief VTK object dependencies. */
	static ImageData* New();
	vtkTypeMacro(ImageData, vtkImageData)

	/// @brief Return the right data type.
	EData getDataType() const override;

	/*!
	 * @brief Creates an image data object with the given parameters.
	 *
	 * @param dimX The x-dimension count.
	 * @param dimY The y-dimension count.
	 * @param dimZ The z-dimension count.
	 * @param spaceX The spacing between the x slices.
	 * @param spaceY The spacing between the y slices.
	 * @param spaceZ The spacing between the z slices.
	 * @param type The data type of the data, defaults to unsigned char.
	 */
	static vtkSmartPointer<ImageData> createImageData(
		const int& dimX, const int& dimY, const int& dimZ,
		const double& spaceX, const double& spaceY, const double spaceZ,
		int type = VTK_UNSIGNED_CHAR);

protected:
	/*! @brief Constructor. */
	ImageData();

	/*! @brief Destructor. */
	~ImageData() override = default;

private:
	ImageData(const ImageData&) = delete;
	void operator=(const ImageData&) = delete;
};

/// @brief Alias for a smart pointer of this class.
using ImageDataVPtr = vtkSmartPointer<ImageData>;

}