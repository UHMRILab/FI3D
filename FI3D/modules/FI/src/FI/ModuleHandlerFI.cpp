#include <FI/ModuleHandlerFI.h>

#include <fi3d/logger/Logger.h>

#include <fi3d/FI3D/FI3D.h>

#include <fi3d/data/DataManager.h>
#include <fi3d/data/Filer.h>

#include <fi3d/server/message_keys/MessageKeys.h>
#include <fi3d/server/message_keys/EApplicationRequest.h>
#include <fi3d/server/message_keys/EModuleRequest.h>

#include <FI/GlobalsFI.h>
#include <FI/data/DataCache.h>

using namespace fi3d;

using namespace fi;

REGISTER_FI3D_MODULE(fi::ModuleHandlerFI, "Framework Interface", "FI")

ModuleHandlerFI::ModuleHandlerFI(const QString& moduleID)
	: ModuleHandler(sModuleName, sModuleAcronym, moduleID),
	mGUI(new ModuleWindowFI()),
	mModuleMenu(new ModuleMenuDialog()),
	mScene(Q_NULLPTR),
	mConnection(new ServerConnection()),
	mSubscribedModuleID(""),
	mIsSynchronized(true)
{
	//Ensure the data folder is created
	Filer::checkAndCreateDirectory(this->getModuleDataPath());
	
	// The data cache
	DataCachePtr cache(new DataCache());

	// Create and set a default scene.
	mScene.reset(new SceneFI(cache, mGUI->getSceneWidget(), moduleID));
	this->setScene(mScene);
	mScene->setAxesVisible(false);
	
	// Set the GUI
	this->setModuleWindow(mGUI);
	
	// Set up the Module Menu.
	this->attachModuleElement(mModuleMenu.data());

	// Set up interactions.
	this->setUpInteractions();

	// Connect connections to get new messages.
	QObject::connect(
		mConnection.data(), &ServerConnection::newApplicationResponse,
		this, &ModuleHandlerFI::onApplicationMessage);
	QObject::connect(
		mConnection.data(), &ServerConnection::newModuleResponse,
		this, &ModuleHandlerFI::onModuleMessage);
	QObject::connect(
		mConnection.data(), &ServerConnection::newDataResponse,
		cache.data(), &DataCache::handleDataMessage);

	// Connect connections to make requests.
	// TODO: Connect scene requests
	QObject::connect(
		cache.data(), &DataCache::dataRequest,
		mConnection.data(), &ServerConnection::sendDataRequest);

	// Handle closing the ModuleMenuDialog window.
	QObject::connect(
		mModuleMenu.data(), &ModuleMenuDialog::dialogClosing,
		this, &ModuleHandlerFI::onModuleMenuDialogClose);

	// Update GUI based on connection events.
	QObject::connect(
		mConnection.data(), &ServerConnection::connected,
		mModuleMenu.data(), &ModuleMenuDialog::updateConnected);
	QObject::connect(
		mConnection.data(), &ServerConnection::disconnected,
		mModuleMenu.data(), &ModuleMenuDialog::updateDisconnected);
	QObject::connect(
		mConnection.data(), &ServerConnection::changedConnectionFeedback,
		mModuleMenu.data(), &ModuleMenuDialog::setConnectionFeedback);

	// Load configuration.
	this->applyConfig();
}

ModuleHandlerFI::~ModuleHandlerFI() {
	// Save configuration
	this->makeConfig();
}

