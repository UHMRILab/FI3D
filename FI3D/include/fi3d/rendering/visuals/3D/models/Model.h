#pragma once
/*!
* @author	VelazcoJD
* @file		Model.h
* @class	fi3d::Model
* @brief	A Model renders a polygon mesh in 3D space.
*/

#include <fi3d/rendering/visuals/3D/Visual3D.h>

#include <fi3d/data/ModelData.h>

#include <vtkActor.h>

class vtkPolyDataMapper;

namespace fi3d {
class Model : public Visual3D {

	Q_OBJECT

signals:
	/// @brief Emitted when the data of this Model has changed.
	void changedModelData(fi3d::ModelDataVPtr data);

	/// @brief Emitted when the color of this Model has changed.
	void changedColor(const double& r, const double& g, const double& b);

protected:
	/// @brief The data to display.
	fi3d::ModelDataVPtr mData;

	/// @brief The mapper that maps the data to screen.
	vtkSmartPointer<vtkPolyDataMapper> mMapper;

	/// @brief The actor that renders the mapped data to screen.
	vtkSmartPointer<vtkActor> mActor;

public:
	/*!
	* @brief Constructor.
	*
	* @param id The ID for this Visual.
	* @param data The data to assign. Defaults to an empty data object.
	*/
	Model(const QString& id, ModelDataVPtr data = ModelDataVPtr::New());

	/// @brief Destructor.
	~Model();

public slots:
	/// @brief Sets the data to display.
	void setModelData(fi3d::ModelDataVPtr data);

public:
	/// @brief Gets the data.
	ModelDataVPtr getModelData();

public slots:
	/// @brief Set the color of the Model in RGB (value range 0 to 1).
	void setColor(const double& r, const double& g, const double& b);

public:
	/// @brie Get the Model's color.
	void getColor(double& r, double& g, double& b) const;

public slots:
	/// @brief Set the point size. It only affects vertices. 
	void setPointSize(double sz);

	/// @brief Sets the width of lines. It Only affects lines.
	void setLineWidth(double width);

public:
	/*!
	* @name Axes Directions.
	* @brief Gets the unit vector representing the direction of that
	*	axis based on the rendered Model.
	*/
	/// @{
	virtual void getXDirection(double& x, double& y, double& z) const;
	virtual void getXDirection(double direction[3]) const;
	virtual void getYDirection(double& x, double& y, double& z) const;
	virtual void getYDirection(double direction[3]) const;
	virtual void getZDirection(double& x, double& y, double& z) const;
	virtual void getZDirection(double direction[3]) const;
	/// @}

private slots:
	/// @brief Catches the changedData signal from the assigned ModelData and
	/// remits it as the local changedModelData signal
	void onDataUpdated();

public:
	/*!
	*	@name Visual interface implementations.
	*	@brief See Visual for more information.
	*/
	/// @{
	virtual EVisual getVisualType() const override;
	virtual vtkActor* getActor() override;
	virtual void setOpacity(const double& a) override;
	virtual double getOpacity() const override;
	virtual void setVisible(const bool& isVisible) override;
	virtual bool isVisible() const override;
	virtual void setPickable(const bool& isPickable) override;
	virtual bool isPickable() const override;
	/// @}
};

/// @brief Alias for a smart pointer of this class.
using ModelPtr = QSharedPointer<Model>;

}