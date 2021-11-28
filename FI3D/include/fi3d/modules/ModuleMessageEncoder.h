#pragma once
/*!
* @author	VelazcoJD
* @file		ModuleMessageEncoder.h
* @class	fi3d::ModuleMessageEncoder
* @brief	Abstract class to handle module requests and send responses.
*
* Each module can receive client requests, thus every module has a
* ModuleMessageEncoder. This object does most things a module would need in
* terms of communication. That is, the visuals on the scene should be sent to
* the clients.
*
* The parse functions handle the different kind of requests that a client can
* make. Modules may have specific interactions which are parsed by the
* moduleInteraction function. This encoder does nothing with such request.
* Ideally, these interactions should be implemented by derived classes of this
* class. Thus, derive a class and override the parseModuleRequest method.
*
* The ModuleMessageEncoder keeps a list of subscribers (authenticated clients)
* that care about this module. The ModuleMessageEncoder notifies of changes
* to the Scene and module interactions. For more information, please read
* the NetworkMessageFormats.MD document.
*
* The other set of functions are the prepare functions. These functions are
* executed when something happens in the scene or one of its visuals. It
* automatically listens for their signals which prepare the corresponding
* message to update the subscribers. If there are no subscribers, the prepare
* methods don't do anything. There is one prepare method per EModuleResponse.
*/

#include <fi3d/server/MessageEncoder.h>

#include <fi3d/modules/ModuleElement.h>
#include <fi3d/modules/interactions/ValuelessInteraction.h>
#include <fi3d/modules/interactions/BooleanInteraction.h>
#include <fi3d/modules/interactions/IntegerInteraction.h>
#include <fi3d/modules/interactions/FloatInteraction.h>
#include <fi3d/modules/interactions/StringInteraction.h>

#include <fi3d/rendering/scenes/Scene.h>

#include <fi3d/server/message_keys/EModuleRequest.h>
#include <fi3d/server/message_keys/EModuleResponse.h>
#include <fi3d/server/message_keys/EResponseStatus.h>
#include <fi3d/server/message_keys/EModuleInteraction.h>
#include <fi3d/server/message_keys/EModuleInteractionConstraint.h>

#include <QJsonArray>
#include <QSharedPointer>
#include <QTimer>

namespace fi3d {
/// @brief Struct used interally to keep up with module interaction updates.
typedef struct InteractionUpdateKey {
	QString InteractionID;
	fi3d::EModuleResponse ResponseID;
	bool UpdateConstraint;
} InteractionUpdateKey;

/// @brief Compares two InteractionUpdateKey instances, needed for QHash.
inline bool operator==(const InteractionUpdateKey& k1, const InteractionUpdateKey& k2) {
	return k1.InteractionID == k2.InteractionID && k1.ResponseID == k2.ResponseID;
}

/// @brief Hashes the InteractionUpdateKey, needed for QHash.
inline uint qHash(const InteractionUpdateKey& key, uint seed) {
	return qHash(key.InteractionID, seed) ^ key.ResponseID.toInt();
}

/// @brief Struct used internally to keep up with scene updates.
typedef struct SceneUpdateKey {
	QString VisualID;
	fi3d::EModuleResponse ResponseID;
} SceneUpdateKey;

/// @brief Compares two SceneUpdateKey instances, needed for QHash.
inline bool operator==(const SceneUpdateKey& k1, const SceneUpdateKey& k2) {
	return k1.VisualID == k2.VisualID && k1.ResponseID == k2.ResponseID;
}

/// @brief Hashes the SceneUpdateKey, needed for QHash.
inline uint qHash(const SceneUpdateKey& key, uint seed) {
	return qHash(key.VisualID, seed) ^ key.ResponseID.toInt();
}

/// @brief Struct used internally  to keep up with changes to Assembly objects.
typedef struct AssembyUpdateKey : public SceneUpdateKey {
	QString PartID;
	fi3d::EModuleResponse AssemblyResponseID;
} AssembyUpdateKey;

/// @brief Compares two SceneUpdateKey instances, needed for QHash.
inline bool operator==(const AssembyUpdateKey& k1, const AssembyUpdateKey& k2) {
	return k1.VisualID == k2.VisualID && 
		k1.ResponseID == k2.ResponseID &&
		k1.PartID == k2.PartID &&
		k1.AssemblyResponseID == k2.AssemblyResponseID;
}

/// @brief Hashes the SceneUpdateKey, needed for QHash.
inline uint qHash(const AssembyUpdateKey& key, uint seed) {
	return qHash(key.VisualID, seed) ^ key.ResponseID.toInt() ^
		qHash(key.PartID, seed) ^ key.AssemblyResponseID.toInt();
}

class ModuleMessageEncoder : public fi3d::MessageEncoder, public ModuleElement, virtual public IFeedbackEmitter {