void ModuleHandlerFI::setUpInteractions() {
	// Sets the IP of the FI3D server.
	StringInteractionPtr ipInte(new StringInteraction(MI_IP, ""));
	this->addModuleInteraction(ipInte);
	QObject::connect(
		ipInte.data(), &StringInteraction::changedValue,
		mModuleMenu.data(), &ModuleMenuDialog::updateIP);

	// Sets the port of the FI3D server.
	IntegerInteractionPtr portInte(	new IntegerInteraction(MI_PORT, 0));
	this->addModuleInteraction(portInte);
	QObject::connect(
		portInte.data(), &IntegerInteraction::changedValue,
		mModuleMenu.data(), &ModuleMenuDialog::updatePort);

	// Sets the password of the FI3D server.
	StringInteractionPtr passwordInte(new StringInteraction(MI_PASSWORD, ""));
	this->addModuleInteraction(passwordInte);
	QObject::connect(
		passwordInte.data(), &StringInteraction::changedValue,
		mConnection.data(), &ServerConnection::setConnectionPassword);
	QObject::connect(
		passwordInte.data(), &StringInteraction::changedValue,
		mModuleMenu.data(), &ModuleMenuDialog::updatePassword);

	// Initiates the connection with the FI3D server.
	ValuelessInteractionPtr connectInte(new ValuelessInteraction(MI_CONNECT));
	this->addModuleInteraction(connectInte);
	QObject::connect(
		connectInte.data(), &ValuelessInteraction::triggered,
		this, &ModuleHandlerFI::startFI3DConnection);

	// Disconnects from the FI3D server.
	ValuelessInteractionPtr disconnectInte(new ValuelessInteraction(MI_DISCONNECT));
	this->addModuleInteraction(disconnectInte);
	QObject::connect(
		disconnectInte.data(), &ValuelessInteraction::triggered,
		this, &ModuleHandlerFI::stopFI3DConnection);

	// Activates a module.
	StringInteractionPtr activateModuleInte(new StringInteraction(MI_ACTIVATE_MODULE));
	this->addModuleInteraction(activateModuleInte);
	QObject::connect(
		activateModuleInte.data(), &StringInteraction::changedValue,
		this, &ModuleHandlerFI::activateModule);

	// Stops a running module.
	StringInteractionPtr stopModuleInte(new StringInteraction(MI_STOP_MODULE));
	this->addModuleInteraction(stopModuleInte);
	QObject::connect(
		stopModuleInte.data(), &StringInteraction::changedValue,
		this, &ModuleHandlerFI::stopModule);

	// Subscribes to a module.
	StringInteractionPtr subscribeToModuleInte(new StringInteraction(MI_SUBSCRIBE_TO_MODULE));
	this->addModuleInteraction(subscribeToModuleInte);
	QObject::connect(
		subscribeToModuleInte.data(), &StringInteraction::changedValue,
		this, &ModuleHandlerFI::subscribeToModule);

	// Opens the Module menu.
	ValuelessInteractionPtr openSettings(new ValuelessInteraction(MI_SETTINGS));
	this->addModuleInteraction(openSettings);
	QObject::connect(
		openSettings.data(), &ValuelessInteraction::triggered,
		this, &ModuleHandlerFI::onSettings);

	// Enables/disables synchronization
	BooleanInteractionPtr sync(new BooleanInteraction(MI_SYNCHRONIZE, mIsSynchronized));
	this->addModuleInteraction(sync);
	QObject::connect(
		sync.data(), &BooleanInteraction::changedValue,
		this, &ModuleHandlerFI::onSynchronize);

	// Calls for a refreshal of the Scene
	ValuelessInteractionPtr refreshScene(new ValuelessInteraction(MI_REFRESH_SCENE));
	this->addModuleInteraction(refreshScene);
	QObject::connect(
		refreshScene.data(), &ValuelessInteraction::triggered,
		this, &ModuleHandlerFI::onRefreshScene);
}

void ModuleHandlerFI::closeModule() {
	if (mModuleMenu->isVisible()) {
		return;
	}

	ModuleHandler::closeModule();
}

void ModuleHandlerFI::showModuleWindow() {
	mModuleMenu->show();
}

void ModuleHandlerFI::startFI3DConnection() {
	qDebug() << "Enter";
	
	mConnection->connectToHost(this->getModuleInteractionValue(MI_IP).toString(), 
		this->getModuleInteractionValue(MI_PORT).toInt());

	qDebug() << "Exit";
}

void ModuleHandlerFI::stopFI3DConnection() {
	mConnection->disconnectFromHost();
}

void ModuleHandlerFI::activateModule(const QString& moduleName) {
	QJsonObject request;
	request.insert(ACTION_TYPE, EApplicationRequest::START_MODULE);
	request.insert(MODULE_NAME, moduleName);
	mConnection->sendApplicationRequest(request);
}

void ModuleHandlerFI::stopModule(const QString& moduleID) {
	QJsonObject request;
	request.insert(ACTION_TYPE, EApplicationRequest::STOP_MODULE);
	request.insert(MODULE_ID, moduleID);
	mConnection->sendApplicationRequest(request);
}

void ModuleHandlerFI::subscribeToModule(const QString& moduleID) {
	mSubscribedModuleID = moduleID;

	QJsonObject request{
		{MODULE_ID, moduleID},
		{REQUEST_ID, EModuleRequest::SUBSCRIBE_TO_MODULE}
	};
	mConnection->sendModuleRequest(request);

	if (!mGUI->isVisible()) {
		mGUI->show();
		mGUI->setWindowTitle(tr("%1 - %2").arg(this->getModuleID()).arg(moduleID));
	}
}

