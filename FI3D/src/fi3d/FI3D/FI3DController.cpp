#include <fi3d/FI3D/FI3DController.h>

#include <fi3d/logger/Logger.h>

#include <fi3d/FI3D/FI3D.h>
#include <fi3d/FI3D/ApplicationMessageEncoder.h>
#include <fi3d/FI3D/FI3DControllerWindow.h>

#include <fi3d/utilities/Icons.h>

#include <fi3d/server/Server.h>

#include <QGuiApplication>
#include <QIcon>
#include <QMenuBar>

using namespace fi3d;

/// Static methods available for the system to use.
QSharedPointer<FI3DController> FI3DController::INSTANCE;

bool FI3DController::sIsInitiated = false;

QHash<QString, FI3DController::ComponentInformation>* FI3DController::sComponents;

QHash<QString, FI3DController::ComponentInformation>* FI3DController::sComponentsWithGUI;

void FI3DController::init() {
	if (FI3DController::sIsInitiated) {
		return;
	}

	QIcon icon = Icons::getIcon(EIcon::BRAND);
	QGuiApplication::setWindowIcon(icon);

	// The server is ran manually first because it is needed by other 
	// components. When the init function is called in the next loop. It will 
	// do nothing as it only initializes once.
	Server::init();

	// Run all the registered initiators
	if (sComponents != Q_NULLPTR) {
		QHash<QString, ComponentInformation>::iterator it = sComponents->begin();
		for (; it != sComponents->end(); it++) {
			if (it.value().initiator != Q_NULLPTR) {
				it.value().initiator();
			}
		}
	}

	if (sComponentsWithGUI != Q_NULLPTR) {
		QHash<QString, ComponentInformation>::iterator it = sComponentsWithGUI->begin();
		for (; it != sComponentsWithGUI->end(); it++) {
			if (it.value().initiator != Q_NULLPTR) {
				it.value().initiator();
			}
		}
	}

	INSTANCE.reset(new FI3DController());

	// Create and set the message encoder
	INSTANCE->mMessageEncoder.reset(new ApplicationMessageEncoder());
	QObject::connect(
		INSTANCE.data(), &FI3DController::startedModule, 
		INSTANCE->mMessageEncoder.data(), &ApplicationMessageEncoder::addActiveModule);
	QObject::connect(
		INSTANCE.data(), &FI3DController::stoppedModule, 
		INSTANCE->mMessageEncoder.data(), &ApplicationMessageEncoder::removeActiveModule);
	QObject::connect(
		INSTANCE->mMessageEncoder.data(), &ApplicationMessageEncoder::changeStartModule, 
		INSTANCE.data(), &FI3DController::activateModule);
	QObject::connect(
		INSTANCE->mMessageEncoder.data(), &ApplicationMessageEncoder::changeStopModule, 
		INSTANCE.data(), &FI3DController::stopModule);

	QString windowTitle = "Framework for Immersive Interaction into Imaging Data";
	INSTANCE->mGUI.reset(new FI3DControllerWindow());
	INSTANCE->mGUI->setWindowTitle(windowTitle);
	INSTANCE->mGUI->setModules(ModuleFactory::getListOfAvailableModules());

	if (sComponentsWithGUI != Q_NULLPTR) {
		INSTANCE->mGUI->setComponents(sComponentsWithGUI->keys());
	}

	QObject::connect(
		INSTANCE->mGUI.data(), &FI3DControllerWindow::componentButtonClicked,
		&FI3DController::showComponentGUI);

	QObject::connect(
		INSTANCE->mGUI.data(), &FI3DControllerWindow::moduleButtonClicked,
		&FI3DController::activateModule);

	FI3DController::sIsInitiated = true;
}

void FI3DController::clean() {
	INSTANCE.clear();

	// Run all the registered cleaners
	if (sComponents != Q_NULLPTR) {
		QHash<QString, ComponentInformation>::iterator it = sComponents->begin();
		for (; it != sComponents->end(); it++) {
			if (it.value().cleaner != Q_NULLPTR) {
				it.value().cleaner();
			}
		}
	}

	if (sComponentsWithGUI != Q_NULLPTR) {
		QHash<QString, ComponentInformation>::iterator it = sComponentsWithGUI->begin();
		for (; it != sComponentsWithGUI->end(); it++) {
			if (it.value().cleaner != Q_NULLPTR) {
				it.value().cleaner();
			}
		}
	}

	sIsInitiated = false;
}

