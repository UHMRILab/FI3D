#include <fi3d/modules/ModuleMessageEncoder.h>

#include <fi3d/logger/Logger.h>

#include <fi3d/server/message_keys/MessageKeys.h>

#include <fi3d/data/EData.h>

#include <QJsonArray>
#include <QVariantList>

using namespace fi3d;

ModuleMessageEncoder::ModuleMessageEncoder()
	: MessageEncoder(EMessage::MODULE),
	ModuleElement(),
	mSubscriberList(),
	mScene(Q_NULLPTR),
	mSubscriberUpdateTimer(),
	mSceneUpdates()
{
	// TODO: What is the optimal value here? Determine proper value.
	mSubscriberUpdateTimer.setInterval(60);

	QObject::connect(
		&mSubscriberUpdateTimer, &QTimer::timeout,
		this, &ModuleMessageEncoder::sendSceneUpdates);
}

ModuleMessageEncoder::~ModuleMessageEncoder() {}

void ModuleMessageEncoder::moduleInformationAssigned(ModuleInformationPtr moduleInfo) {
	if (moduleInfo == Q_NULLPTR) {
		return;
	}

	QObject::connect(
		moduleInfo.data(), &ModuleInformation::changedAddedInteraction,
		this, &ModuleMessageEncoder::prepareAddInteraction,
		Qt::UniqueConnection);
	QObject::connect(
		moduleInfo.data(), &ModuleInformation::changedRemovedInteraction,
		this, &ModuleMessageEncoder::prepareRemoveInteraction,
		Qt::UniqueConnection);

	QList<ModuleInteractionPtr> interactions = moduleInfo->getModuleInteractions();
	for (ModuleInteractionPtr inte : interactions) {
		QObject::connect(
			inte.data(), &ModuleInteraction::changedInteractionValue,
			this, &ModuleMessageEncoder::prepareChangeInteractionValue,
			Qt::UniqueConnection);
		QObject::connect(
			inte.data(), &ModuleInteraction::changedInteractionConstraint,
			this, &ModuleMessageEncoder::prepareChangeInteractionConstraint,
			Qt::UniqueConnection);
	}
}

void ModuleMessageEncoder::parseRequest(const QJsonObject& request, const QString& clientID, const QString& message) {
	qDebug() << "Enter - New request:" << clientID;
	
	// If the message does not belong to this module, ignore the message.
	QString moduleID = request.value(MODULE_ID).toString("");
	if (moduleID != this->getModuleID()) {
		qDebug() << "Exit - Request belongs to" << moduleID << "and not" << this->getModuleID();
		return;
	}

	int requestId = request.value(REQUEST_ID).toInt(0);

	if (this->isRequestBad(request, clientID)) return;

	if (!message.isEmpty()) {
		emit feedbackColor(tr("Message from client [%1]: %2").arg(clientID).arg(message), Qt::GlobalColor::darkGreen);
	}

	// call the adequate function based on the action type
	switch (requestId) {
		case EModuleRequest::MODULE_INTERACTION:
			this->parseModuleInteraction(request, clientID);
			break;
		case EModuleRequest::SUBSCRIBE_TO_MODULE:
			this->parseSubscribeToModule(request, clientID);
			break;
		case EModuleRequest::UNSUBSCRIBE_TO_MODULE:
			this->parseUnbuscribeToModule(request, clientID);
			break;
		case EModuleRequest::GET_SCENE:
			this->parseGetScene(request, clientID);
			break;
		case EModuleRequest::GET_VISUAL:
			this->parseGetobject(request, clientID);
			break;
		case EModuleRequest::HIDE_VISUAL:
			this->parseHideObject(request, clientID);
			break;
		case EModuleRequest::TRANSLATE_VISUAL:
			this->parseTranslateObject(request, clientID);
			break;
		case EModuleRequest::ROTATE_VISUAL:
			this->parseRotateObject(request, clientID);
			break;
		case EModuleRequest::SELECT_SLICE:
			this->parseSelectSlice(request, clientID);
			break;
		default:
			break;
	}
	qDebug() << "Exit";
}

void ModuleMessageEncoder::onDisconnectedClient(const QString& clientID) {
	qDebug() << "Enter";
	if (mSubscriberList.contains(clientID)) {
		mSubscriberList.removeOne(clientID);
		qInfo() << "Subscriber=" << clientID << "has been removed";

		if (mSubscriberList.isEmpty()) {
			mSubscriberUpdateTimer.stop();
		}
	}
	qDebug() << "Exit";
}

bool ModuleMessageEncoder::isRequestBad(const QJsonObject& params, const QString& clientID) {
	int requestId = params.value(REQUEST_ID).toInt(0);

	// This does not mean the request is bad, but because there's no scene,
	// there's nothing to request.
	if (mScene.isNull()) {
		this->sendErrorMessage("No scene is available", clientID);
		return true;
	}

	if (!mSubscriberList.contains(clientID) && requestId != EModuleRequest::SUBSCRIBE_TO_MODULE) {
		this->sendErrorMessage("Must subscribe to module first", clientID);
		return true;
	}

	switch (requestId) {
		// There is nothing to check for these kind of messages
		case EModuleRequest::SUBSCRIBE_TO_MODULE:
		case EModuleRequest::MODULE_INTERACTION:
		case EModuleRequest::UNSUBSCRIBE_TO_MODULE:
		case EModuleRequest::GET_SCENE:
			return false;
		case EModuleRequest::HIDE_VISUAL:
			if (!params.contains(IS_VISIBLE) || !params.value(IS_VISIBLE).isBool()) {
				this->sendErrorMessage("No visibility information was given", clientID);
				return true;
			}
			break;
		case EModuleRequest::TRANSLATE_VISUAL:
			if (!params.contains(TRANSLATE) || 
				!params.value(TRANSLATE).isArray() || 
				params.value(TRANSLATE).toArray().size() != 3) 
			{
				this->sendErrorMessage("No translate information was given", clientID);
				return true;
			}
			break;
		case EModuleRequest::ROTATE_VISUAL:
			if (!params.contains(ROTATE) || 
				!params.value(ROTATE).isArray() ||
				params.value(ROTATE).toArray().size() != 3)
			{
				this->sendErrorMessage("No rotate information was given", clientID);
				return true;
			}
			break;
		case EModuleRequest::SELECT_SLICE:
			if (!params.contains(SLICE_INDEX)) {
				this->sendErrorMessage("No slice index information was given", clientID);
				return true;
			}
			break;
		case EModuleRequest::SELECT_ORIENTATION:
			if (!params.contains(SLICE_ORIENTATION)) {
				this->sendErrorMessage("No slice orientation information was given", clientID);
				return true;
			}
			break;
		case EModuleRequest::SELECT_SERIES:
			if (!params.contains(SERIES_INDEX)) {
				this->sendErrorMessage("No series information was given", clientID);
				return true;
			}
			break;
	}

	if (!params.contains(TARGET_VISUAL) || params.value(TARGET_VISUAL).toString("").isEmpty()) {
		this->sendErrorMessage("No visual object was given", clientID);
		return true;
	}

	return false;
}

void ModuleMessageEncoder::parseModuleInteraction(const QJsonObject& params, const QString& clientID) {
	QString inID = params.value(INTERACTION_ID).toString();
	QJsonValue inValue = params.value(INTERACTION_VALUE);

	qDebug() << "Module Interaction - ID:" << inID << "Value:" << inValue;

	ModuleInteractionPtr inte = this->getModuleInteraction(inID);
	if (inte.isNull()) {
		qWarning() << "Received interaction request, but the interaction" << inID << "was not found.";
		return;
	}

	inte->setValueAsJson(inValue);
}