void ModuleHandlerFI::onSettings() {
	if (!mModuleMenu->isVisible()) {
		mModuleMenu->show();
	} else if (!mModuleMenu->isActiveWindow()) {
		mModuleMenu->activateWindow();
	}
}

void ModuleHandlerFI::onSynchronize(const bool& isEnabled) {
	// TODO: Set up this feature.
	emit feedbackWarning("This feature is currently not available.");
}

void ModuleHandlerFI::onRefreshScene() {
	QJsonObject request{
		{MODULE_ID, mSubscribedModuleID},
		{REQUEST_ID, EModuleRequest::GET_SCENE}
	};

	mConnection->sendModuleRequest(request);
}

void ModuleHandlerFI::onApplicationMessage(MessagePtr message) {
	qDebug() << "Enter";
	QSharedPointer<QJsonObject> applicationInfo = message->getInfo();
	QJsonArray moduleNamesJson = applicationInfo->value(AVAILABLE_MODULES).toArray();
	QList<QString> moduleNames;
	for (int i = 0; i < moduleNamesJson.count(); i++) {
		QJsonObject availableModule = moduleNamesJson[i].toObject();
		moduleNames.append(availableModule.value(NAME).toString());
	}
	moduleNames.sort();

	QJsonArray activeModulesJson = applicationInfo->value(ACTIVE_MODULES).toArray();
	QList<QString> activeModules;
	for (int i = 0; i < activeModulesJson.count(); i++) {
		QJsonObject activeModule = activeModulesJson[i].toObject();
		activeModules.append(activeModule.value(ID).toString());
	}
	activeModules.sort();

	mModuleMenu->setAvailableModules(moduleNames);
	mModuleMenu->setActiveModules(activeModules);
	qDebug() << "Exit";
}

void ModuleHandlerFI::onModuleMessage(MessagePtr message) {
	QJsonObject moduleInfo = message->getInfo()->value(MODULE_INFO).toObject();

	QJsonArray visualsInfo = moduleInfo.value(VISUALS_INFO).toArray();
	mScene->handleModuleResponse(visualsInfo);

	QJsonArray interactionsInfo = moduleInfo.value(MODULE_INTERACTIONS).toArray();
	qDebug() << "Handling" << interactionsInfo.count() << "module interaction updates";

	for (int i = 0; i < interactionsInfo.count(); i++) {
		QJsonObject interactionInfo = interactionsInfo[i].toObject();

		EModuleResponse responseID(interactionInfo.value(RESPONSE_ID).toInt());
		switch (responseID.toInt()) {
		case EModuleResponse::ADD_MODULE_INTERACTION:
			this->handleAddModuleInteraction(interactionInfo);
			break;
		case EModuleResponse::UPDATE_MODULE_INTERACTION:
			this->handleUpdateModuleInteraction(interactionInfo);
			break;
		case EModuleResponse::REMOVE_MODULE_INTERACTION:
			this->handleRemoveModuleInteraction(interactionInfo);
			break;
		default:
			break;
		}
	}
}

