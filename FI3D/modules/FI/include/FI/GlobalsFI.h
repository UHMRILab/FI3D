#pragma once
/*!
* @author	VelazcoJD
* @file     GlobalsFI.h
* @brief	Defines global values needed for the module.
*
* Defines global variables which are used by the module elements, such
* as ModuleInteraction IDs, Visual IDs, and other common values.
*/

#include <QObject>

namespace fi {
/*!
* @name Module Interaction IDs.
* @brief The IDs used for the ModuleInteractions.
*/
/// @{
extern const QString MI_IP;
extern const QString MI_PORT;
extern const QString MI_PASSWORD;
extern const QString MI_CONNECT;
extern const QString MI_DISCONNECT;
extern const QString MI_ACTIVATE_MODULE;
extern const QString MI_STOP_MODULE;
extern const QString MI_SUBSCRIBE_TO_MODULE;
extern const QString MI_SETTINGS;
extern const QString MI_SYNCHRONIZE;
extern const QString MI_REFRESH_SCENE;
/// @}

/*!
* @name Config Keys.
* @brief Keys used in the config file.
*/
/// @{
extern const QString CK_IP;
extern const QString CK_PORT;
extern const QString CK_PASSWORD;
/// @}

}
