#pragma once
/*!
* @author	VelazcoJD
* @file		Visual.h
* @class	fi3d::Visual 
* @brief	Interface to a Visual. All Visual instances have at least these 
*			methods.
*/

#include <fi3d/rendering/visuals/EVisual.h>

#include <QObject>

#include <vtkProp.h>

namespace fi3d {
class Visual : public QObject {

	Q_OBJECT

signals:
	/// @brief Emitted when Visual's holographic state has changed.
	void changedHolographicState(const bool& isHolographic);

	/// @brief Emitted when the Visual's opacity has changed.
	void changedOpacity(const double& a);

	/// @brief Emitted when the Visual's visibility has changed.
	void changedVisibleState(const bool& isVisible);

	/// @brief Emitted when the Visual's pickability has changed.
	void changedPickableState(const bool& isPickable);

	/// @brief Emitted when the Visual's interactable state has changed.
	void changedInteractableState(const bool& isInteractable);

private:
	/// @brief The visual's ID.
	QString mID;

	/// @brief Whether the Visual is a 3D object or not.
	bool mIs3D;

	/// @brief Whether the Visual should be sent to holographic devices.
	bool mIsHolographic;

	/// @brief Whether the Visual can be interacted through user input.
	bool mIsInteractable;

public:
	/*! 
		* @brief Constructor.
		*
		* @param id The id for the Visual.
		* @param is3D Whether this Visual is a 3D visual.
		*/
	Visual(const QString& id, const bool& is3D);

	/// @brief Destructor.
	~Visual();

public slots:
	/// @brief Set whether this Visual is holographic or not.
	virtual void setHolographic(const bool& isHolographic);

	/// @brief Set the Visual's opacity.
	virtual void setOpacity(const double& a) = 0;

	/// @brief Set whether the Visual should be visible or not.
	virtual void setVisible(const bool& isVisible) = 0;

	/// @brief Set whether the Visual can be picked in the scene.
	virtual void setPickable(const bool& isPickable) = 0;

	/*! 
		* @brief Sets whether the object is interactable. 
		* 
		* If interactable is set to true, pickable will be set to true.
		* See isPickable for more info.
		*/
	virtual void setInteractable(const bool& isInteractable);

public:
	/// @brief Gets the ID of the visual.
	QString getVisualID() const;

	/// @brief Gets the type this Visual is.
	virtual EVisual getVisualType() const = 0;
				
	/// @brief Get the VTK object that renders the data.
	virtual vtkProp* getActor() = 0;
			
	/// @brief Get whether this Visual is a 3D Visual or not.
	bool is3D() const;

	/// @brief Get whether this Visual should be sent as a hologram.
	virtual bool isHolographic() const;

	/// @brief Gets the Visual's opacity.
	virtual double getOpacity() const = 0;

	/// @brief Gets whether the Visual is visible or invisible.
	virtual bool isVisible() const = 0;

	/*! 
		* @brief Gets whether the Visual is pickable or non-pickable. 
		*
		* Pickable visuals can be "selected" in the scene. This is 
		* necessary to identify objects by user input. This is different
		* than the isInteractable because interactable objects can be
		* translated and rotated through user input. Pickable objects can
		* only be selected.
		*/
	virtual bool isPickable() const = 0;

	/*! 
		* @brief Gets whether the Visual is interactable. 
		*
		* Interactable visuals can be translated and rotated in the scene.
		* Note that for interactable visuals to be interacted with, they 
		* must also be pickable. See isPickable for information on the 
		* difference between the two.			
		*/
	virtual bool isInteractable() const;
};

/// @brief Alias for a smart pointer of this class.
using VisualPtr = QSharedPointer<Visual>;

}