void ModuleMessageEncoder::parseSubscribeToModule(const QJsonObject& params, const QString& clientID) {
	qDebug() << "Enter - Client:" << clientID << "has subscribed to module";

	// Add client to module subscribers
	if (mSubscriberList.contains(clientID)) {
		qInfo() << "Client:" << clientID << " tried to subscribe to=" << 
			this->getModuleID() << "but is already subscribed.";
		return;
	}
	mSubscriberList.push_back(clientID);

	// If there were no subscribers, activate auto sending of updates
	if (!mSubscriberUpdateTimer.isActive()) {
		mSceneUpdates.clear();
		mSubscriberUpdateTimer.start();
	}
	
	// Prepare the visual information
	QList<Visual3DPtr> visuals = mScene->getVisuals3D();
	QVariantList visualsInfo;
	visualsInfo.reserve(visuals.count());
	for (int i = 0; i < visuals.count(); i++) {
		if (!visuals.at(i)->isHolographic()) {
			continue;
		}
		QJsonObject visualJson = ModuleMessageEncoder::toJson(visuals.at(i).data());
		visualJson.insert(RESPONSE_ID, EModuleResponse::ADD_VISUAL);
		visualsInfo.push_back(visualJson);
	}

	// Prepare the module interaction information
	QList<ModuleInteractionPtr> interactions = this->getModuleInteractions();
	QVariantList interactionsJson;
	interactionsJson.reserve(interactions.count());
	for (ModuleInteractionPtr interaction : interactions) {
		if (!interaction->isEnabledForFrameworkInterfaces()) {
			continue;
		}
		QJsonObject interactionJson = ModuleMessageEncoder::toJson(interaction.data());
		interactionJson.insert(RESPONSE_ID, EModuleResponse::ADD_MODULE_INTERACTION);
		interactionsJson.push_back(interactionJson);
	}

	// Send Module Information
	QJsonObject moduleInfo;
	moduleInfo.insert(VISUALS_INFO, QJsonArray::fromVariantList(visualsInfo));
	moduleInfo.insert(MODULE_INTERACTIONS, QJsonArray::fromVariantList(interactionsJson));

	// Send Response
	QJsonObject response = this->prepareModuleResponse(moduleInfo);
	this->sendMessage(response, clientID);
	
	emit feedbackColor(tr("User %1 has subscribed to module").arg(clientID), Qt::GlobalColor::darkBlue);

	qDebug() << "Exit";
}

void ModuleMessageEncoder::parseUnbuscribeToModule(const QJsonObject& params, const QString& clientID) {
	qDebug() << "Enter - Client:" << clientID << "unsubscribed from module";
	mSubscriberList.removeOne(clientID);
	// TODO: Remove auto sending of scene updates.
	emit feedbackColor(tr("User %1 has unsubscribed from module").arg(clientID), Qt::GlobalColor::darkBlue);
	qDebug() << "Exit";
}

void ModuleMessageEncoder::parseGetScene(const QJsonObject& params, const QString& clientID) {
	qDebug() << "Enter - Scene requested by=" << clientID;

	// Prepare the Visuals information
	QList<Visual3DPtr> visuals = mScene->getVisuals3D();
	QVariantList visualsInfo;
	visualsInfo.reserve(visuals.count());
	for (int i = 0; i < visuals.count(); i++) {
		if (!visuals.at(i)->isHolographic()) {
			continue;
		}
		QJsonObject visual = ModuleMessageEncoder::toJson(visuals.at(i).data());
		visual.insert(RESPONSE_ID, EModuleResponse::REFRESH_VISUAL);
		visualsInfo.push_back(visual);
	}
	
	// Prepare Module information.
	QJsonObject moduleInfo;
	moduleInfo.insert(VISUALS_INFO, QJsonArray::fromVariantList(visualsInfo));

	// Send Response
	QJsonObject response = this->prepareModuleResponse(moduleInfo);
	this->sendMessage(response, clientID);

	qDebug() << "Exit";
}

void ModuleMessageEncoder::parseGetobject(const QJsonObject& params, const QString& clientID) {
	qDebug() << "Enter - client=" << clientID << "requested an object";

	// Prepare Visual information
	QString visualID = params.value(TARGET_VISUAL).toString("");
	Visual3DPtr visual = mScene->getVisual3D(visualID);
	if (visual == Q_NULLPTR) {
		this->sendErrorMessage("Object with given name not found", clientID);
		return;
	}
	QJsonObject visualInfo = ModuleMessageEncoder::toJson(visual.data());
	visualInfo.insert(RESPONSE_ID, EModuleResponse::REFRESH_VISUAL);

	QJsonArray visuals;
	visuals.append(visualID);

	// Prepare Module information
	QJsonObject moduleInfo;
	moduleInfo.insert(VISUALS_INFO, visuals);

	QJsonObject response = this->prepareModuleResponse(moduleInfo);
	this->sendMessage(response, clientID);

	qDebug() << "Exit";
}

void ModuleMessageEncoder::parseHideObject(const QJsonObject& params, const QString& clientID) {
	qDebug() << "Enter";
	bool isVisible = params.value(IS_VISIBLE).toBool(false);
	QString visualID = params.value(TARGET_VISUAL).toString("");
	if (!mScene.isNull()) {
		mScene->setVisualVisible(isVisible, visualID);
	}
	qDebug() << "Exit";
}

void ModuleMessageEncoder::parseTranslateObject(const QJsonObject& params, const QString& clientID) {
	qDebug() << "Enter - client:" << clientID << "translates visual";
	
	QJsonArray translate = params.value(TRANSLATE).toArray({0, 0, 0});

	double x = translate.at(0).toDouble(0);
	double y = translate.at(1).toDouble(0);
	double z = translate.at(2).toDouble(0);
	QString visualID = params.value(TARGET_VISUAL).toString("");
	if (!mScene.isNull()) {
		mScene->translateVisual(x, y, z, visualID);
	}
	
	qDebug() << "Exit";
}

void ModuleMessageEncoder::parseRotateObject(const QJsonObject& params, const QString& clientID) {
	qDebug() << "Enter - client:" << clientID << "rotates object";

	QJsonArray rotate = params.value(ROTATE).toArray({0, 0, 0});

	double x = rotate.at(0).toDouble(0);
	double y = rotate.at(1).toDouble(0);
	double z = rotate.at(2).toDouble(0);
	QString visualID = params.value(TARGET_VISUAL).toString("");
	if (!mScene.isNull()) {
		mScene->rotateVisual(x, y, z, visualID);
	}

	qDebug() << "Exit";
}

void ModuleMessageEncoder::parseSelectSlice(const QJsonObject& params, const QString& clientID) {
	qDebug() << "Enter - client:" << clientID << "requested to select slice";

	int sliceIndex = params.value(SLICE_INDEX).toInt(0);
	ESliceOrientation orientation(params.value(SLICE_ORIENTATION).toInt(0));
	QString visualID = params.value(TARGET_VISUAL).toString("");

	// If the orientation was not provided, only assign the slice
	if (orientation == ESliceOrientation::UNKNOWN) {
		if (!mScene.isNull()) {
			mScene->setImageSliceIndex(sliceIndex, visualID);
		}
	} else {
		if (!mScene.isNull()) {
			mScene->setImageSlice(sliceIndex, orientation, visualID);
		}
	}
	
	qDebug() << "Exit";
}