void FI3DController::activateModule(const QString& modName, const QString& moduleID) {
	qDebug() << "Enter";

	QString modID = moduleID;
	INSTANCE->verifyModuleID(modID);

	ModuleHandlerPtr modHandler = ModuleFactory::createModule(modName, modID);
	if (!modHandler.isNull()) {
		INSTANCE->setupModule(modHandler);
	} else {
		qWarning() << "Failed to activate module. The given Module Name does not match a module." <<
			"Module Name given = " << modName;
	}

	if (INSTANCE->mGUI->isVisible()) {
		INSTANCE->mGUI->close();
	}

	qDebug() << "Exit";
}

bool FI3DController::addComponent(const QString& componentID, 
	const QString& acronym, Initiator initiator, Cleaner cleaner, Dialoger dialoger) 
{
	if (sComponents == Q_NULLPTR) {
		sComponents = new QHash<QString, ComponentInformation>();
	}

	if (sComponentsWithGUI == Q_NULLPTR) {
		sComponentsWithGUI = new QHash<QString, ComponentInformation>();
	}

	FI3DController::ComponentInformation newInitiator {
		componentID, acronym, initiator, cleaner, dialoger
	};

	if (dialoger == Q_NULLPTR) {
		sComponents->insert(componentID, newInitiator);
	} else {
		sComponentsWithGUI->insert(componentID, newInitiator);
	}

	return true;
}

void FI3DController::showComponentGUI(const QString& componentID) {
	if (sComponentsWithGUI->contains(componentID)) {
		sComponentsWithGUI->value(componentID).dialoger();
	}
}

void FI3DController::stopModule(const QString& moduleID) {
	qDebug() << "Enter";

	ModuleHandlerPtr module = INSTANCE->mActiveModules.take(moduleID);
	if (module.isNull()) {
		return;
	}

	if (module == INSTANCE->mFocusedModule) {
		INSTANCE->mFocusedModule.clear();
	}

	emit INSTANCE->stoppedModule(module->getModuleID(), module->getModuleName());

	QObject::disconnect(
		module.data(), &ModuleHandler::feedbackGlobal,
		INSTANCE.data(), &FI3DController::handleGlobalFeedback);

	module.clear();
	qInfo() << "Module instance with ID:" << moduleID << "has been stopped";

	qDebug() << "Exit";
}

void FI3DController::setModuleFocus(const QString& moduleID) {
	qDebug() << "Enter";

	if (!INSTANCE->mFocusedModule.isNull()) {
		if (INSTANCE->mFocusedModule->getModuleID() != moduleID) {
			qDebug() << "Pausing module:" << INSTANCE->mFocusedModule->getModuleID();
			INSTANCE->mFocusedModule->pause();
		} else {
			return;
		}
	}

	INSTANCE->mFocusedModule = INSTANCE->mActiveModules.value(moduleID, Q_NULLPTR);

	if (!INSTANCE->mFocusedModule.isNull()) {
		qDebug() << "Resuming module:" << INSTANCE->mFocusedModule->getModuleID();
		INSTANCE->mFocusedModule->resume();
	}

	qDebug() << "Exit";
}

void FI3DController::showMainWindow() {
	// Check if there are no active modules and there is only one available
	// module. If so, start that module immediately insead of showing
	// the FI3D menu.

	if (INSTANCE->mActiveModules.count() == 0 && ModuleFactory::getListOfAvailableModules().count() == 1) {
		QString moduleName = ModuleFactory::getListOfAvailableModules().at(0);
		FI3DController::activateModule(moduleName, moduleName);
	} else if (!INSTANCE->mGUI->isVisible()) {
		INSTANCE->mGUI->show();
	}
}

const FI3DController* FI3DController::getInstance() {
	return INSTANCE.data();
}

/// Singleton members
FI3DController::FI3DController()
	: QObject(),
	mGUI(),
	mActiveModules(),
	mFocusedModule(),
	mMessageEncoder()
{
}

FI3DController::~FI3DController() {}

