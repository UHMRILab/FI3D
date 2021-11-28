#pragma once
/*!
* @author	VelazcoJD
* @file		Scene.h
* @class	fi3d::Scene
* @brief	A Scene manages all Visual instances that are rendered together. 
*
* The Scene manages all Visual instances visualized in a single scene.
* There are many functions available to manipulate the visuals. Alternatively, 
* one can get the Visual by its name to do manipulations. By default, 
* the Scene has an axes glyph used to visualize the origin (0, 0, 0). 
*/

#include <fi3d/modules/ModuleElement.h>
#include <fi3d/modules/IFeedbackEmitter.h>

#include <fi3d/rendering/visuals/2D/Subtitle.h>
#include <fi3d/rendering/visuals/3D/slices/AnimatedStudySlice.h>
#include <fi3d/rendering/visuals/3D/models/Assembly.h>
#include <fi3d/rendering/visuals/3D/models/AnimatedModel.h>

#include <vtkSmartPointer.h>

class QVTKOpenGLWidget;
class QVTKInteractor;

class vtkAxesActor;
class vtkEventQtSlotConnect;
class vtkGenericOpenGLRenderWindow;
class vtkInteractorStyle;
class vtkRenderer;

namespace fi3d {
class Scene : public QObject, public fi3d::ModuleElement, public IFeedbackEmitter {

	Q_OBJECT
	Q_INTERFACES(fi3d::IFeedbackEmitter)

signals:
	/// @brief Emitted when a Visual has been added.
	void changedAddedSceneObject(VisualPtr sceneObject);

	/// @brief Emitted when a Visual was removed.
	void changedRemovedSceneObject(VisualPtr sceneObject);

	/// @brief Emitted when the Scene's background color was changed.
	void changedBackgroundColor(const double& r, const double& g, const double& b);

	/// @brief Emitted when the Scene's axes' visibility was changed.
	void changedAxesVisibility(const bool& isVisible);

	/// @brief Emitted when the Scene's view was reset.
	void changedCameraReset();

protected:
	/// @brief The Scene's ID, used to identify the scene.
	QString mSceneID;

	/// @brief The widget that this scene is assigned to.
	QVTKOpenGLWidget* mWidget;

	/// @brief The render window.
	vtkSmartPointer<vtkGenericOpenGLRenderWindow> mRenderWindow;

	/// @brief The renderer of the window.
	vtkSmartPointer<vtkRenderer> mRenderer;

	/// @brief The scene's interactor.
	vtkSmartPointer<QVTKInteractor> mInteractor;

	/// @brief The default interactor style.
	vtkSmartPointer<vtkInteractorStyle> mInteractorStyle;

	/// @brief VTK slot connector used to connect to window events.
	vtkSmartPointer<vtkEventQtSlotConnect> mSlotConnector;

	/// @brief A, XYZ glyph to represent the origin.
	vtkSmartPointer<vtkAxesActor> mAxesVisual;

	/// @brief Whether the axes are visualized or not, on by default.
	bool mAreAxesVisible;

	/// @brief Organizes all visuals in the Scene.
	QHash<QString, VisualPtr> mVisuals;

	/// @brief Organizes all 3D visuals in the Scene.
	QHash<QString, Visual3DPtr> mVisuals3D;

	/*!
	*	@name Specific Visual Tables
	*	@brief Hash tables used to manage the different type of visuals.
	*/
	/// @{
	QHash<QString, ImageSlicePtr> mImageSlices;
	QHash<QString, StudySlicePtr> mStudySlices;
	QHash<QString, ModelPtr> mModels;
	QHash<QString, AnimatedStudySlicePtr> mAnimatedStudySlices;
	QHash<QString, AnimatedModelPtr> mAnimatedModels;
	QHash<QString, AssemblyPtr> mAssemblies;
	QHash<QString, SubtitlePtr> mSubtitles;
	/// @}

	/// @brief Hash table used to find a Visual based on its vtkProp.
	QHash<vtkProp*, QString> mActorsToNames;

	/// @brief The time of the last render call.
	int mTimeFromLastRender;

	/// @brief Whether a render call is scheduled.
	bool mIsRenderScheduled;

public:
	/*!
	* @brief Constructor of the scene.
	*
	* @param widget The QtVTKOpenGLWidget that this scene will use to 
	*			render the visuals.
	* @param name The name of this scene.
	*/
	Scene(QVTKOpenGLWidget* widget, const QString& id);

	/// @brief Destructor. */
	~Scene();

public slots:
	/// @brief Gets the Scene's ID.
	QString getSceneID() const;

	/*!
	* @name Visual Adders
	* @brief Creates a Visual and adds it to the Scene.
	* 
	* The created Visual is returned. If the object cannot be added, 
	* null is returned.
	*/
	/// @{
	virtual VisualPtr addVisual(const QString& visualID, const EVisual& objectType);
			