void ModuleMessageEncoder::parseSelectOrientation(const QJsonObject& params, const QString& clientID) {
	qDebug() << "Enter - client:" << clientID << "selects orientation";

	ESliceOrientation orientation(params.value(SLICE_ORIENTATION).toInt(0));
	QString visualID = params.value(TARGET_VISUAL).toString("");
	if (!mScene.isNull()) {
		mScene->setImageSliceOrientation(orientation, visualID);
	}

	qDebug() << "Exit";
}

void ModuleMessageEncoder::parseSelectSeries(const QJsonObject& params, const QString& clientID) {
	qDebug() << "Enter - client:" << clientID << "requested to select series";
	
	int seriesIndex = params.value(SERIES_INDEX).toInt(0);
	QString visualID = params.value(TARGET_VISUAL).toString("");

	if (!mScene.isNull()) {
		mScene->setStudySliceSeriesIndex(seriesIndex, visualID);
	}

	qDebug() << "Exit";
}

QJsonObject ModuleMessageEncoder::prepareModuleResponse(QJsonObject& modInfo, const QString& message) {
	modInfo.insert(MODULE_ID, this->getModuleID());
	modInfo.insert(SCENE_ID, this->getMainScene()->getSceneID());

	QJsonObject response;
	response.insert(RESPONSE_STATUS, EResponseStatus::SUCCESS);
	response.insert(MESSAGE_TYPE, EMessage::MODULE);
	response.insert(MODULE_INFO, modInfo);
	response.insert(MESSAGE, message);
	return response;
}

void ModuleMessageEncoder::prepareAddInteraction(const QString& interactionID) {
	qDebug() << "Enter";

	ModuleInteractionPtr inte = this->getModuleInteraction(interactionID);
	if (inte.isNull()) {
		qWarning() << "Failed to add interaction. Interaction not found.";
		return;
	}

	if (!mSubscriberList.empty()) {
		InteractionUpdateKey updateKey {
			interactionID,
			EModuleResponse::ADD_MODULE_INTERACTION,
			true
		};
		if (!mInteractionUpdates.contains(updateKey)) {
			mInteractionUpdates.insert(updateKey, true);
		}
	}

	// Connect to changes in the interaction
	QObject::connect(
		inte.data(), &ModuleInteraction::changedInteractionValue,
		this, &ModuleMessageEncoder::prepareChangeInteractionValue,
		Qt::UniqueConnection);
	QObject::connect(
		inte.data(), &ModuleInteraction::changedInteractionConstraint,
		this, &ModuleMessageEncoder::prepareChangeInteractionConstraint,
		Qt::UniqueConnection);

	// If the interaction is a SELECT, we need to hear for the change of
	// options as well.
	SelectInteractionPtr selectInte = this->getSelectModuleInteraction(interactionID);
	if (!selectInte.isNull()) {
		QObject::connect(
			selectInte.data(), &SelectInteraction::changedOptions,
			this, &ModuleMessageEncoder::prepareChangeInteractionValue,
			Qt::UniqueConnection);
	}

	qDebug() << "Exit";
}

void ModuleMessageEncoder::prepareRemoveInteraction(const QString& interactionID) {
	qDebug() << "Enter";
	
	ModuleInteractionPtr inte = this->getModuleInteraction(interactionID);
	if (inte.isNull()) {
		qWarning() << "Failed to add interaction. Interaction not found.";
		return;
	}

	if (!mSubscriberList.empty()) {
		InteractionUpdateKey updateKey {
			interactionID,
			EModuleResponse::REMOVE_MODULE_INTERACTION,
			false
		};
		if (!mInteractionUpdates.contains(updateKey)) {
			mInteractionUpdates.insert(updateKey, true);
		}
	}

	// Connect to changes in the interaction
	QObject::disconnect(
		inte.data(), &ModuleInteraction::changedInteractionValue,
		this, &ModuleMessageEncoder::prepareChangeInteractionValue);
	QObject::disconnect(
		inte.data(), &ModuleInteraction::changedInteractionConstraint,
		this, &ModuleMessageEncoder::prepareChangeInteractionConstraint);

	qDebug() << "Exit";
}

void ModuleMessageEncoder::prepareChangeInteractionValue() {
	qDebug() << "Enter";
	
	if (mSubscriberList.empty()) {
		return;
	}

	ModuleInteraction* inte = qobject_cast<ModuleInteraction*>(sender());
	if (inte == Q_NULLPTR) {
		qWarning() << "Failed to change interaction value. Interaction not found.";
		return;
	}

	InteractionUpdateKey updateKey {
		inte->getInteractionID(),
		EModuleResponse::UPDATE_MODULE_INTERACTION,
		false
	};
	if (!mInteractionUpdates.contains(updateKey)) {
		mInteractionUpdates.insert(updateKey, true);
	}

	qDebug() << "Exit";
}

void ModuleMessageEncoder::prepareChangeInteractionConstraint() {
	qDebug() << "Enter";
	
	if (mSubscriberList.empty()) {
		return;
	}

	ModuleInteraction* inte = qobject_cast<ModuleInteraction*>(sender());
	if (inte == Q_NULLPTR) {
		qWarning() << "Failed to change interaction constraint. Interaction not found.";
		return;
	}

	InteractionUpdateKey updateKey {
		inte->getInteractionID(),
		EModuleResponse::UPDATE_MODULE_INTERACTION,
		true
	};

	// We insert because if it doesn't contain, we need. If it does, we replace
	// ensuring the UpdateConstraint flag stays true.
	mInteractionUpdates.insert(updateKey, true);

	qDebug() << "Exit";
}

void ModuleMessageEncoder::prepareAddObject(VisualPtr visual) {
	qDebug() << "Enter";

	if (visual.isNull()) {
		qDebug() << "Exit - Visual is null";
		return;
	}

	if (!visual->is3D() || !visual->isHolographic()) {
		qDebug() << "Exit - Visual is not 3D or Holographic";
		return;
	}

	// If there are subscribers, send them information on new added object
	if (!mSubscriberList.empty()) {
		SceneUpdateKey updateKey{visual->getVisualID(), EModuleResponse::ADD_VISUAL};
		if (!mSceneUpdates.contains(updateKey)) {
			mSceneUpdates.insert(updateKey, true);
		}
	}

	// Because it is a new object, we need to connect its signals to send updates
	Visual3D* visual3D = qobject_cast<Visual3D*>(visual.data());
	if (visual3D == Q_NULLPTR) {
		qDebug() << "Exit";
		return;
	}

	this->connectVisual3DSignals(visual3D);
	QObject::connect(
		visual3D, &Visual3D::changedHolographicState,
		this, &ModuleMessageEncoder::onVisualHolographicStateChange, 
		Qt::UniqueConnection);

	qDebug() << "Exit";
}

void ModuleMessageEncoder::prepareDataChange() {
	qDebug() << "Enter";
	if (mSubscriberList.empty()) {
		return;
	}

	Visual3D* visual = qobject_cast<Visual3D*>(sender());
	if (visual == Q_NULLPTR) {
		qWarning() << "Failed to update Visual. It is not 3D.";
		return;
	}

	SceneUpdateKey updateKey{visual->getVisualID(), EModuleResponse::DATA_CHANGE};
	if (!mSceneUpdates.contains(updateKey)) {
		mSceneUpdates.insert(updateKey, true);
	}

	qDebug() << "Exit";
}