	Q_OBJECT
	Q_INTERFACES(fi3d::IFeedbackEmitter)

private:
	/// @brief List of subscribers to the module.
	QVector<QString> mSubscriberList;

	/// @brief Pointer to the scene being managed by this ModuleMessageEncoder.
	ScenePtr mScene;

	/// @brief Timer in charge of sending information to subscribers.
	QTimer mSubscriberUpdateTimer;

	/// @brief ModuleInteraction updates list as of last update.
	QHash<InteractionUpdateKey, bool> mInteractionUpdates;

	/// @brief Scene updates list as of last update.
	QHash<SceneUpdateKey, bool> mSceneUpdates;

	/// @brief Scene updates about assemblies only.
	QHash<AssembyUpdateKey, bool> mAssemblyUpdates;

public:
	/// @brief Construct a Message Encoder for a module.
	ModuleMessageEncoder();

	/// @brief Destructor.
	~ModuleMessageEncoder();

	virtual void moduleInformationAssigned(ModuleInformationPtr moduleInfo) const override;

public:
	/// @brief Parses the given request. 
	void parseRequest(const QJsonObject& request, const QString& clientID, const QString& message) override;

	/// @brief Remove from subscriber list if it's a subscriber.
	void onDisconnectedClient(const QString& clientID) override;

private:
	/// @brief Checks whether the received request has an error in it.
	bool isRequestBad(const QJsonObject& params, const QString& clientID);

protected:
	/*!
	 * @brief Parses the ModuleInteraction.
	 *
	 * This function should be overriden by derived classes to handle the
	 * ModuleInteraction being interacted with. The base function does
	 * nothing with the parsed information.
	 */
	virtual void parseModuleInteraction(const QJsonObject& params, const QString& clientID);

private:
	/*!
	*	@name Request Parsers
	*	@brief Parses a request based on its EModuleRequest type.
	*/
	/// @{
	void parseSubscribeToModule(const QJsonObject& params, const QString& clientID);
	void parseUnbuscribeToModule(const QJsonObject& params, const QString& clientID);
	void parseGetScene(const QJsonObject& params, const QString& clientID);
	void parseGetobject(const QJsonObject& params, const QString& clientID);
	void parseHideObject(const QJsonObject& params, const QString& clientID);
	void parseTranslateObject(const QJsonObject& params, const QString& clientID);
	void parseRotateObject(const QJsonObject& params, const QString& clientID);
	void parseSelectSlice(const QJsonObject& params, const QString& clientID);
	void parseSelectOrientation(const QJsonObject& params, const QString& clientID);
	void parseSelectSeries(const QJsonObject& params, const QString& clientID);
	/// @}