void ModuleHandlerFI::handleAddModuleInteraction(const QJsonObject& interactionJson) {
	QString id = interactionJson.value(ID).toString();
	EModuleInteraction interactionType(interactionJson.value(TYPE).toInt());
	QString info = interactionJson.value(INFO).toString();

	switch (interactionType.toInt()) {
	case EModuleInteraction::VALUELESS:
		{
			ValuelessInteractionPtr interaction(new ValuelessInteraction(id, info));
			this->addModuleInteraction(interaction);
			QObject::connect(
				interaction.data(), &ValuelessInteraction::triggered,
				this, &ModuleHandlerFI::onValuelessInteraction);
		}
		break;
	case EModuleInteraction::BOOL:
		{
			bool value = interactionJson.value(VALUE).toBool();
			BooleanInteractionPtr interaction(new BooleanInteraction(id, value, info));
			this->addModuleInteraction(interaction);
			QObject::connect(
				interaction.data(), &BooleanInteraction::changedValue,
				this, &ModuleHandlerFI::onBooleanInteraction);
		}
		break;
	case EModuleInteraction::INTEGER:
		{
			int value = interactionJson.value(VALUE).toInt();
			EModuleInteractionConstraint constraint(interactionJson.value(CONSTRAINT).toInt());
			IntegerInteractionPtr interaction;
			if (constraint == EModuleInteractionConstraint::MIN) {
				int min = interactionJson.value(MIN_VALUE).toInt();
				interaction = IntegerInteraction::createMinInteraction(id, value, min, info);
			} else if (constraint == EModuleInteractionConstraint::MAX) {
				int max = interactionJson.value(MAX_VALUE).toInt();
				interaction = IntegerInteraction::createMaxInteraction(id, value, max, info);
			} else if (constraint == EModuleInteractionConstraint::RANGE) {
				int min = interactionJson.value(MIN_VALUE).toInt();
				int max = interactionJson.value(MAX_VALUE).toInt();
				interaction = IntegerInteraction::createRangeInteraction(id, value, min, max, info);
			} else {
				interaction.reset(new IntegerInteraction(id, value, info));
			}

			this->addModuleInteraction(interaction);
			QObject::connect(
				interaction.data(), &IntegerInteraction::changedValue,
				this, &ModuleHandlerFI::onIntegerInteraction);
		}
		break;
	case EModuleInteraction::FLOAT:
		{
			double value = interactionJson.value(VALUE).toDouble();
			EModuleInteractionConstraint constraint(interactionJson.value(CONSTRAINT).toInt());
			FloatInteractionPtr interaction;
			if (constraint == EModuleInteractionConstraint::MIN) {
				double min = interactionJson.value(MIN_VALUE).toDouble();
				interaction = FloatInteraction::createMinInteraction(id, value, min, info);
			} else if (constraint == EModuleInteractionConstraint::MAX) {
				double max = interactionJson.value(MAX_VALUE).toDouble();
				interaction = FloatInteraction::createMaxInteraction(id, value, max, info);
			} else if (constraint == EModuleInteractionConstraint::RANGE) {
				double min = interactionJson.value(MIN_VALUE).toDouble();
				double max = interactionJson.value(MAX_VALUE).toDouble();
				interaction = FloatInteraction::createRangeInteraction(id, value, min, max, info);
			} else {
				interaction.reset(new FloatInteraction(id, value, info));
			}

			this->addModuleInteraction(interaction);
			QObject::connect(
				interaction.data(), &FloatInteraction::changedValue,
				this, &ModuleHandlerFI::onFloatInteraction);
		}
		break;
	case EModuleInteraction::STRING:
		{
			QString value = interactionJson.value(VALUE).toString();
			StringInteractionPtr interaction;
			interaction.reset(new StringInteraction(id, value, info));
			
			this->addModuleInteraction(interaction);
			QObject::connect(
				interaction.data(), &StringInteraction::changedValue,
				this, &ModuleHandlerFI::onStringInteraction);
		}
		break;
	case EModuleInteraction::SELECT:
		{
			int value = interactionJson.value(VALUE).toInt();
			SelectInteractionPtr interaction;

			QJsonArray optionsJson = interactionJson.value(VALUE_OPTIONS).toArray();
			QVector<QString> options;
			options.reserve(optionsJson.count());
			for (int i = 0; i < optionsJson.count(); i++) {
				options.append(optionsJson[i].toString());
			}

			interaction.reset(new SelectInteraction(id, options, info));
			interaction->setValue(value);

			this->addModuleInteraction(interaction);
			QObject::connect(
				interaction.data(), &SelectInteraction::changedValue,
				this, &ModuleHandlerFI::onSelectInteraction);
		}
		break;
	default:
		break;
	}
}

void ModuleHandlerFI::handleUpdateModuleInteraction(const QJsonObject& interactionJson) {
	// TODO: Need to take into account when an interaction constraint needs to 
	// be updated as well.

	QString id = interactionJson.value(ID).toString();
	ModuleInteractionPtr interaction = this->getModuleInteraction(id);
	if (!interaction.isNull()) {
		QJsonValue value = interactionJson.value(VALUE);
		interaction->setValueAsJson(value); 
	} else {
		QString feedback = tr("Failed to set value of %1 because the interaction was not found.").arg(id);
		emit feedbackWarning(feedback);
	}
}

void ModuleHandlerFI::handleRemoveModuleInteraction(const QJsonObject& interactionJson) {
	QString id = interactionJson.value(ID).toString();
	this->removeModuleInteraction(id);
}

void ModuleHandlerFI::onValuelessInteraction() {
	ValuelessInteraction* interaction = qobject_cast<ValuelessInteraction*>(sender());
	if (interaction == Q_NULLPTR) {
		qWarning() << "Failed to send Valueless request. Object not found.";
		return;
	}

	QJsonObject request{
		{MODULE_ID, mSubscribedModuleID},
		{REQUEST_ID, EModuleRequest::MODULE_INTERACTION},
		{INTERACTION_ID, interaction->getInteractionID()},
	};

	mConnection->sendModuleRequest(request);
}