void ModuleMessageEncoder::prepareRefreshObject() {
	qDebug() << "Enter";
	if (mSubscriberList.empty()) {
		return;
	}

	Visual3D* visual = qobject_cast<Visual3D*>(sender());
	if (visual == Q_NULLPTR) {
		qWarning() << "Failed to refresh Visual. It is not 3D.";
		return;
	}

	SceneUpdateKey key{visual->getVisualID(), EModuleResponse::REFRESH_VISUAL};
	if (!mSceneUpdates.contains(key)) {
		mSceneUpdates.insert(key, true);
	}

	qDebug() << "Exit";
}

void ModuleMessageEncoder::prepareRemoveObject(VisualPtr visual) {
	qDebug() << "Enter";
	if (!mSubscriberList.empty()) {
		if (visual == Q_NULLPTR || !visual->is3D()) {
			return;
		}

		SceneUpdateKey updateKey{visual->getVisualID(), EModuleResponse::REMOVE_VISUAL};
		if (!mSceneUpdates.contains(updateKey)) {
			mSceneUpdates.insert(updateKey, true);
		}
	}

	// Remove the update events since the Visual is not part of the scene
	Visual3D* visual3D = qobject_cast<Visual3D*>(visual.data());
	if (visual3D == Q_NULLPTR) {
		qDebug() << "Exit";
		return;
	}

	this->disconnectVisual3DSignals(visual3D);
	QObject::disconnect(
		visual3D, &Visual3D::changedHolographicState,
		this, &ModuleMessageEncoder::onVisualHolographicStateChange);

	qDebug() << "Exit";
}

void ModuleMessageEncoder::prepareHideObject(const bool& isVisible) {
	qDebug() << "Enter";
	if (mSubscriberList.empty()) {
		return;
	}

	Visual3D* visual = qobject_cast<Visual3D*>(sender());
	if (visual == Q_NULLPTR) {
		qWarning() << "Failed to hide object. It is not 3D.";
		return;
	}

	SceneUpdateKey key {
		visual->getVisualID(), 
		EModuleResponse::HIDE_VISUAL
	};
	if (!mSceneUpdates.contains(key)) {
		mSceneUpdates.insert(key, true);
	}

	qDebug() << "Exit";
}

void ModuleMessageEncoder::prepareTransformObject() {
	qDebug() << "Enter";
	if (mSubscriberList.empty()) {
		return;
	}

	Visual3D* visual = qobject_cast<Visual3D*>(sender());
	if (visual == Q_NULLPTR) {
		qWarning() << "Failed to transform Visual. It is not 3D.";
		return;
	}

	SceneUpdateKey key {
		visual->getVisualID(),
		EModuleResponse::TRANSFORM_VISUAL
	};
	if (!mSceneUpdates.contains(key)) {
		mSceneUpdates.insert(key, true);
	}

	qDebug() << "Exit";
}

void ModuleMessageEncoder::prepareParentChange(Visual3DPtr parentVisual) {
	qDebug() << "Enter";
	if (mSubscriberList.empty()) {
		return;
	}

	Visual3D* visual = qobject_cast<Visual3D*>(sender());
	if (visual == Q_NULLPTR) {
		qWarning() << "Failed to update Visual's parent. It is not 3D.";
		return;
	}

	SceneUpdateKey key{visual->getVisualID(), EModuleResponse::PARENT_CHANGE};
	if (!mSceneUpdates.contains(key)) {
		mSceneUpdates.insert(key, true);
	}

	qDebug() << "Exit";
}

void ModuleMessageEncoder::prepareSetObjectOpacity(const double& a) {
	qDebug() << "Enter";
	if (mSubscriberList.empty()) {
		return;
	}

	Visual3D* visual = qobject_cast<Visual3D*>(sender());
	if (visual == Q_NULLPTR) {
		qWarning() << "Failed to update Visual's opacity. It is not 3D.";
		return;
	}

	SceneUpdateKey key {
		visual->getVisualID(),
		EModuleResponse::SET_VISUAL_OPACITY
	};
	if (!mSceneUpdates.contains(key)) {
		mSceneUpdates.insert(key, true);
	}

	qDebug() << "Exit";
}

void ModuleMessageEncoder::prepareSetSliceIndex() {
	qDebug() << "Enter";
	if (mSubscriberList.empty()) {
		return;
	}

	// Only models have color
	ImageSlice* imageSlice = qobject_cast<ImageSlice*>(sender());
	if (imageSlice == Q_NULLPTR) {
		qWarning() << "Failed to set slice index. It is not ImageSlice.";
		return;
	}

	SceneUpdateKey key {
		imageSlice->getVisualID(),
		EModuleResponse::SET_SLICE
	};
	if (!mSceneUpdates.contains(key)) {
		mSceneUpdates.insert(key, true);
	}

	qDebug() << "Exit";
}

void ModuleMessageEncoder::prepareSetObjectColor(const double& r, const double& g, const double& b) {
	qDebug() << "Enter";
	if (mSubscriberList.empty()) {
		return;
	}

	// Only models have color
	Model* model = qobject_cast<Model*>(sender());
	if (model == Q_NULLPTR) {
		qWarning() << "Failed to set Model color. It is not Model.";
		return;
	}

	SceneUpdateKey key {
		model->getVisualID(),
		EModuleResponse::SET_OBJECT_COLOR
	};
	if (!mSceneUpdates.contains(key)) {
		mSceneUpdates.insert(key, true);
	}

	qDebug() << "Exit";
}

void ModuleMessageEncoder::prepareAssemblyAddPart(ModelPtr part) {
	qDebug() << "Enter";
	if (mSubscriberList.empty()) {
		return;
	}

	Assembly* assembly = qobject_cast<Assembly*>(sender());
	if (assembly == Q_NULLPTR) {
		qWarning() << "Failed to add Assembly part. It is not Assembly.";
		return;
	}

	AssembyUpdateKey key;
	key.VisualID = assembly->getVisualID();
	key.ResponseID = EModuleResponse::DATA_CHANGE;
	key.PartID = part->getVisualID();
	key.AssemblyResponseID = EModuleResponse::ADD_VISUAL;
	if (!mAssemblyUpdates.contains(key)) {
		mAssemblyUpdates.insert(key, true);
	}

	qDebug() << "Exit";
}

void ModuleMessageEncoder::prepareAssemblyRemovePart(ModelPtr part) {
	qDebug() << "Enter";
	if (mSubscriberList.empty()) {
		return;
	}

	Assembly* assembly = qobject_cast<Assembly*>(sender());
	if (assembly == Q_NULLPTR) {
		qWarning() << "Failed to remove Assembly part. It is not Assembly.";
		return;
	}

	AssembyUpdateKey key;
	key.VisualID = assembly->getVisualID();
	key.ResponseID = EModuleResponse::DATA_CHANGE;
	key.PartID = part->getVisualID();
	key.AssemblyResponseID = EModuleResponse::REMOVE_VISUAL;
	if (!mAssemblyUpdates.contains(key)) {
		mAssemblyUpdates.insert(key, true);
	}

	qDebug() << "Exit";
}

