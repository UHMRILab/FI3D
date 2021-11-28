#include <fi3d/modules/ModuleFactory.h>

#include <fi3d/logger/Logger.h>

#include <fi3d/modules/ModuleHandler.h>

using namespace fi3d;

QHash<QString, ModuleFactory::ModuleCreator>* ModuleFactory::mModuleCreators;
QHash<QString, QString>* ModuleFactory::mModuleAcronyms;

ModuleFactory::ModuleFactory()
	: QObject()
{}

ModuleFactory::~ModuleFactory() {
	// TODO: May need to delete the 2 hash pointers.
}

bool ModuleFactory::registerModule(const QString& moduleName, 
	const QString& moduleAcronym, ModuleFactory::ModuleCreator creatorFunction) 
{
    if (mModuleCreators == Q_NULLPTR) {
        mModuleCreators = new QHash<QString, ModuleFactory::ModuleCreator>();
    }
    if (mModuleAcronyms == Q_NULLPTR) {
        mModuleAcronyms = new QHash<QString, QString>();
    }
    
	if (mModuleCreators->contains(moduleName)) {
		qWarning() << "Module" << moduleName << "Failed to register because another module already uses that name.";
		return false;
	}

	if (mModuleCreators->contains(moduleAcronym)) {
		qWarning() << "Module" << moduleName << "Failed to register because" <<
			"another module already uses the acronym:" << moduleAcronym;
		return false;
	}

	if (creatorFunction == Q_NULLPTR) {
		qWarning() << "Module" << moduleName << "Failed to register. Its creator function cannot be null.";
		return false;
	}

	mModuleCreators->insert(moduleName, creatorFunction);
	mModuleAcronyms->insert(moduleName, moduleAcronym);
	return true;
}

ModuleHandlerPtr ModuleFactory::createModule(const QString& moduleName, 
	const QString& moduleID) 
{
    if (mModuleCreators == Q_NULLPTR) {
        mModuleCreators = new QHash<QString, ModuleFactory::ModuleCreator>();
    }
    if (mModuleAcronyms == Q_NULLPTR) {
        mModuleAcronyms = new QHash<QString, QString>();
    }
    
    if (mModuleCreators->contains(moduleName)) {
		return mModuleCreators->value(moduleName)(moduleID);
	}
	return Q_NULLPTR;
}

QList<QString> ModuleFactory::getListOfAvailableModules() {
    if (mModuleCreators == Q_NULLPTR) {
        mModuleCreators = new QHash<QString, ModuleFactory::ModuleCreator>();
    }
    if (mModuleAcronyms == Q_NULLPTR) {
        mModuleAcronyms = new QHash<QString, QString>();
    }
	return mModuleCreators->keys();
}

QString ModuleFactory::getModuleAcronym(const QString& moduleName) {
	return mModuleAcronyms->value(moduleName, "");
}

bool ModuleFactory::isModuleExistent(const QString& moduleName) {
    if (mModuleCreators == Q_NULLPTR) {
        mModuleCreators = new QHash<QString, ModuleFactory::ModuleCreator>();
    }
    if (mModuleAcronyms == Q_NULLPTR) {
        mModuleAcronyms = new QHash<QString, QString>();
    }
    
	if (mModuleCreators->contains(moduleName)) {
		return true;
	}
	return false;
}
