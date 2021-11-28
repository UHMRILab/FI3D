#include <fi3d/FI3D/ApplicationMessageEncoder.h>

#include <fi3d/logger/Logger.h>

#include <fi3d/server/message_keys/MessageKeys.h>
#include <fi3d/server/message_keys/EApplicationRequest.h>
#include <fi3d/server/message_keys/EResponseStatus.h>

#include <fi3d/modules/ModuleFactory.h>

#include <QJsonArray>
#include <QVariantList>

using namespace fi3d;

ApplicationMessageEncoder::ApplicationMessageEncoder()
	: MessageEncoder(EMessage::APPLICATION),
	mActiveModules()
{
}

ApplicationMessageEncoder::~ApplicationMessageEncoder() {}

void ApplicationMessageEncoder::addActiveModule(const QString& moduleID, const QString& moduleName) {
	mActiveModules.insert(moduleID, moduleName);
	QJsonObject response = this->prepareApplicationResponse();
	this->sendGlobalMessage(response);
}

void ApplicationMessageEncoder::removeActiveModule(const QString& moduleID) {
	mActiveModules.remove(moduleID);
	QJsonObject response = this->prepareApplicationResponse();
	this->sendGlobalMessage(response);
}

void ApplicationMessageEncoder::parseRequest(
	const QJsonObject& applicationParams, const QString& clientID, 
	const QString& message) 
{
	qDebug() << "Enter";
	/*if (!message.isEmpty()) {
		emit feedbackGlobal(tr("%1 Message: %2").arg(clientID).arg(message));
	}*/

	EApplicationRequest actionType = 
		applicationParams.value(ACTION_TYPE).toInt(0);
	qDebug() << ACTION_TYPE << actionType.getName();
	switch (actionType.toInt()) {
		case EApplicationRequest::START_MODULE:
			parseStartModuleRequest(applicationParams, clientID);
			break;
		case EApplicationRequest::STOP_MODULE:
			parseStopModuleRequest(applicationParams, clientID);
			break;
		case EApplicationRequest::LOG:
			qInfo() << "Client Log:" << clientID << "|" << applicationParams.value("Log").toString();
			break;
	}
	qDebug() << "Exit";
}

void ApplicationMessageEncoder::parseStartModuleRequest(
	const QJsonObject& applicationParams, const QString& clientID) 
{
	QString moduleName = applicationParams.value(MODULE_NAME).toString("");
	emit changeStartModule(moduleName, moduleName);
}

void ApplicationMessageEncoder::parseStopModuleRequest(const QJsonObject& applicationParams, const QString& clientID) {
	QString moduleID = applicationParams.value(MODULE_ID).toString("");
	emit changeStopModule(moduleID);
}

void ApplicationMessageEncoder::onConnectedClient(const QString& ClientID) {
	qDebug() << "Enter";

	QJsonObject response = this->prepareApplicationResponse();
	this->sendMessage(response, ClientID);

	qDebug() << "Exit";
}

QJsonObject ApplicationMessageEncoder::prepareApplicationResponse() {
	QJsonObject response;
	response.insert(RESPONSE_STATUS, EResponseStatus::SUCCESS);
	response.insert(MESSAGE_TYPE, mMessageEncoderType.toInt());
	
	QList<QString> availableModules = ModuleFactory::getListOfAvailableModules();
	availableModules.sort();
	QVariantList availableModulesJSON;
	availableModulesJSON.reserve(availableModules.count());
	for (int i = 0; i < availableModules.size(); i++) {
		QJsonObject availableModule;
		availableModule.insert(NAME, availableModules[i]);
		availableModule.insert(ACRONYM, ModuleFactory::getModuleAcronym(availableModules[i]));

		availableModulesJSON.append(availableModule);
	}
	response.insert(AVAILABLE_MODULES, QJsonArray::fromVariantList(availableModulesJSON));

	QHash<QString, QString>::iterator it = mActiveModules.begin();
	QVariantList activeModules;
	activeModules.reserve(mActiveModules.count());
	for (; it != mActiveModules.end(); it++) {
		QJsonObject activeModule;
		activeModule.insert(ID, it.key());
		activeModule.insert(NAME, it.value());
		activeModules.push_back(activeModule);
	}
	response.insert(ACTIVE_MODULES, QJsonArray::fromVariantList(activeModules));

	return response;
}