void ModuleMessageEncoder::prepareAssemblyPartOpacity(const double& a, ModelPtr part) {
	qDebug() << "Enter";
	if (mSubscriberList.empty()) {
		return;
	}

	Assembly* assembly = qobject_cast<Assembly*>(sender());
	if (assembly == Q_NULLPTR) {
		qWarning() << "Failed to change Assembly part opacity. It is not Assembly.";
		return;
	}

	AssembyUpdateKey key;
	key.VisualID = assembly->getVisualID();
	key.ResponseID = EModuleResponse::DATA_CHANGE;
	key.PartID = part->getVisualID();
	key.AssemblyResponseID = EModuleResponse::SET_VISUAL_OPACITY;
	if (!mAssemblyUpdates.contains(key)) {
		mAssemblyUpdates.insert(key, true);
	}

	qDebug() << "Exit";
}

void ModuleMessageEncoder::prepareAssemblyPartHide(const bool& isVisible, ModelPtr part) {
	qDebug() << "Enter";
	if (mSubscriberList.empty()) {
		return;
	}

	Assembly* assembly = qobject_cast<Assembly*>(sender());
	if (assembly == Q_NULLPTR) {
		qWarning() << "Failed to change Assembly part visibility. It is not Assembly.";
		return;
	}

	AssembyUpdateKey key;
	key.VisualID = assembly->getVisualID();
	key.ResponseID = EModuleResponse::DATA_CHANGE;
	key.PartID = part->getVisualID();
	key.AssemblyResponseID = EModuleResponse::HIDE_VISUAL;
	if (!mAssemblyUpdates.contains(key)) {
		mAssemblyUpdates.insert(key, true);
	}

	qDebug() << "Exit";
}

void ModuleMessageEncoder::prepareAssemblyPartTransform(ModelPtr part) {
	qDebug() << "Enter";
	if (mSubscriberList.empty()) {
		return;
	}

	Assembly* assembly = qobject_cast<Assembly*>(sender());
	if (assembly == Q_NULLPTR) {
		qWarning() << "Failed to transform Assembly part. It is not Assembly.";
		return;
	}

	AssembyUpdateKey key;
	key.VisualID = assembly->getVisualID();
	key.ResponseID = EModuleResponse::DATA_CHANGE;
	key.PartID = part->getVisualID();
	key.AssemblyResponseID = EModuleResponse::TRANSFORM_VISUAL;
	if (!mAssemblyUpdates.contains(key)) {
		mAssemblyUpdates.insert(key, true);
	}

	qDebug() << "Exit";
}

void ModuleMessageEncoder::prepareAssemblyPartData(ModelDataVPtr data, ModelPtr part) {
	qDebug() << "Enter";
	if (mSubscriberList.empty()) {
		return;
	}

	Assembly* assembly = qobject_cast<Assembly*>(sender());
	if (assembly == Q_NULLPTR) {
		qWarning() << "Failed to change Assembly part data. It is not Assembly.";
		return;
	}

	AssembyUpdateKey key;
	key.VisualID = assembly->getVisualID();
	key.ResponseID = EModuleResponse::DATA_CHANGE;
	key.PartID = part->getVisualID();
	key.AssemblyResponseID = EModuleResponse::DATA_CHANGE;
	if (!mAssemblyUpdates.contains(key)) {
		mAssemblyUpdates.insert(key, true);
	}

	qDebug() << "Exit";
}

void ModuleMessageEncoder::prepareAssemblyPartColor(const double& r, const double& g, const double& b, ModelPtr part) {
	qDebug() << "Enter";
	if (mSubscriberList.empty()) {
		return;
	}

	Assembly* assembly = qobject_cast<Assembly*>(sender());
	if (assembly == Q_NULLPTR) {
		qWarning() << "Failed to change Assembly part color." <<
			"It is not Assembly.";
		return;
	}

	AssembyUpdateKey key;
	key.VisualID = assembly->getVisualID();
	key.ResponseID = EModuleResponse::DATA_CHANGE;
	key.PartID = part->getVisualID();
	key.AssemblyResponseID = EModuleResponse::SET_OBJECT_COLOR;
	if (!mAssemblyUpdates.contains(key)) {
		mAssemblyUpdates.insert(key, true);
	}

	qDebug() << "Exit";
}

void ModuleMessageEncoder::encodeVisualInfo(Visual3DPtr visual, 
	const EModuleResponse& responseType, QJsonObject& visualInfo) 
{
	qDebug() << "Enter";

	visualInfo.insert(RESPONSE_ID, responseType.toInt());
	switch (responseType.toInt()) {
		case EModuleResponse::ADD_VISUAL:
		case EModuleResponse::REFRESH_VISUAL:
		case EModuleResponse::DATA_CHANGE:
			ModuleMessageEncoder::toJson(visual.data(), visualInfo);
			break;
		case EModuleResponse::REMOVE_VISUAL:
			visualInfo.insert(ID, visual->getVisualID());
			visualInfo.insert(TYPE, visual->getVisualType().toInt());
			break;
		case EModuleResponse::HIDE_VISUAL:
			visualInfo.insert(ID, visual->getVisualID());
			visualInfo.insert(TYPE, visual->getVisualType().toInt());
			visualInfo.insert(IS_VISIBLE, visual->isVisible());
			break;
		case EModuleResponse::TRANSFORM_VISUAL:
		{
			QVariantList transformation;
			transformation.reserve(16);
			vtkMatrix4x4* matrix = visual->getRelativeMatrix();
			for (int i = 0; i < 16; i++) {
				transformation.push_back(matrix->GetData()[i]);
			}

			visualInfo.insert(ID, visual->getVisualID());
			visualInfo.insert(TYPE, visual->getVisualType().toInt());
			visualInfo.insert(TRANSFORMATION, QJsonArray::fromVariantList(transformation));
			break;
		}
		case EModuleResponse::PARENT_CHANGE:
			visualInfo.insert(ID, visual->getVisualID());
			visualInfo.insert(TYPE, visual->getVisualType().toInt());
			visualInfo.insert(PARENT_ID, visual->getParentVisualName());
			break;
		case EModuleResponse::SET_VISUAL_OPACITY:
			visualInfo.insert(ID, visual->getVisualID());
			visualInfo.insert(TYPE, visual->getVisualType().toInt());
			visualInfo.insert(OPACITY, visual->getOpacity());
			break;
		case EModuleResponse::SET_SLICE:
		{
			visualInfo.insert(ID, visual->getVisualID());
			visualInfo.insert(TYPE, visual->getVisualType().toInt());

			ImageSlice* imageSlice = qobject_cast<ImageSlice*>(visual.data());

			visualInfo.insert(SLICE_INDEX, imageSlice->getSliceIndex());
			visualInfo.insert(SLICE_ORIENTATION, imageSlice->getSliceOrientation().toInt());
			visualInfo.insert(DATA_NAME, imageSlice->getImageData()->getDataName());

			// If it is a StudySlice, provide series index too.
			StudySlice* studyImageSlice = qobject_cast<StudySlice*>(imageSlice);
			if (studyImageSlice != Q_NULLPTR) {
				visualInfo.insert(SERIES_INDEX, studyImageSlice->getSeriesIndex());
				visualInfo.insert(DATA_TYPE, EData::STUDY);
				visualInfo.insert(DATA_ID, studyImageSlice->getStudy()->getDataID().toString());
			} else {
				visualInfo.insert(DATA_TYPE, EData::IMAGE);
				visualInfo.insert(DATA_ID, imageSlice->getImageData()->getDataID().toString());
			}
			break;
		}
		case EModuleResponse::SET_OBJECT_COLOR:
		{
			Model* model = qobject_cast<Model*>(visual.data());

			double r, g, b;
			model->getColor(r, g, b);

			QJsonArray color;
			color.push_back(r);
			color.push_back(g);
			color.push_back(b);

			visualInfo.insert(ID, model->getVisualID());
			visualInfo.insert(TYPE, model->getVisualType().toInt());
			visualInfo.insert(COLOR, color);
			break;
		}
		default:
			break;
	}

	qDebug() << "Exit";
}

