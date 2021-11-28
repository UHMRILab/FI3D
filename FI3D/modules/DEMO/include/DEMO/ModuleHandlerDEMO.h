#pragma once
/*!
* @author	VelazcoJD
* @file		ModuleHandlerDEMO.h
* @class	demo::ModuleHandlerDEMO
* @brief	Handles the Demonstration (DEMO) module.
*
* TODO: Add information on what this module does.
*/

#include <fi3d/modules/ModuleHandler.h>

#include <fi3d/modules/ModuleRegistration.h>

#include <DEMO/ModuleWindowDEMO.h>

namespace demo {
class ModuleHandlerDEMO : public fi3d::ModuleHandler {

	Q_OBJECT
	FI3D_MODULE

private:
	/// @brief The module's GUI.
	ModuleWindowDEMOPtr mGUI;

	/// @breif The rendered Slice.
	fi3d::AnimatedStudySlicePtr mSlice;

	/// @brief The epicardium and endocardium.
	fi3d::AnimatedModelPtr mEndocardium, mEpicardium;

	/// @breif Timer which handles the animation of the renderings. 
	QSharedPointer<QTimer> mDataAnimationTimer;

	/// @brief The speed, in milliseconds, of the animation.
	int mAnimationSpeed;

public:
	/// @brief Constructor.
	ModuleHandlerDEMO(const QString& moduleID);

	/// @brief Destructor.
	~ModuleHandlerDEMO();

public slots:
	/// @brief Override the showModuleWindow to change the window not in 
	/// full screen.
	void showModuleWindow() override;

	/// @brief Starts/Stops the animation of the cardiac data.
	void setAnimationEnable(const bool& isEnable);

private slots:
	/// @brief Updates the GUI with latest connection information.
	void updateConnectionNumbers();

private:
	/// @brief Setups all the interactions.
	void setUpInteractions();

	/// @brief Setups all connections between the module elements.
	void setUpConnections();

protected slots:
	/// <summary>
	/// Loads config file and applies saved defaults.
	/// </summary>
	void applyConfig();
	
	/// <summary>
	/// Makes config file and saves to disk.
	/// </summary>
	void makeConfig();
};

/// @brief Alias for a smart pointer of this class.
using ModuleHandlerDEMOPtr = QSharedPointer<ModuleHandlerDEMO>;

}