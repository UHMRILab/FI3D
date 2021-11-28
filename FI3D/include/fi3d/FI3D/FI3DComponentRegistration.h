#pragma once
/*!
*	@author		VelazcoJD
*   @file		FI3DComponentRegistration.h
*	@brief		Set of macros used to help components register themselves in 
*				FI3D.
*/

#include <fi3d/FI3D/FI3DController.h>
#include <fi3d/FI3D/FI3D.h>
#include <fi3d/data/Filer.h>

#include <QSharedPointer>

/// Components with no GUI should use these next two macros.

#define FI3D_COMPONENT(m_componentClass) \
friend fi3d::FI3DController; \
private: \
	static QSharedPointer<m_componentClass> INSTANCE; \
	static bool sRegistered; \
	static bool sIsInitiated; \
	static QString sComponentID; \
	static QString sComponentAcronym; \
private slots: \
	static QString getComponentID(); \
	static QString getComponentAcronym(); \
	static QString getAssetsPath(); \
	static QString getDataPath(); \
	static bool init(); \
	static void clean(); 

#define REGISTER_FI3D_COMPONENT(m_componentClass, name, acronym) \
QSharedPointer<m_componentClass> m_componentClass::INSTANCE; \
bool m_componentClass::sRegistered = fi3d::FI3DController::addComponent(name, acronym, &m_componentClass::init, &m_componentClass::clean, Q_NULLPTR);  \
bool m_componentClass::sIsInitiated = false; \
QString m_componentClass::sComponentID = name; \
QString m_componentClass::sComponentAcronym = acronym; \
QString m_componentClass::getComponentID() { \
	return sComponentID; \
} \
QString m_componentClass::getComponentAcronym() { \
	return sComponentAcronym; \
} \
QString m_componentClass::getAssetsPath() { \
	return QObject::tr("%1/components/%2").arg(FI3D::ASSETS_DIRECTORY).arg(sComponentAcronym); \
} \
QString m_componentClass::getDataPath() { \
	return QObject::tr("%1/components/%2").arg(FI3D::DATA_DIRECTORY).arg(sComponentAcronym); \
} \
bool m_componentClass::init() { \
	if (sIsInitiated) { \
		return false; \
	} \
	Filer::checkAndCreateDirectory(m_componentClass::getDataPath()); \
	INSTANCE.reset(new m_componentClass()); \
	sIsInitiated = true; \
	return true; \
} \
void m_componentClass::clean() { \
	INSTANCE.clear(); \
	sIsInitiated = false; \
} 

/// Components that have a GUI should use these two macros instead.

#define FI3D_COMPONENT_WITH_GUI(m_componentClass) \
friend fi3d::FI3DController; \
private: \
	static QSharedPointer<m_componentClass> INSTANCE; \
	static bool sRegistered; \
	static bool sIsInitiated; \
	static QString sComponentID; \
	static QString sComponentAcronym; \
private slots: \
	static QString getComponentID(); \
	static QString getComponentAcronym(); \
	static QString getAssetsPath(); \
	static QString getDataPath(); \
	static bool init(); \
	static void clean(); \
public slots: \
	static void showDialog();

#define REGISTER_FI3D_COMPONENT_WITH_GUI(m_componentClass, name, acronym) \
QSharedPointer<m_componentClass> m_componentClass::INSTANCE; \
bool m_componentClass::sRegistered = fi3d::FI3DController::addComponent(name, acronym, &m_componentClass::init, &m_componentClass::clean, &m_componentClass::showDialog);  \
bool m_componentClass::sIsInitiated = false; \
QString m_componentClass::sComponentID = name; \
QString m_componentClass::sComponentAcronym = acronym; \
QString m_componentClass::getComponentID() { \
	return sComponentID; \
} \
QString m_componentClass::getComponentAcronym() { \
	return sComponentAcronym; \
} \
QString m_componentClass::getAssetsPath() { \
	return QObject::tr("%1/components/%2").arg(FI3D::ASSETS_DIRECTORY).arg(sComponentAcronym); \
} \
QString m_componentClass::getDataPath() { \
	return QObject::tr("%1/components/%2").arg(FI3D::DATA_DIRECTORY).arg(sComponentAcronym); \
} \
bool m_componentClass::init() { \
	if (sIsInitiated) { \
		return false; \
	} \
	Filer::checkAndCreateDirectory(m_componentClass::getDataPath()); \
	INSTANCE.reset(new m_componentClass()); \
	sIsInitiated = true; \
	return true; \
} \
void m_componentClass::clean() { \
	INSTANCE.clear(); \
	sIsInitiated = false; \
}