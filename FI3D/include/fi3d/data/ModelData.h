#pragma once
/*!
* @author	VelazcoJD
* @file		ModelData.h
* @class	fi3d::ModelData
* @brief	Stores data of a singular polygon mesh.
*/

#include <fi3d/data/DataObject.h>

#include <vtkPolyData.h>

namespace fi3d {
class ModelData : public DataObject, public vtkPolyData {

	Q_OBJECT

public:
	/*! @brief VTK object dependencies. */
	static ModelData* New();
	vtkTypeMacro(ModelData, vtkPolyData)

	/*! @brief Constructor. */
	ModelData();

	/*! @brief Destructor. */
	~ModelData();

	/// @brief Return the right data type.
	EData getDataType() const override;

	/*!
	 * @brief Creates a cylinder with the given parameters.
	 *
	 * @param radius The radius of the cylinder.
	 * @param height The height of the cylinder.
	 * @param resolution The quality of the cylinder. Defaults to 100.
	 */
	static vtkSmartPointer<ModelData> createCylinder(const double& radius, 
		const double& height, const int& resolution = 100);

	/*!
	 * @brief Creates a tube with the given parementers.
	 *
	 * @param radius The radius of the tube.
	 * @param p0 The start of the tube.
	 * @param p1 The end of the tube.
	 * @param closeEnds Whether the ends of the tube should be closed or open.
	 * @param resolution The quality of the tube. Defaults to 100.
	 */
	static vtkSmartPointer<ModelData> createTube(const double& radius,
		double(&p0)[3], double(&p1)[3], const bool& closedEnds = false, 
		const int& resolution = 50);

	/*!
	 * @brief Creates a sphere with the given parementers.
	 *
	 * @param radius The radius of the sphere.
	 * @param p The center of the sphere.
	 * @param resolution The quality of the sphere. Defaults to 100.
	 */
	static vtkSmartPointer<ModelData> createSphere(const double& radius,
		double(&p)[3], const int& resolution = 50);
};

/// @brief Alias for a smart pointer of this class.
using ModelDataVPtr = vtkSmartPointer<ModelData>;

}