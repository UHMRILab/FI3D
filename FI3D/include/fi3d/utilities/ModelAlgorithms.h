#pragma once
/*!
* @author	VelazcoJD
* @file		ModelAlgorithms.h
* @class	fi3d::ModelAlgorithms
* @brief	Static functions to manipulate ModelData.
*/

#include <fi3d/data/ModelData.h>
#include <fi3d/data/ImageData.h>

#include <QVector>

#include <vtkMatrix4x4.h>
#include <vtkPlane.h>
#include <vtkSmartPointer.h>
#include <vtkTransform.h>

namespace fi3d {
class ModelAlgorithms {
private:
	ModelAlgorithms() {}

public:
	~ModelAlgorithms() {}

	/*!
	* @brief Combines two ModelData objects into one.
	*
	* see: https://www.vtk.org/Wiki/VTK/Examples/Cxx/Filtering/CombinePolyData
	*
	* @param firstData The first ModelData.
	* @param secondData The second ModelData.
	* @param outData The object to place the output data in.
	* //TODO: Check if the out data can be one of the input data sets.
	*/
	static void combineModelData(
		vtkSmartPointer<vtkPolyData> firstData, 
		vtkSmartPointer<vtkPolyData> secondData,
		vtkSmartPointer<vtkPolyData> outData);

	/*!
	* @brief Combines two ModelData objects into one.
	*
	* see: https://www.vtk.org/Wiki/VTK/Examples/Cxx/Filtering/CombinePolyData
	*
	* @param firstData The first ModelData.
	* @param secondData The second ModelData.
	* @return The combined ModelData.s
	* //TODO: Check if the out data can be one of the input data sets.
	*/
	static ModelDataVPtr combineModelData(
		vtkSmartPointer<vtkPolyData> firstData,
		vtkSmartPointer<vtkPolyData> secondData);

	/*!
	* @brief Combines several ModelData objects into one.
	*
	* see: https://www.vtk.org/Wiki/VTK/Examples/Cxx/Filtering/CombinePolyData
	*
	* @param modelDatas The list of ModelData objects to combine.
	* @param outData The object to place the output data in.
	* //TODO: Check if the out data can be one of the input data sets.
	*/
	static void combineModelData(
		QVector<vtkSmartPointer<vtkPolyData>> modelDatas,
		vtkSmartPointer<vtkPolyData> outData);

	/*!
	* @brief Combines several ModelData objects into one.
	*
	* see: https://www.vtk.org/Wiki/VTK/Examples/Cxx/Filtering/CombinePolyData
	*
	* @param modelDatas The list of ModelData objects to combine.
	* @return The object containing the combined ModelData objects.
	* //TODO: Check if the out data can be one of the input data sets.
	*/
	static ModelDataVPtr combineModelData(QVector<vtkSmartPointer<vtkPolyData>> modelDatas);

	/*!
	 * @brief Clips a ModelData and fills the clipped space.
	 *
	 * Creates a ModelData of another ModelData clipped by the given plane. 
	 * The clipped data then is filled to create a cap.
	 *
	 * see: https://www.vtk.org/Wiki/VTK/Examples/Cxx/Meshes/CapClip
	 *
	 * @param modelData The ModelData to clip.
	 * @param cutPlane The plane to be used to do the clip.
	 * @return The capped model.
	 */
	static ModelDataVPtr capClip(vtkSmartPointer<vtkPolyData> modelData, vtkSmartPointer<vtkPlane> cutPlane);

	/*! 
	 * @brief Gets the filled cap of a clipped ModelData.
	 *
	 * "Slices" the given model by the given plane. The resulting ModelData
	 * is what is left off on the side of the plane's normal. 
	 *
     * @param modelData The ModelData to make a cap out of.
	 * @param cutPlane The plane to be used to do the clip.
	 * @return The cap.
	 */
	static ModelDataVPtr clipModel(vtkSmartPointer<vtkPolyData> modelData, vtkSmartPointer<vtkPlane> cutPlane);

	/*!
	* @brief Converts a ModelData to ImageData. 
	*
	* This functions sets the value to 255 to every converted points and 0 
	* to empty space.
	*
	* @param modelData The data to convert.
	* @param vsX The voxel spacing in the x direction
	* @param vsY The voxel spacing in the y direction
	* @param vsZ The voxel spacing in the z direction
	* @return The converted ImageData.
	*/
	static ImageDataVPtr polyToImage(vtkSmartPointer<vtkPolyData> modelData, 
		const double& vsX, const double& vsY, const double& vsZ);

	/*!
	* @brief Creates a ModelData resulting from transforming another ModelData.
	*
	* @param modelData The ModelData to transform.
	* @param transformMatrix The transformation matrix.
	* @return The transformed ModelData.
	*/
	static ModelDataVPtr transform(vtkSmartPointer<vtkPolyData> modelData,
		vtkSmartPointer<vtkMatrix4x4> transformMatrix);

	/*!
	* @brief Creates a ModelData resulting from transforming another ModelData.
	*
	* @param modelData The ModelData to transform.
	* @param transformMatrix The transform.
	*/
	static ModelDataVPtr transform(vtkSmartPointer<vtkPolyData> modelData, vtkSmartPointer<vtkTransform> transform);

	/*! 
	 * @brief Interpolates along the given points using splines.
	 *
	 * @param points The points to interpolate.
	 * @param resU The resolution of the interpolation in the x-dimension.
	 * @param resV The resolution of the interpolation in the y-dimension.
	 * @param resW The resolution of the interpolation in the z-dimension.
	 * @param closedSplined Whether the spline should loop around and connect.
	 * @return The spline.
	 */
	static ModelDataVPtr splineInterpolate(vtkSmartPointer<vtkPoints> points, 
		const double& resU, const double& resV, const double& resW, 
		const bool& closedSplined);

	/*!
	 * @brief Interpolates along the given points using splines.
	 *
	 * @param points The points to interpolate.
	 * @param resU The resolution of the interpolation in the x-dimension.
	 * @param resV The resolution of the interpolation in the y-dimension.
	 * @param resW The resolution of the interpolation in the z-dimension.
	 * @param closedSplined Whether the spline should loop around and connect.
	 * @param outData The ModelData to store the spline information.
	 */
	static void splineInterpolate(vtkSmartPointer<vtkPoints> points,
		const double& resU, const double& resV, const double& resW,
		const bool& closedSplined, vtkSmartPointer<vtkPolyData> outData);

	/// @brief Reconstruct a surface based on the given point cloud. 
	static ModelDataVPtr reconstructSurface(vtkSmartPointer<vtkPolyData> pointCloud);
};
}
