#pragma once
/*!
* @author	VelazcoJD
* @file		Visual3D.h
* @class	fi3d::Visual3D
* @brief	Interface to a 3D Visual.
*/

#include <fi3d/rendering/visuals/Visual.h>

#include <QSharedPointer>

#include <vtkSmartPointer.h>
#include <vtkTransform.h>
#include <vtkMatrix4x4.h>
#include <vtkProp3D.h>

namespace fi3d {
class Visual3D : public Visual {

	Q_OBJECT

signals:
	/// @brief Emitted when the Visual has movied, e.g., translated.
	void changedTransformation();

	/// @brief Emitted when the parent Visual has changed.
	void changedSceneParent(QSharedPointer<Visual3D> parent);

private:
	/*! @brief The visual's transform.
	 *
	 * Represents the object's location, orientaiton, and scale in 3D space.
	 */
	vtkSmartPointer<vtkTransform> mTransform;

	/*!
	 * @brief The Visual's parent in 3D space.
	 *
	 * It's a hierarchical relationship. The Visual's transform is
	 * relative to the parent (or space's origin when parent is null).
	 */
	QSharedPointer<Visual3D> mParentVisual;

public:
	/// @brief Constructor.
	Visual3D(const QString& id);

	/// @brief Destructor.
	~Visual3D();

public slots:
	/// @brief Resets the Visual's location, orientation, and scale
	/// back to 0 (an identity matrix).
	void resetTransform();

	/*!
	 * @brief Sets the parent Visual.
	 *
	 * This visual moves with respect to the parent visual. It also
	 * moves with the parent when the parent moves.
	 */
	void setParentVisual(QSharedPointer<Visual3D> parent);

	/*!
	*	@name Position Functions
	*	@brief Positions the Visual by the given values.
	*/
	/// @{
	virtual void setPosition(const double& x, const double& y, const double& z);

	virtual void setPositionX(const double& x);
	virtual void setPositionY(const double& y);
	virtual void setPositionZ(const double& z);

	virtual void translate(const double& x, const double& y, const double& z);

	/// @brief Unlike the other position methods, this function places 
	/// the object relative to the world coordinate system (or it's 
	/// Visual parent if available).*/
	virtual void setPositionGlobal(const double& x, const double& y, const double& z);
	/// @}

	/*!
	*	@name Rotation Functions
	*	@brief Rotates the Visual by the given angles in degrees.
	*/
	/// @{
	virtual void rotate(const double& x, const double& y, const double& z);
	virtual void rotateX(const double& x);
	virtual void rotateY(const double& y);
	virtual void rotateZ(const double& z);
	virtual void rotateWXYZ(const double& w, const double& x, const double& y, const double& z);
	/// @}

	/*!
	 * @bvief Scales the Visual by the given ratios.
	 *
	 * A value of 1 means to keep the current scale (no change).
	 */
	virtual void setScale(const double& x, const double& y, const double& z);

public:
	/// @brief Copies the values from the given transform.
	void setTransformData(vtkTransform* source);

	/// @brief Copies the values from the given matrix.
	void setTransformData(vtkMatrix4x4* source);

	/// @brief Sets the order for transform concatenation to post-order.
	void setTransformToPostMultiply();

	/// @brief Sets the order for transform concatenation to pre-order.
	void setTransformToPreMultiply();

	/*!
	* @name Point/Vector Transform Functions
	* @brief Transforms the given point/vector by the Visual's
	*	transformation.
	*/
	/// @{
	virtual void transformVector(double in[3], double out[3]) const;
	virtual void transformVector(double v[3]) const;
	virtual void transformPoint(double in[3], double out[3]) const;
	virtual void transformPoint(double p[3]) const;
	/// @}

	/// @brief Get the Visual's transform relative to its parent.
	vtkSmartPointer<vtkTransform> getRelativeTransform() const;

	/// @brief Get the Visual's matrix relative to its parent.
	vtkSmartPointer<vtkMatrix4x4> getRelativeMatrix() const;

	/// @brief Get a copy of the transform.
	vtkSmartPointer<vtkTransform> getTransformCopy() const;

	/// @brief Get a copy of the transform matrix.
	vtkSmartPointer<vtkMatrix4x4> getMatrixCopy() const;

	/// @brief Get a copy of the transform matrix in an array.
	void getTransformData(double values[16]) const;

	/// @brief Gets the inverse transform of the Visual's transform.
	vtkSmartPointer<vtkTransform> getInverseTransform() const;

	/// @brief Gets the inverse matrix of the Visual's transform.
	vtkSmartPointer<vtkMatrix4x4> getInverseMatrix() const;

	/*!
	*	@name Position/Orientation/Scale getters.
	*	@brief Gets the position, orientation, and scale of this Visual.
	*/
	/// @{
	void getPosition(double& x, double& y, double& z) const;
	void getPosition(double p[3]) const;
	void getOrientation(double& x, double& y, double& z) const;
	void getOrientation(double a[3]) const;
	void getOrientationWXYZ(double a[4]);
	void getScales(double& x, double& y, double& z) const;
	void getScales(double s[3]) const;
	/// @}

	/*! @brief Checks whether this Visual has a parent Visual. */
	bool hasParentVisual() const;

	/*! @brief Gets the parent Visual. Null if there's no parent. */
	QSharedPointer<Visual3D> getParentVisual() const;

	/*! @brief Gets the parent's visual ID name. Empty if no parent. */
	QString getParentVisualName() const;

	/*!
	*	@name Visual interface implementations.
	*	@brief See Visual for more information.
	*/
	/// @{
	virtual EVisual getVisualType() const override = 0;
	virtual vtkProp3D* getActor() override = 0;
	virtual void setOpacity(const double& a) override = 0;
	virtual double getOpacity() const override = 0;
	virtual void setVisible(const bool& isVisible) override = 0;
	virtual bool isVisible() const override = 0;
	virtual void setPickable(const bool& isPickable) override = 0;
	virtual bool isPickable() const override = 0;
	/// @}

protected:
	/*!
	 * @brief Gets the transform that defines the object's position,
	 * orientation, and scale.
	 *
	 * Be careful with this function, many Visuals are connected through 
	 * signals. All modifications to a Visual's transformation are done through 
	 * the provided functions. If the transform here is manually modified, it 
	 * will not emit any signal. This means that changes to this transformation 
	 * will not update other objects who may be interested that this Visual has 
	 * moved. For instance, updating an HMD or updating GUI elements.
	 *
	 * This function, however, is provided to be used internally by derived 
	 * classes. If changes are made to this transform, it is recommended to 
	 * emit a changedTransformation signal. This would inform interested 
	 * objects about the changes to the transform.
	 */
	vtkTransform* getTransform();
};

/// @brief Alias for a smart pointer of this class.
using Visual3DPtr = QSharedPointer<Visual3D>;

}