	/// @brief Inserts the Json keys that are common to all module messages.
	QJsonObject prepareModuleResponse(QJsonObject& moduleInfo, const QString& message = "");

private slots:
	/*!
	 * @name Prepare functions for Module Interaction changes.
	 * @brief Create responses to module actions.
	 *
	 * If there are no subscribers, nothing happens.
	 */
	/// @{
	void prepareAddInteraction(const QString& interactionID);
	void prepareRemoveInteraction(const QString& interactionID);
	void prepareChangeInteractionValue();
	void prepareChangeInteractionConstraint();
	/// @}

	/*!
	 * @name Prepare functions for Scene changes.
	 * @brief Create responses to module actions. 
	 *
	 * If there are no subscribers, nothing happens.
	 */
	/// @{
	void prepareAddObject(fi3d::VisualPtr visual);
	void prepareDataChange();
	void prepareRefreshObject();
	void prepareRemoveObject(fi3d::VisualPtr visual);
	void prepareHideObject(const bool& isVisible);
	void prepareTransformObject();
	void prepareParentChange(fi3d::Visual3DPtr visualParent);
	void prepareSetObjectOpacity(const double& a);
	void prepareSetSliceIndex();
	void prepareSetObjectColor(const double& r, const double& g, const double& b);
	void prepareAssemblyAddPart(fi3d::ModelPtr part);
	void prepareAssemblyRemovePart(fi3d::ModelPtr part);
	void prepareAssemblyPartOpacity(const double& a, fi3d::ModelPtr part);
	void prepareAssemblyPartHide(const bool& isVisible, fi3d::ModelPtr part);
	void prepareAssemblyPartTransform(fi3d::ModelPtr part);
	void prepareAssemblyPartData(fi3d::ModelDataVPtr data, fi3d::ModelPtr part);
	void prepareAssemblyPartColor(const double& r, const double& g, const double& b, fi3d::ModelPtr part);
	/// @}

private:
	/// @brief Creates the VisualInfo object based on the visual and response.
	void encodeVisualInfo(Visual3DPtr visual, const fi3d::EModuleResponse& responseType, QJsonObject& visualInfo);

	/// @brief Creates the VisualInfo object based on the visual and response.
	QJsonObject encodeVisualInfo(Visual3DPtr visual, const fi3d::EModuleResponse& responseType);

private slots:
	/// @brief Sends the Scene updates to all subscribers.
	void sendSceneUpdates();

public:
	/*! 
	 * @brief Setups the scene to start listening for updates.
	 *
	 * This function is called internally and is not needed to be called.
	 */
	void setupScene();

	/// @brief Enables/disables checking for updates on the Visual.
	void onVisualHolographicStateChange(const bool& isHoloGraphic);

	/// @brief Setups events to listen for Visual changes.
	void connectVisual3DSignals(Visual3D* visual);

	/// @brief Removes events to listen for Visual changes.
	void disconnectVisual3DSignals(Visual3D* visual);

public:
	/*!
	*	@name JSON Formatters
	*	@brief Converts the object into its JSON form.
	*/
	/// @{
	//static void toJson(fi3d::rendering::Visual3DPtr visual, EModuleResponse responseType, QJsonObject&  )
	static void toJson(Visual3D* visual, QJsonObject& jsonObject);
	static QJsonObject toJson(Visual3D* visual);
	static void toJson(ModuleInteraction* moduleInteraction, 
		QJsonObject& jsonObject, const bool& includeConstraintData = true);
	static QJsonObject toJson(ModuleInteraction* moduleInteraction, const bool& includeConstraintData = true);
	/// @}

/// Interface implementations.
signals:
	/*!
	*	@name Implementation of the IFeedbackEmitter interface.
	*/
	/// @{
    void feedback(const QString& text) override;
    void feedbackWarning(const QString& text) override;
    void feedbackError(const QString& text) override;
    void feedbackGlobal(const QString& text) override;
    void feedbackColor(const QString& text, const Qt::GlobalColor& color) override;
	/// @}
};

/// @brief Alias for a smart pointer of this class.
using ModuleMessageEncoderPtr = QSharedPointer<ModuleMessageEncoder>;

}