void FI3DController::setupModule(ModuleHandlerPtr newModule) {
	qDebug() << "Enter";

	if (!mFocusedModule.isNull()) {
		qDebug() << "Pausing module:" << INSTANCE->mFocusedModule->getModuleID();
		mFocusedModule->pause();
	}

	mFocusedModule = newModule;
	qDebug() << "Focusing on new module:" << INSTANCE->mFocusedModule->getModuleID();
	
	// Add framework controls to the toolbar.
	ModuleWindowPtr gui = newModule->getModuleWindow();
	if (!gui.isNull()) {		
		gui->menuBar()->addSeparator();

		QMenu* componentMenu = gui->menuBar()->addMenu("Components");
		QMenu* moduleMenu = gui->menuBar()->addMenu("Modules");

		qInfo() << "Component count:" << sComponents->count();

		QList<QString> comps = sComponentsWithGUI->keys();
		comps.sort();
		for (int i = 0; i < comps.count(); i++) {
			QAction* comp = componentMenu->addAction(comps[i]);

			QObject::connect(
				comp, &QAction::triggered,
				sComponentsWithGUI->value(comps[i]).dialoger);
		}

		QList<QString> moduleTypes = ModuleFactory::getListOfAvailableModules();
		moduleTypes.sort();
		for (QString moduType : moduleTypes) {
			QAction* modu = moduleMenu->addAction(moduType);

			QObject::connect(
				modu, &QAction::triggered,
				this, &FI3DController::activateModuleByAction,
				Qt::UniqueConnection);
		}
		 
		// Connect the GUI close event to stop the module.
		QObject::connect(
			newModule.data(), &ModuleHandler::requestCloseModule,
			this, &FI3DController::closeModuleByModuleRequest,
			Qt::UniqueConnection);

		// Activate the GUI
		newModule->showModuleWindow();
	}

	// Connect to global feedback signal.
	QObject::connect(
		newModule.data(), &ModuleHandler::feedbackGlobal,
		this, &FI3DController::handleGlobalFeedback,
		Qt::UniqueConnection);

	// Add module to list of active modules
	mActiveModules.insert(newModule->getModuleID(), newModule);
	emit startedModule(newModule->getModuleID(), newModule->getModuleName());
	
	qDebug() << "Exit";
}

bool FI3DController::verifyModuleID(QString& moduleID) {
	qDebug() << "Enter - Verifying name:" << moduleID;

	bool nameModified = false;
	if (moduleID.isEmpty()) {
		moduleID = "NO_ID";
		nameModified = true;
	}

	bool uniqueName = false;

	// TODO: What if every possible name is taken? Realistically never happens, 
	quint64	id = 0;
	while (!uniqueName) {						
		if (mActiveModules.contains(moduleID)) {
			moduleID = tr("%1 %2").arg(moduleID).arg(id++);
			nameModified = true;
		} else {
			uniqueName = true;
		}
	}

	qDebug() << "Exit - Name out:" << moduleID;
	return nameModified;
}

void FI3DController::activateModuleByAction() {
	QAction* action = qobject_cast<QAction*>(sender());

	if (action == Q_NULLPTR) {
		qWarning() << "Failed to activate module by action. Not an Action.";
		return;
	}

	QString moduleType = action->text();
	FI3DController::activateModule(moduleType, moduleType);
}

void FI3DController::closeModuleByModuleRequest() {
	ModuleHandler* modu = qobject_cast<ModuleHandler*>(sender());
	if (modu == Q_NULLPTR) {
		qWarning() << "Failed to close module. Unkonwn object request.";
		return;
	}

	stopModule(modu->getModuleID());
}

void FI3DController::handleGlobalFeedback(const QString& feedback) {
	ModuleHandler* modu = qobject_cast<ModuleHandler*>(sender());
	if (modu == Q_NULLPTR) {
		qWarning() << "Failed to handle global feedback, sender is not module";
		return;
	}

	QString msg = tr("[%1] Message: %2").arg(modu->getModuleID()).arg(feedback);

	QHash<QString, ModuleHandlerPtr>::iterator it = mActiveModules.begin();
	for (; it != mActiveModules.end(); it++) {
		emit it.value()->feedbackColor(msg, Qt::GlobalColor::blue);
	}
}
