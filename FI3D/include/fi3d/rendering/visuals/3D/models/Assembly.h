#pragma once
/*!
* @author	VelazcoJD
* @file		Assembly.h
* @class	fi3d::Assembly
* @brief	An Assembly is composed of many Model instances and used to treat
*			many Model instances as a single entity.
*
* This is a useful Visual to be used in situations where a single Model is
* composed of many other models, like a robot. Under situations where a
* structure is saved as many models (several STL files for example), these can
* be combined as a single Visual but can still be kept as separate data. Do 
* note that this is different than just parenting visuals with each other, the
* difference being that parents do not obstruct Visual movement/interaction in
* the scene. This Visual puts its parts together as a single Visual in the
* scene. This allows the interaction with any part of the Assembly to be the
* same as if the Assembly was one single object.

* The transformation scheme in an assembly is interesting and its sole purpose
* is to, again, visualize many models as a single Visual in the scene. This is
* needed because of how VTK interacts with objects. However, it may be needed 
* to interact with the individual parts (like move them relative to the whole
* Assembly). In this case, you can get the parts from Assembly using the part
* ID, or Visual ID, and modify them like you modify a regular Model. You can
* also set parenting within the assembly so that you can specify if a Visual
* should move with respect to others within the Assembly.
*/

#include <fi3d/rendering/visuals/3D/models/Model.h>

#include <fi3d/data/ModelData.h>

#include <vtkAssembly.h>

template <typename K, typename V>
class QHash;
template <typename T>
class QList;

namespace fi3d {
class Assembly : public Visual3D {

	Q_OBJECT

signals:
	/// @brief Emitted when a new Model was added to the assembly.
	void changedAddedPart(ModelPtr part);

	/// @brief Emitted when a Model was removed from the assembly.
	void changedRemovedPart(ModelPtr part);

	/// @brief Emitted when an inner part changed its opacity.
	void changedPartOpacity(const double& a, ModelPtr part);

	/// @brief Emitted when an inner part changed its visibility.
	void changedPartVisible(const bool& isVisible, ModelPtr part);

	/// @brief Emitted when an inner part was transformed.
	void changedPartTransformation(ModelPtr part);

	/// @brief Emitted when an inner part had its data changed.
	void changedPartData(fi3d::ModelDataVPtr data, ModelPtr part);

	/// @brief Emitted when an inner part had its color changed.
	void changedPartColor(const double& r, const double& g, const double& b, ModelPtr part);

protected:
	/// @brief Actor to display children in scene.
	vtkSmartPointer<vtkAssembly> mActor;

	/// @brief Hash tables containing the Model parts.
	QHash<QString, ModelPtr> mParts;

public:
	/// @brief Constructor.
	Assembly(const QString& id);

	/// @brief Destructor.
	~Assembly();

public slots:
	/// @brief Creates and adds a new part to the assembly.
	ModelPtr addPart(const QString& partID, fi3d::ModelDataVPtr data = fi3d::ModelDataVPtr::New());

	/// @brief Adds a pre-constructed part to the assembly.
	void addPart(ModelPtr part);

public:
	/// @brief Get the part with the specified name. Null if not found.
	ModelPtr getPart(const QString& partID);

public slots:
	/// @brief Removes the part with the given ID from the Assembly.
	void removePart(const QString& partID);

	/// @brief Removes all the parts from the Assembly.
	void removeAllParts();

public:
	/// @brief Gets all the parts of this Assembly.
	QList<ModelPtr> getParts() const;

	/// @brief Gets how many parts are in the Assembly.
	int getPartCount() const;

	/*!
	*	@name Visual interface implementations.
	*	@brief See Visual for more information.
	*/
	/// @{
	/*!
	 * @brief Sets the opacity to every part in the assembly.
	 *
	 * The opacity of the Assembly object is unchanged. That is, no
	 * changedOpacity signal is emitted since the Assembly Visual itself
	 * has nothing to render.
	 */
	virtual void setOpacity(const double& a) override;

	/*!
	 * @brief Always returns 1.0 because the Assembly itself has nothing
	 *		to render.
	 *
	 * Each Assembly part can have its own opacity, so it is best to
	 * check on the part's opacity instead.
	 */
	virtual double getOpacity() const override;
	virtual EVisual getVisualType() const override;
	virtual vtkAssembly* getActor() override;
	virtual void setVisible(const bool& isVisible) override;
	virtual bool isVisible() const override;
	virtual void setPickable(const bool& isPickable) override;
	virtual bool isPickable() const override;
	/// @}

private slots:
	/*!
	*	@name Signal Mappers.
	*	@brief Map part signals to assembly signals.
	*/
	/// @{
	void mapChangedPartOpacity(const double& a);
	void mapChangedPartVisible(const bool& isVisible);
	void mapChangedPartTransformation();
	void mapChangedPartData(fi3d::ModelDataVPtr data);
	void mapChangedPartColor(const double& r, const double& g, const double& b);
	// TODO: What does it mean for an inner part to change its 
	// isHolographic state?
	// TODO: What if someone changes pickability of a part? What does
	// this do to the assembly?
	/// @}

private:
	/// @brief Connects all the part signals to the assembly signals. */
	void connectPartSignals(Model* part);
};

/// @brief Alias for a smart pointer of this class.
using AssemblyPtr = QSharedPointer<Assembly>;

}
