#pragma once
/*!
* @author	VelazcoJD
* @file		ModuleRegistration.h
* @brief	Macros used for module registration.
*/

#include <fi3d/modules/ModuleHandler.h>

/// @brief Include this macro in your class declaration.
#define FI3D_MODULE \
static bool sRegistered; \
static QString sModuleName; \
static QString sModuleAcronym; \
static fi3d::ModuleHandlerPtr \
	createModule(const QString& moduleID); 

/// @brief Include this macro in your class definition.
#define REGISTER_FI3D_MODULE(m_Module, m_ModuleName, m_ModuleAcronym) \
bool m_Module::sRegistered = ModuleFactory::registerModule( \
	m_ModuleName, m_ModuleAcronym, &m_Module::createModule); \
\
QString m_Module::sModuleName = m_ModuleName; \
QString m_Module::sModuleAcronym = m_ModuleAcronym; \
\
fi3d::ModuleHandlerPtr m_Module::createModule( \
	const QString& moduleID) \
{ \
	QSharedPointer<m_Module> md(new m_Module(moduleID)); \
	return md; \
} 