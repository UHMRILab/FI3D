#pragma once
/*!
* @author	VelazcoJD
* @file		ImageAlgorithms.h
* @class	fi3d::ImageAlgorithms
* @brief	Static functions to manipulate ImageData.
*/

#include <fi3d/data/ImageData.h>
#include <fi3d/data/ModelData.h>

namespace fi3d {
class ImageAlgorithms {
private:
	ImageAlgorithms() {}

public:
	~ImageAlgorithms() {}

	/// <summary>
	/// Applys thresholding to the source image using the given min & max
	/// values and stores result into the given output image.
	/// </summary>
	/// <param name="min">Minimum value to include in the threshold.</param>
	/// <param name="max">Maximum value to include in the threshold.</param>
	/// <param name="source">Source image.</param>
	/// <param name="output">Output image.</param>
	static void applyThreshold(const double& min, const double& max,
		vtkSmartPointer<vtkImageData> source, vtkSmartPointer<vtkImageData> output);

	/// <summary>
	/// Applys thresholding to the source image using the given min & max
	/// values and returns the thresholded image.
	/// </summary>
	/// <param name="min">Minimum value to include in the threshold.</param>
	/// <param name="max">Maximum value to include in the threshold.</param>
	/// <param name="source">Source image.</param>
	/// <returns>Output image.</returns>
	static ImageDataVPtr applyThreshold(const double& min, const double& max, vtkSmartPointer<vtkImageData> source);

	/// <summary>
	/// Computes the edges out of a pure black/white (0 for black, 255 
	/// for white) image. The given image must be a 2D image (third dimension
	/// must be 1).
	/// </summary>
	/// <param name="image">The image.</param>
	/// <returns>The lines corresponding to the edges.</returns>
	static ModelDataVPtr computeEdges(vtkSmartPointer<vtkImageData> image);

	/// @brief Loads the Geology data.
	static ImageDataVPtr loadGeologyData(const QString& dirPath);
};
}