QJsonObject ModuleMessageEncoder::encodeVisualInfo(Visual3DPtr visual, const EModuleResponse& responseType) {
	QJsonObject visualInfo;
	this->encodeVisualInfo(visual, responseType, visualInfo);
	return visualInfo;
}

void ModuleMessageEncoder::sendSceneUpdates() {
	if (mSceneUpdates.empty() && mAssemblyUpdates.empty() && mInteractionUpdates.empty()) {
		return;
	}

	QVariantList visualUpdates;
	visualUpdates.reserve(mSceneUpdates.count() + mAssemblyUpdates.count());

	QHash<SceneUpdateKey, bool>::iterator it = mSceneUpdates.begin();
	for (; it != mSceneUpdates.end(); it++) {
		Visual3DPtr visual = mScene->getVisual3D(it.key().VisualID);
		if (visual.isNull()) {
			qWarning() << "Failed to update Visual" << it.key().VisualID << "because it was not found.";
			continue;
		}

		// Prepare Visual info
		QJsonObject visualInfo = this->encodeVisualInfo(visual, it.key().ResponseID);
		visualUpdates.append(visualInfo);
	}

	QHash<AssembyUpdateKey, bool>::iterator asIt = mAssemblyUpdates.begin();
	for (; asIt != mAssemblyUpdates.end(); asIt++) {
		AssemblyPtr assembly = mScene->getAssembly(asIt.key().VisualID);
		if (assembly.isNull()) {
			qWarning() << "Failed to update Assembly" << asIt.key().VisualID << "because it was not found";
			return;
		}

		ModelPtr part = assembly->getPart(asIt.key().PartID);
		if (part.isNull()) {
			qWarning() << "Failed to update Assembly" << asIt.key().VisualID 
				<< "because Part" << asIt.key().PartID << "was not found";
			continue;
		}

		QJsonArray assemblyPartsInfo;
		assemblyPartsInfo.push_back(this->encodeVisualInfo(part, asIt.key().AssemblyResponseID));

		QJsonObject visualInfo;
		visualInfo.insert(RESPONSE_ID, EModuleResponse::DATA_CHANGE);
		visualInfo.insert(ID, assembly->getVisualID());
		visualInfo.insert(TYPE, assembly->getVisualType().toInt());
		visualInfo.insert(ASSEMBLY_PARTS_INFO, assemblyPartsInfo);

		visualUpdates.append(visualInfo);
	}

	QVariantList interactionUpdates;
	interactionUpdates.reserve(mInteractionUpdates.count());
	QHash<InteractionUpdateKey, bool>::iterator inteIt = mInteractionUpdates.begin();
	for (; inteIt != mInteractionUpdates.end(); inteIt++) {
		InteractionUpdateKey key = inteIt.key();
		ModuleInteractionPtr inte = this->getModuleInteraction(key.InteractionID);
		if (inte.isNull()) {
			qWarning() << "Failed to send module interaction update for" <<
				"interaction:" << key.InteractionID << "because the" <<
				"interaction was not found";
			continue;
		}

		if (key.ResponseID == EModuleResponse::ADD_MODULE_INTERACTION) {
			QJsonObject inteJson;
			this->toJson(inte.data(), inteJson);
			inteJson.insert(RESPONSE_ID, key.ResponseID.toInt());
			inteJson.insert(UPDATE_CONSTRAINT, true);

			interactionUpdates.append(inteJson);
		} else if (key.ResponseID == EModuleResponse::REMOVE_MODULE_INTERACTION) {
			QJsonObject inteJson;
			inteJson.insert(RESPONSE_ID, key.ResponseID.toInt());
			inteJson.insert(ID, key.InteractionID);

			interactionUpdates.append(inteJson);
		} else if (key.ResponseID == EModuleResponse::UPDATE_MODULE_INTERACTION) {
			QJsonObject inteJson;
			this->toJson(inte.data(), inteJson, key.UpdateConstraint);
			inteJson.insert(RESPONSE_ID, key.ResponseID.toInt());
			inteJson.insert(UPDATE_CONSTRAINT, key.UpdateConstraint);

			interactionUpdates.append(inteJson);
		} else {
			qWarning() << "Failed to send module interaction update because" <<
				"the response type is unknown.";
		}
	}

	QJsonObject moduleInfo;
	moduleInfo.insert(VISUALS_INFO, QJsonArray::fromVariantList(visualUpdates));
	moduleInfo.insert(MODULE_INTERACTIONS, QJsonArray::fromVariantList(interactionUpdates));
	QJsonObject response = this->prepareModuleResponse(moduleInfo);
	this->sendSelectMessage(response, mSubscriberList);

	mSceneUpdates.clear();
	mAssemblyUpdates.clear();
	mInteractionUpdates.clear();
}

void ModuleMessageEncoder::setupScene() {
	qDebug() << "Enter - Setting up scene for" << this->getModuleID();

	ScenePtr scene = this->getMainScene();

	// If the old scene has not been deleted, disconnect all the signals 
	if (!mScene.isNull()) {
		QObject::disconnect(
			mScene.data(), &Scene::changedAddedSceneObject,
			this, &ModuleMessageEncoder::prepareAddObject);
		QObject::disconnect(
			mScene.data(), &Scene::changedRemovedSceneObject,
			this, &ModuleMessageEncoder::prepareRemoveObject);

		QList<Visual3DPtr> visuals = mScene->getVisuals3D();
		for (int i = 0; i < visuals.count(); i++) {
			this->disconnectVisual3DSignals(visuals.at(i).data());
			QObject::disconnect(
				visuals.at(i).data(), &Visual3D::changedHolographicState,
				this, &ModuleMessageEncoder::onVisualHolographicStateChange
			);
		}
	}

	mScene = scene;

	// if the passed scene is null, then nothing to do
	if (mScene.isNull()) {
		return;
	}

	// Connect signals from new scene
	QObject::connect(
		mScene.data(), &Scene::changedAddedSceneObject,
		this, &ModuleMessageEncoder::prepareAddObject,
		Qt::UniqueConnection);
	QObject::connect(
		mScene.data(), &Scene::changedRemovedSceneObject,
		this, &ModuleMessageEncoder::prepareRemoveObject,
		Qt::UniqueConnection);

	QList<Visual3DPtr> visuals = mScene->getVisuals3D();
	for (int i = 0; i < visuals.count(); i++) {
		this->connectVisual3DSignals(visuals.at(i).data());
		QObject::connect(
			visuals.at(i).data(), &Visual3D::changedHolographicState,
			this, &ModuleMessageEncoder::onVisualHolographicStateChange,
			Qt::UniqueConnection);
	}

	qDebug() << "Exit";
}

void ModuleMessageEncoder::onVisualHolographicStateChange(const bool& isHolographic) {
	qDebug() << "Enter";

	Visual3D* visual = qobject_cast<Visual3D*>(sender());
	if (visual == Q_NULLPTR) {
		qWarning() << "Failed to update holographic state. It is not 3D.";
		return;
	}

	if (isHolographic) {
		this->connectVisual3DSignals(visual);
	} else {
		this->disconnectVisual3DSignals(visual);
	}

	qDebug() << "Exit";
}