void ModuleHandlerFI::onBooleanInteraction(const bool& value) {
	BooleanInteraction* interaction = qobject_cast<BooleanInteraction*>(sender());
	if (interaction == Q_NULLPTR) {
		qWarning() << "Failed to send Boolean interaction request. Object not found.";
		return;
	}

	QJsonObject request{
		{MODULE_ID, mSubscribedModuleID},
		{REQUEST_ID, EModuleRequest::MODULE_INTERACTION},
		{INTERACTION_ID, interaction->getInteractionID()},
		{INTERACTION_VALUE, interaction->getValue()},
	};

	mConnection->sendModuleRequest(request);
}

void ModuleHandlerFI::onIntegerInteraction(const int& value) {
	IntegerInteraction* interaction = qobject_cast<IntegerInteraction*>(sender());
	if (interaction == Q_NULLPTR) {
		qWarning() << "Failed to send Integer interaction request. Object not found.";
		return;
	}

	QJsonObject request{
		{MODULE_ID, mSubscribedModuleID},
		{REQUEST_ID, EModuleRequest::MODULE_INTERACTION},
		{INTERACTION_ID, interaction->getInteractionID()},
		{INTERACTION_VALUE, interaction->getValue()},
	};

	mConnection->sendModuleRequest(request);
}

void ModuleHandlerFI::onFloatInteraction(const float& value) {
	FloatInteraction* interaction = qobject_cast<FloatInteraction*>(sender());
	if (interaction == Q_NULLPTR) {
		qWarning() << "Failed to send Float interaction request. Object not found.";
		return;
	}

	QJsonObject request{
		{MODULE_ID, mSubscribedModuleID},
		{REQUEST_ID, EModuleRequest::MODULE_INTERACTION},
		{INTERACTION_ID, interaction->getInteractionID()},
		{INTERACTION_VALUE, interaction->getValue()},
	};

	mConnection->sendModuleRequest(request);
}

void ModuleHandlerFI::onStringInteraction(const QString& value) {
	StringInteraction* interaction = qobject_cast<StringInteraction*>(sender());
	if (interaction == Q_NULLPTR) {
		qWarning() << "Failed to send String interaction request. Object not found.";
		return;
	}

	QJsonObject request{
		{MODULE_ID, mSubscribedModuleID},
		{REQUEST_ID, EModuleRequest::MODULE_INTERACTION},
		{INTERACTION_ID, interaction->getInteractionID()},
		{INTERACTION_VALUE, interaction->getValue()},
	};

	mConnection->sendModuleRequest(request);
}

void ModuleHandlerFI::onSelectInteraction(const int& value) {
	SelectInteraction* interaction = qobject_cast<SelectInteraction*>(sender());
	if (interaction == Q_NULLPTR) {
		qWarning() << "Failed to send String interaction request. Object not found.";
		return;
	}

	QJsonObject request{
		{MODULE_ID, mSubscribedModuleID},
		{REQUEST_ID, EModuleRequest::MODULE_INTERACTION},
		{INTERACTION_ID, interaction->getInteractionID()},
		{INTERACTION_VALUE, interaction->getValue()},
	};

	mConnection->sendModuleRequest(request);
}

void ModuleHandlerFI::onModuleMenuDialogClose() {
	if (!this->getModuleWindow()->isVisible()) {
		// Emit the signal instead of calling the closeModule function since the closeModule process is overriden.
		emit requestCloseModule();
	}
}

void ModuleHandlerFI::applyConfig() {
	QJsonObject config = this->loadConfig();

	this->setModuleInteractionValue(MI_IP, config.value(CK_IP).toString("localhost"));
	this->setModuleInteractionValue(MI_PORT, config.value(CK_PORT).toInt(9000));
	this->setModuleInteractionValue(MI_PASSWORD, config.value(CK_PASSWORD).toString("admin"));
	this->setModuleInteractionValue(MI_CONNECT, true);
}

void ModuleHandlerFI::makeConfig() {
	QJsonObject config;

	config.insert(CK_IP, this->getModuleInteractionValue(MI_IP));
	config.insert(CK_PORT, this->getModuleInteractionValue(MI_PORT));
	config.insert(CK_PASSWORD, this->getModuleInteractionValue(MI_PASSWORD));

	this->saveConfig(config);
}