	virtual ImageSlicePtr addImageSlice(const QString& visualID,
		const ESliceOrientation& orientation = ESliceOrientation::XY,
		ImageDataVPtr data = ImageDataVPtr::New());
			
	virtual StudySlicePtr addStudyImageSlice(const QString& visualID,
		const ESliceOrientation& orientation = ESliceOrientation::XY,
		StudyPtr data = Q_NULLPTR);
			
	virtual ModelPtr addModel(const QString& visualID, ModelDataVPtr modelData = ModelDataVPtr::New());

	virtual AnimatedStudySlicePtr addAnimatedStudySlice(const QString& visualID,
		const ESliceOrientation& orientation = ESliceOrientation::XY,
		StudyPtr study = Q_NULLPTR);

	virtual AnimatedModelPtr addAnimatedModel(const QString& visualID, 
		AnimatedModelDataPtr data = AnimatedModelDataPtr());

	virtual AssemblyPtr addAssembly(const QString& visualID);
			
	virtual SubtitlePtr addSubtitle(const QString& visualID, const QString& text = "");
	/// @}

	/*! 
		* @brief Add a pre-constructed Visual.
		*
		* @param visual The Visual to add to the scene.
		* @return true if it was added successfully, false if it failed to 
		*		add (name taken). 
		*/
	virtual bool addVisual(VisualPtr visual);

public:
	/// @brief Checks whether the given ID is taken in the Scene.
	bool isVisualIDTaken(const QString& visualID);

	/*!
	*	@name Visual Getters
	*	@brief Gets a Visual identified by the given visual ID.
	*/
	/// @{
	virtual VisualPtr getVisual(const QString& visualID);

	virtual Visual3DPtr getVisual3D(const QString& visualID);

	virtual ImageSlicePtr getImageSlice(const QString& visualID);

	virtual StudySlicePtr getStudyImageSlice(const QString& visualID);

	virtual ModelPtr getModel(const QString& visualID);

	virtual AnimatedStudySlicePtr getAnimatedStudySlice(const QString& visualID);

	virtual AnimatedModelPtr getAnimatedModel(const QString& visualID);

	virtual AssemblyPtr getAssembly(const QString& visualID);

	virtual SubtitlePtr getSubtitle(const QString& visualID);
	/// @}

	/// @brief Used to find the name of a Visual by the vtkProp.
	QString getVisualIDByProp(vtkProp* prop) const;

	/// @brief Get the EVisual type of the Visual with the given id.
	virtual EVisual getVisualType(const QString& visualID) const;

	/*!
	*	@name Visual List Getters
	*	@brief Gets a list of all visuals of a certain type.
	*/
	/// @{
	virtual QList<VisualPtr> getVisuals();
	virtual QList<Visual3DPtr> getVisuals3D();
	virtual QList<ImageSlicePtr> getImageSlices();
	virtual QList<StudySlicePtr> getStudySlices();
	virtual QList<ModelPtr> getModels();
	virtual QList<AnimatedStudySlicePtr> getAnimatedStudySlices();
	virtual QList<AnimatedModelPtr> getAnimatedModels();
	virtual QList<AssemblyPtr> getAssemblies();
	virtual QList<SubtitlePtr> getSubtitles();
	/// @}

	/*!
	*	@name Visual Name List Getters
	*	@brief Gets a list with all the IDs of the visuals of a 
	*		certain type.
	*/
	/// @{
	virtual QList<QString> getVisualList() const;
	virtual QList<QString> getVisual3DList() const;
	virtual QList<QString> getImageSliceList() const;
	virtual QList<QString> getStudySliceList() const;
	virtual QList<QString> getModelList() const;
	virtual QList<QString> getAnimatedStudySliceList() const;
	virtual QList<QString> getAnimatedModelList() const;
	virtual QList<QString> getAssemblyList() const;
	virtual QList<QString> getSubtitleList() const;
	/// @}

public slots:
	/// @brief Removes the Visual with the given name.
	virtual void removeVisual(const QString& visualID);

	/// @brief Removes all SceneObjects.
	virtual void clearScene();

	/*!
	* @name Visual Modifiers
	* @brief These calls change the properties of a Visual.
	*
	* All these methods update Visual properties without having to get the 
	* Visual and applying the change manually. The Visual is identified based 
	* on the given ID. If the ID does not match a Visual in the Scene, nothing 
	* happens.
	*/
	/// @{
	virtual void setVisualVisible(const bool& isVisible, const QString& visualID);

	virtual void setVisualPickable(const bool& isPickable, const QString& visualID);

	virtual void setVisualOpacity(const double& a, const QString& visualID);

	virtual void resetVisualTransform(const QString& visualID);

	virtual void translateVisual(const double& x, const double& y, const double& z, const QString& visualID);