void ModuleMessageEncoder::connectVisual3DSignals(Visual3D* visual) {
	if (visual == Q_NULLPTR) {
		qDebug() << "Given visual was null";
		return;
	}

	if (!visual->isHolographic()) {
		qDebug() << "The visual" << visual->getVisualID() << "is not holographic";
		return;
	}

	QObject::connect(
		visual, &Visual3D::changedOpacity,
		this, &ModuleMessageEncoder::prepareSetObjectOpacity,
		Qt::UniqueConnection);
	QObject::connect(
		visual, &Visual3D::changedVisibleState,
		this, &ModuleMessageEncoder::prepareHideObject,
		Qt::UniqueConnection);
	QObject::connect(
		visual, &Visual3D::changedTransformation,
		this, &ModuleMessageEncoder::prepareTransformObject,
		Qt::UniqueConnection);
	QObject::connect(
		visual, &Visual3D::changedSceneParent,
		this, &ModuleMessageEncoder::prepareParentChange, 
		Qt::UniqueConnection);

	if (visual->getVisualType() == EVisual::IMAGE_SLICE) {
		ImageSlice* imageSlice = qobject_cast<ImageSlice*>(visual);
		QObject::connect(
			imageSlice, &ImageSlice::changedImageData, 
			this, &ModuleMessageEncoder::prepareDataChange, 
			Qt::UniqueConnection);
		QObject::connect(
			imageSlice, &ImageSlice::changedSlice, 
			this, &ModuleMessageEncoder::prepareSetSliceIndex,
			Qt::UniqueConnection);
	} else if (visual->getVisualType().isStudySlice()) {
		StudySlice* studyImageSlice = qobject_cast<StudySlice*>(visual);
		QObject::connect(
			studyImageSlice, &StudySlice::changedStudy, 
			this, &ModuleMessageEncoder::prepareDataChange, 
			Qt::UniqueConnection);
		QObject::connect(
			studyImageSlice, &StudySlice::changedSlice,
			this, &ModuleMessageEncoder::prepareSetSliceIndex,
			Qt::UniqueConnection);
	} else if (visual->getVisualType().isModel()) {
		Model* model = qobject_cast<Model*>(visual);
		QObject::connect(
			model, &Model::changedModelData,
			this, &ModuleMessageEncoder::prepareDataChange,
			Qt::UniqueConnection);
		QObject::connect(
			model, &Model::changedColor, 
			this, &ModuleMessageEncoder::prepareSetObjectColor,
			Qt::UniqueConnection);
	}  else if (visual->getVisualType() == EVisual::ASSEMBLY) {
		Assembly* assembly = qobject_cast<Assembly*>(visual);
		QObject::connect(
			assembly, &Assembly::changedAddedPart,
			this, &ModuleMessageEncoder::prepareAssemblyAddPart,
			Qt::UniqueConnection);
		QObject::connect(
			assembly, &Assembly::changedRemovedPart, 
			this, &ModuleMessageEncoder::prepareAssemblyRemovePart, 
			Qt::UniqueConnection);
		QObject::connect(
			assembly, &Assembly::changedPartOpacity, 
			this, &ModuleMessageEncoder::prepareAssemblyPartOpacity, 
			Qt::UniqueConnection);
		QObject::connect(
			assembly, &Assembly::changedPartVisible, 
			this, &ModuleMessageEncoder::prepareAssemblyPartHide,
			Qt::UniqueConnection);
		QObject::connect(
			assembly, &Assembly::changedPartTransformation, 
			this, &ModuleMessageEncoder::prepareAssemblyPartTransform,
			Qt::UniqueConnection);
		QObject::connect(
			assembly, &Assembly::changedPartData,
			this, &ModuleMessageEncoder::prepareAssemblyPartData, 
			Qt::UniqueConnection);
		QObject::connect(
			assembly, &Assembly::changedPartColor, 
			this, &ModuleMessageEncoder::prepareAssemblyPartColor, 
			Qt::UniqueConnection);
	}
}

void ModuleMessageEncoder::disconnectVisual3DSignals(Visual3D* visual) {
	if (visual == Q_NULLPTR) {
		qDebug() << "Object does not qualify to connect signals.";
		return;
	}

	QObject::disconnect(
		visual, &Visual3D::changedOpacity,
		this, &ModuleMessageEncoder::prepareSetObjectOpacity);
	QObject::disconnect(
		visual, &Visual3D::changedVisibleState,
		this, &ModuleMessageEncoder::prepareHideObject);
	QObject::disconnect(
		visual, &Visual3D::changedTransformation,
		this, &ModuleMessageEncoder::prepareTransformObject);
	QObject::disconnect(
		visual, &Visual3D::changedSceneParent,
		this, &ModuleMessageEncoder::prepareParentChange);
	if (visual->getVisualType() == EVisual::IMAGE_SLICE) {
		ImageSlice* imageSlice = qobject_cast<ImageSlice*>(visual);
		QObject::disconnect(
			imageSlice, &ImageSlice::changedImageData,
			this, &ModuleMessageEncoder::prepareDataChange);
		QObject::disconnect(
			imageSlice, &ImageSlice::changedSlice, 
			this, &ModuleMessageEncoder::prepareSetSliceIndex);
	} else if (visual->getVisualType().isStudySlice()) {
		StudySlice* studyImageSlice = qobject_cast<StudySlice*>(visual);
		QObject::disconnect(
			studyImageSlice, &StudySlice::changedStudy, 
			this, &ModuleMessageEncoder::prepareDataChange);
		QObject::disconnect(
			studyImageSlice, &StudySlice::changedSlice, 
			this, &ModuleMessageEncoder::prepareSetSliceIndex);
	} else if (visual->getVisualType().isModel()) {
		Model* model = qobject_cast<Model*>(visual);
		QObject::disconnect(
			model, &Model::changedModelData, 
			this, &ModuleMessageEncoder::prepareDataChange);
		QObject::disconnect(
			model, &Model::changedColor,
			this, &ModuleMessageEncoder::prepareSetObjectColor);
	} else if (visual->getVisualType() == EVisual::ASSEMBLY) {
		Assembly* assembly = qobject_cast<Assembly*>(visual);
		QObject::disconnect(
			assembly, &Assembly::changedAddedPart,
			this, &ModuleMessageEncoder::prepareAssemblyAddPart);
		QObject::disconnect(
			assembly, &Assembly::changedRemovedPart,
			this, &ModuleMessageEncoder::prepareAssemblyRemovePart);
		QObject::disconnect(
			assembly, &Assembly::changedPartOpacity,
			this, &ModuleMessageEncoder::prepareAssemblyPartOpacity);
		QObject::disconnect(
			assembly, &Assembly::changedPartVisible, 
			this, &ModuleMessageEncoder::prepareAssemblyPartHide);
		QObject::disconnect(
			assembly, &Assembly::changedPartTransformation, 
			this, &ModuleMessageEncoder::prepareAssemblyPartTransform);
		QObject::disconnect(
			assembly, &Assembly::changedPartData,
			this, &ModuleMessageEncoder::prepareAssemblyPartData);
		QObject::disconnect(
			assembly, &Assembly::changedPartColor, 
			this, &ModuleMessageEncoder::prepareAssemblyPartColor);
	}
}

