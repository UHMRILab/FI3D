#pragma once
/*!
* @author	VelazcoJD
* @file		SceneFI.h
* @class	FI::SceneFI
* @brief	The scene for the Framework Interface (FI) module.
*/

#include <fi3d/rendering/scenes/InteractiveScene.h>

#include <FI/data/DataCache.h>

#include <QJsonObject>

namespace fi {
class SceneFI : public fi3d::InteractiveScene {

	Q_OBJECT
		
signals:
	/// @brief Emitted when a module request is ready.
	void moduleRequest(QJsonObject& request, const QString& message);

private:
	/// @brief The ID of the scene subscribed to.
	QString mSubscribedSceneID;

	/// @brief The object managing the cached data.
	DataCachePtr mCache;

public:
	/// @brief Constructor.
	SceneFI(DataCachePtr cache, QVTKOpenGLStereoWidget* widget, const QString& id = "");

	/// @brief Destructor.
	~SceneFI();

public slots:
	/// @brief Process a module response.
	void handleModuleResponse(const QJsonArray& moduleInfo);

private:
	/*!
	* @name Server request/response message keys.
	*/
	/// @{
	void handleAddVisual(const QJsonObject& visualInfo);
	void handleRefreshVisual(const QJsonObject& visualInfo);
	void handleRemoveVisual(const QJsonObject& visualInfo);
	void handleDataChange(const QJsonObject& visualInfo);
	void handleHideVisual(const QJsonObject& visualInfo);
	void handleTransformVisual(const QJsonObject& visualInfo);
	void handleParentChange(const QJsonObject& visualInfo);
	void handleSetVisualOpacity(const QJsonObject& visualInfo);
	void handleSetSlice(const QJsonObject& visualInfo);
	void handleSetColor(const QJsonObject& visualInfo);
	/// @}

private slots:
	/// @brief Handles a slice being changed (to request new data).
	void onSliceChange(const int& sliceIndex, const fi3d::ESliceOrientation orientation);
};

/// @brief Alias for a smart pointer of this class.
using SceneFIPtr = QSharedPointer<SceneFI>;

}