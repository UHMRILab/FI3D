#pragma once
/*!
* @author	VelazcoJD
* @file     GlobalsDEMO.h
* @brief	Defines global values for the Demonstration (DEMO) module.
*
* Defines global values which are used by the module elements, such
* as ModuleInteraction IDs, Visual IDs, and other common values.
*/

#include <QString>

namespace demo {
/*!
* @name Module Interaction IDs.
* @brief The IDs used for the ModuleInteractions.
*/
/// @{

extern const QString MI_SLICE;
extern const QString MI_PHASE;
extern const QString MI_ENABLE_SURFACE;
extern const QString MI_ANIMATE;

/// @}

/*!
* @name Visual IDs.
* @brief The IDs used for the Visuals.
*/
/// @{

extern const QString VI_ENDOCARDIUM_SURFACE;
extern const QString VI_EPICARDIUM_SURFACE;
extern const QString VI_STUDY_SLICE;

/// @}

}