void ModuleMessageEncoder::toJson(Visual3D* visual, QJsonObject& jsonObject) {
	if (visual == Q_NULLPTR) {
		return;
	}

	jsonObject.insert(ID, visual->getVisualID());

	// TODO: 
	// ATM, Animation variants are not supported. Convert to regular visual.
	int visualType = 0;
	switch (visual->getVisualType().toInt()) {
		case EVisual::STUDY_IMAGE_SLICE:
		case EVisual::ANIMATED_STUDY_SLICE:
			visualType = EVisual::STUDY_IMAGE_SLICE;
			break;
		case EVisual::MODEL:
		case EVisual::ANIMATED_MODEL:
			visualType = EVisual::MODEL;
			break;
		default:
			visualType = visual->getVisualType().toInt();
			break;
	}

	jsonObject.insert(TYPE, visualType);
	jsonObject.insert(IS_VISIBLE, visual->isVisible());
	jsonObject.insert(OPACITY, visual->getOpacity());

	QVariantList transform;
	transform.reserve(16);
	vtkMatrix4x4* matrix = visual->getRelativeMatrix();
	for (int i = 0; i < 16; i++) {
		transform.push_back(matrix->GetData()[i]);
	}
	jsonObject.insert(TRANSFORMATION, QJsonArray::fromVariantList(transform));

	jsonObject.insert(PARENT_ID, visual->getParentVisualName());

	if (visual->getVisualType() == EVisual::IMAGE_SLICE) {
		ImageSlice* imageSlice = qobject_cast<ImageSlice*>(visual);
		jsonObject.insert(DATA_TYPE, EData::IMAGE);
		jsonObject.insert(DATA_ID, imageSlice->getImageData()->getDataID().toString());
		jsonObject.insert(DATA_NAME, imageSlice->getImageData()->getDataName());
		jsonObject.insert(SLICE_INDEX, imageSlice->getSliceIndex());
		jsonObject.insert(SLICE_ORIENTATION, imageSlice->getSliceOrientation().toInt());
	} else if (visual->getVisualType().isStudySlice()) {
		StudySlice* studyImageSlice = qobject_cast<StudySlice*>(visual);
		jsonObject.insert(DATA_TYPE, EData::STUDY);
		jsonObject.insert(DATA_ID, studyImageSlice->getStudy()->getDataID().toString());
		jsonObject.insert(DATA_NAME, studyImageSlice->getStudy()->getDataName());
		jsonObject.insert(SLICE_INDEX, studyImageSlice->getSliceIndex());
		jsonObject.insert(SLICE_ORIENTATION, studyImageSlice->getSliceOrientation().toInt());
		jsonObject.insert(SERIES_INDEX, studyImageSlice->getSeriesIndex());
	} else if (visual->getVisualType().isModel()) {
		Model* md = qobject_cast<Model*>(visual);
		jsonObject.insert(DATA_TYPE, EData::MODEL);
		jsonObject.insert(DATA_ID, md->getModelData()->getDataID().toString());
		jsonObject.insert(DATA_NAME, md->getModelData()->getDataName());

		double r, g, b;
		md->getColor(r, g, b);
		QVariantList color;
		color.reserve(3);
		color.push_back(r);
		color.push_back(g);
		color.push_back(b);
		jsonObject.insert(COLOR, QJsonArray::fromVariantList(color));

	} else if (visual->getVisualType() == EVisual::ASSEMBLY) {
		Assembly* ass = qobject_cast<Assembly*>(visual);
		jsonObject.insert(ASSEMBLY_PART_COUNT, ass->getPartCount());
		jsonObject.insert(ASSEMBLY_ACTION, EModuleResponse::ADD_VISUAL);
		
		QList<ModelPtr> parts = ass->getParts();
		QVariantList partsInfo;
		partsInfo.reserve(parts.count());
		for (int i = 0; i < parts.count(); i++) {
			partsInfo.push_back(ModuleMessageEncoder::toJson(parts[i].data()));
		}
		jsonObject.insert(ASSEMBLY_PARTS_INFO, QJsonArray::fromVariantList(partsInfo));
	}
}

QJsonObject ModuleMessageEncoder::toJson(Visual3D* visual) {
	QJsonObject jsonObject;
	ModuleMessageEncoder::toJson(visual, jsonObject);
	return jsonObject;
}

void ModuleMessageEncoder::toJson(ModuleInteraction* moduleInteraction,
	QJsonObject& jsonObject, const bool& includeConstraintData)
{
	if (moduleInteraction == Q_NULLPTR) {
		return;
	}

	EModuleInteractionConstraint constraint = moduleInteraction->getConstraintType();
	jsonObject.insert(ID, moduleInteraction->getInteractionID());
	jsonObject.insert(TYPE, moduleInteraction->getInteractionType().toInt());
	jsonObject.insert(VALUE, moduleInteraction->getValueAsJson());
	jsonObject.insert(CONSTRAINT, constraint.toInt());
	jsonObject.insert(INFO, moduleInteraction->getInformation());

	// If the interaction is a SELECT, we need to include the options as well.
	if (moduleInteraction->getInteractionType() == EModuleInteraction::SELECT) {
		SelectInteraction* inte = qobject_cast<SelectInteraction*>(moduleInteraction);

		QVariantList options;
		QVector<QString> inteOptions = inte->getOptions();
		options.reserve(inteOptions.count());
		for (QString option:inteOptions) {
			options.append(option);
		}
		jsonObject.insert(VALUE_OPTIONS, QJsonArray::fromVariantList(options));
	}

	if (!includeConstraintData) {
		return;
	}

	switch (moduleInteraction->getInteractionType().toInt()) {
		case EModuleInteraction::UNKNOWN:
		case EModuleInteraction::VALUELESS:
		case EModuleInteraction::BOOL:
			break;
		case EModuleInteraction::INTEGER: {
			IntegerInteraction* inte = qobject_cast<IntegerInteraction*>(moduleInteraction);
			switch (constraint.toInt()) {
				case EModuleInteractionConstraint::UNKNOWN:
					break;
				case EModuleInteractionConstraint::MIN:
					jsonObject.insert(MIN_VALUE, inte->getMin());
					break;
				case EModuleInteractionConstraint::MAX:
					jsonObject.insert(MAX_VALUE, inte->getMax());
					break;
				case EModuleInteractionConstraint::RANGE:
					jsonObject.insert(MIN_VALUE, inte->getMin());
					jsonObject.insert(MAX_VALUE, inte->getMax());
				default:
					break;
			}
			break;
		}
		case EModuleInteraction::FLOAT: {
			FloatInteraction* inte = qobject_cast<FloatInteraction*>(moduleInteraction);
			switch (constraint.toInt()) {
				case EModuleInteractionConstraint::UNKNOWN:
					break;
				case EModuleInteractionConstraint::MIN:
					jsonObject.insert(MIN_VALUE, inte->getMin());
					break;
				case EModuleInteractionConstraint::MAX:
					jsonObject.insert(MAX_VALUE, inte->getMax());
					break;
				case EModuleInteractionConstraint::RANGE:
					jsonObject.insert(MIN_VALUE, inte->getMin());
					jsonObject.insert(MAX_VALUE, inte->getMax());
				default:
					break;
			}
			break;
		}
		case EModuleInteraction::STRING: 
		case EModuleInteraction::SELECT:
		default:
			break;
	}

	
}

QJsonObject ModuleMessageEncoder::toJson(ModuleInteraction* moduleInteraction, const bool& includeConstraintData) {
	QJsonObject jsonObject;
	ModuleMessageEncoder::toJson(moduleInteraction, jsonObject, includeConstraintData);
	return jsonObject;
}