	virtual void rotateVisual(const double& x, const double& y, const double& z, const QString& visualID);

	virtual void rotateXVisual(const double& x,  const QString& visualID);

	virtual void rotateYVisual(const double& y, const QString& visualID);

	virtual void rotateZVisual(const double& z, const QString& visualID);

	virtual void setImageSliceData(vtkSmartPointer<fi3d::ImageData> data, const QString& visualID);

	virtual void setImageSlice(const int& sliceIndex, 
		const fi3d::ESliceOrientation& orientation, const QString& visualID);

	virtual void setImageSliceIndex(const int& index, const QString& visualID);

	virtual void setImageSliceOrientation(const fi3d::ESliceOrientation& orientation, const QString& visualID);

	virtual void setStudySliceData(fi3d::StudyPtr data, const QString& visualID); 

	virtual void setStudySliceSeriesIndex(const int& index, const QString& visualID);

	virtual void setModelData(fi3d::ModelDataVPtr data, const QString& visualID);

	virtual void setModelColor(const double& r, const double& g, const double& b, const QString& visualID);

	virtual void addPartToAssembly(fi3d::ModelPtr part, const QString& visualID);

	virtual void removePartFromAssembly(const QString& partID, const QString& visualID);
	/// @}

protected:
	/// @brief Change the interactor style in the Scene.
	virtual void setInteractorStyle(vtkSmartPointer<vtkInteractorStyle> interactorStyle);

public slots:
	/// @brief Changes the background color of the Scene
	virtual void setBackgroundColor(const double& r, const double& g, const double& b);

	/// @brief Changes the background color of the scene to a gradient
	virtual void setBackgroundGradientColor(
		const double& r1, const double& g1, const double& b1,
		const double& r2, const double& g2, const double& b2);

public:
	/// @brief Gets the scene's background color.
	virtual void getBackgroundColor(double& r, double& g, double& b) const;

public slots:
	/// @brief Sets the visibility of the frame's axes.
	virtual void setAxesVisible(const bool& isVisible);

	/*! 
		* @brief Whether the labels (x, y, z) should be rendered next to 
		*	the axes. 
		*
		* NOTE: If the axes are not visible, the labels will not be visible
		* even if set to true. 
		*/
	virtual void setAxesLabelsVisible(const bool& isVisible);

	/// @brief Sets the length of each axes.
	virtual void setAxesLength(const double& x, const double& y, const double& z);

public:
	/// @brief Whether the axes are visible or not.
	virtual bool getAxesVisible() const;

public slots:
	/// @brief Renders the window.
	virtual void render();

	/// @brief Resets and renders the window.
	virtual void resetCamera();

public:
	/// @brief Given the a coordinate (x, y, z), compute its location 
	/// on the display
	void worldToDisplay(const double& x, const double& y, const double&z, double& i, double& j);

private:
	/// @brief Adds the Visual to the hash tables it belongs to.
	void addToManagement(VisualPtr visual);

	/// @brief Removes the Visual from the hash tables it belonged to.
	void removeFromManagement(VisualPtr visual);

	/*!
	* @name Visual Signal Connectors
	* @brief Connects visual signals.
	*
	* These signals help the scene automatically render every time a 
	* Visual changes one of its properties.
	*/
	/// @{
	void connectVisualSignals(Visual* visual);
	void connectVisual3DSignals(Visual3D* visual);
	void connectImageSliceSignals(ImageSlice* imageSlice);
	void connectStudySliceSignals(StudySlice* image);
	void connectModelSignals(Model* model);
	void connectAssemblySignals(Assembly* assembly);
	void connectSubtitleSignals(Subtitle* subtitle);
	/// @}

	/*!
	* @name Visual Signal Dis-connectors
	* @brief Disconnects Visual signals. 
	* 
	* Used to remove the signals of a Visual that will be removed from 
	* the scene.
	*/
	/// @{
	void disconnectVisualSignals(Visual* visual);
	void disconnectVisual3DSignals(Visual3D* visual);
	void disconnectImageSliceSignals(ImageSlice* imageSlice);
	void disconnectStudySliceSignals(StudySlice* image);
	void disconnectModelSignals(Model* model);
	void disconnectAssemblySignals(Assembly* assembly);
	void disconnectSubtitleSignals(Subtitle* subtitle);
	/// @}

signals:
	/*!
	*	@name Implementation of the IFeedbackEmitter interface.
	*/
	/// @{
	void feedback(const QString& text);
	void feedbackWarning(const QString& text);
	void feedbackError(const QString& text);
	void feedbackGlobal(const QString& text);
	void feedbackColor(const QString& text, const Qt::GlobalColor& color);
	/// @}
};

/// @brief Alias for a smart pointer of this class.
using ScenePtr = QSharedPointer<Scene>;

}