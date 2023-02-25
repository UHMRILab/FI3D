#include <DEMO/ModuleHandlerDEMO.h>

#include <fi3d/logger/Logger.h>

#include <fi3d/FI3D/FI3D.h>

#include <fi3d/data/Filer.h>
#include <fi3d/data/DataManager.h>

#include <fi3d/server/Server.h>

#include <fi3d/rendering/scenes/InteractiveScene.h>

#include <DEMO/GlobalsDEMO.h>

#include <QProgressDialog>
#include <QCoreApplication>

using namespace fi3d;
using namespace demo;

REGISTER_FI3D_MODULE(demo::ModuleHandlerDEMO, "Demonstration", "DEMO")

ModuleHandlerDEMO::ModuleHandlerDEMO(const QString& moduleID)
	: ModuleHandler(sModuleName, sModuleAcronym, moduleID),
	mGUI(new ModuleWindowDEMO()),
	mSlice(Q_NULLPTR), mEndocardium(Q_NULLPTR), mEpicardium(Q_NULLPTR),
	mDataAnimationTimer(new QTimer(this)), mAnimationSpeed(45)
{
	//Ensure the data folder is created
	Filer::checkAndCreateDirectory(this->getModuleDataPath());

	QProgressDialog loaderMessage("Loading DEMO...", "Abort", 0, 100);
	loaderMessage.setCancelButton(Q_NULLPTR); // No cancel
	loaderMessage.setWindowModality(Qt::WindowModal);
	loaderMessage.show();
	QCoreApplication::processEvents();

	// Create and set a default scene.
	InteractiveScenePtr scene(new InteractiveScene(mGUI->getSceneWidget(), moduleID));
	scene->setBackgroundGradientColor(0.5, 0.5, 0.5, 0.9, 0.9, 0.9);
	scene->setAxesVisible(false);
	this->setScene(scene);

	SubtitlePtr brand = scene->addSubtitle("Brand", "FI3D, MRI Lab, University of Houston, NSF Funded");
	brand->setLocationBottomLeft();
	brand->setColor(1, 1, 0);
	
	loaderMessage.setLabelText("Loading Images...");
	loaderMessage.setValue(10);
	StudyPtr study = Filer::readStudyFromFI3DFile(tr("%1/Heart_fixed.FI3D").arg(this->getModuleAssetsPath()));
	study->setStudyID(tr("DEMO_%1").arg(study->getStudyID()));
	DataManager::registerStudy(study);

	mSlice = scene->addAnimatedStudySlice("StudySlice");
	mSlice->setStudy(study);
	mSlice->setSliceIndex(5);
	mSlice->setSeriesIndex(0);
	mSlice->setInteractable(false);
	mSlice->setExternalTimer(mDataAnimationTimer.data());

	loaderMessage.setLabelText("Loading Surfaces...");
	loaderMessage.setValue(60);
	AnimatedModelDataPtr endoData(new AnimatedModelData());
	AnimatedModelDataPtr epiData(new AnimatedModelData());
	for (int i = 0; i < 25; i++) {
		ModelDataVPtr endo = Filer::readModelDataFromSTL(tr("%1/Endocardium_%2.stl").arg(this->getModuleAssetsPath()).arg(i));
		endoData->addAnimationFrame(endo);
		DataManager::registerModelData(endo);

		ModelDataVPtr epi = Filer::readModelDataFromSTL(tr("%1/Epicardium_%2.stl").arg(this->getModuleAssetsPath()).arg(i));
		epiData->addAnimationFrame(epi);
		DataManager::registerModelData(epi);
	}

	mEndocardium = scene->addAnimatedModel("Endocardium", endoData);
	mEndocardium->setFrame(0);
	mEndocardium->setColor(1, 0, 0);
	mEndocardium->setOpacity(0.7);
	mEndocardium->setInteractable(false);

	mEpicardium = scene->addAnimatedModel("Epicardium", epiData);
	mEpicardium->setFrame(0);
	mEpicardium->setColor(1, 1, 0);
	mEpicardium->setOpacity(0.4);
	mEpicardium->setInteractable(false);

	loaderMessage.setLabelText("Applying Final Touches...");
	loaderMessage.setValue(90);

	// Set the GUI
	mGUI->updateIP(Server::GetIPAddress());
	mGUI->updatePort(Server::GetPortNumber());
	mGUI->updateNumberOfConnectedFI(Server::GetHMDCount());
	this->setModuleWindow(mGUI);

	// Set up interactions
	this->setUpInteractions();

	// TODO: Add additional objects here

	// Set up connections
	this->setUpConnections();

	// TODO: Add any other initiation logic here

	// Load configuration.
	this->applyConfig();

	loaderMessage.close();
}

ModuleHandlerDEMO::~ModuleHandlerDEMO() {}

void ModuleHandlerDEMO::showModuleWindow() {
	if (!this->getModuleWindow().isNull()) {
		this->getModuleWindow()->showNormal();
	}
}

void ModuleHandlerDEMO::setAnimationEnable(const bool& isEnable) {
	if (mDataAnimationTimer->isActive() == isEnable) {
		return;
	}

	if (isEnable && !mDataAnimationTimer->isActive()) {
		mDataAnimationTimer->start(mAnimationSpeed);
	} else {
		mDataAnimationTimer->stop();
	}
}

void ModuleHandlerDEMO::updateConnectionNumbers() {
	mGUI->updateNumberOfConnectedFI(Server::GetHMDCount());
}

void ModuleHandlerDEMO::setUpInteractions() {
	// Change the cardiac slice interaction
	IntegerInteractionPtr slice = IntegerInteraction::createRangeInteraction(MI_SLICE, 5, 0, 9);
	this->addModuleInteraction(slice);

	// Change the cardiac phase interaction
	IntegerInteractionPtr phase = IntegerInteraction::createRangeInteraction(MI_PHASE, 0, 0, 24);
	this->addModuleInteraction(phase);

	// Enables visualization of segmentation mesh
	BooleanInteractionPtr surface(new BooleanInteraction(MI_ENABLE_SURFACE, true));
	this->addModuleInteraction(surface);

	// Animate the cardiac cycle interaction
	BooleanInteractionPtr animate(new BooleanInteraction(MI_ANIMATE, false));
	this->addModuleInteraction(animate);
}

void ModuleHandlerDEMO::setUpConnections() {
	// Set up the connections regarding the module interactions
	auto slice = this->getIntegerModuleInteraction(MI_SLICE);
	QObject::connect(
		slice.data(), &IntegerInteraction::changedValue,
		mSlice.data(), &AnimatedStudySlice::setSliceIndex);
	QObject::connect(
		mSlice.data(), &AnimatedStudySlice::changedSlice,
		slice.data(), &IntegerInteraction::setValue);
	QObject::connect(
		slice.data(), &IntegerInteraction::changedValue,
		mGUI.data(), &ModuleWindowDEMO::updateSlice);

	auto phase = this->getIntegerModuleInteraction(MI_PHASE);
	QObject::connect(
		phase.data(), &IntegerInteraction::changedValue,
		mSlice.data(), &AnimatedStudySlice::setSeriesIndex);
	QObject::connect(
		mSlice.data(), &AnimatedStudySlice::changedSeriesIndex,
		phase.data(), &IntegerInteraction::setValue);
	QObject::connect(
		phase.data(), &IntegerInteraction::changedValue,
		mGUI.data(), &ModuleWindowDEMO::updatePhase);

	auto surface = this->getBooleanModuleInteraction(MI_ENABLE_SURFACE);
	QObject::connect(
		surface.data(), &BooleanInteraction::changedValue,
		mEndocardium.data(), &AnimatedModel::setVisible);
	QObject::connect(
		surface.data(), &BooleanInteraction::changedValue,
		mEpicardium.data(), &AnimatedModel::setVisible);
	QObject::connect(
		surface.data(), &BooleanInteraction::changedValue,
		mGUI.data(), &ModuleWindowDEMO::updateSurfaceEnable);

	auto animate = this->getBooleanModuleInteraction(MI_ANIMATE);
	QObject::connect(
		animate.data(), &BooleanInteraction::changedValue,
		this, &ModuleHandlerDEMO::setAnimationEnable);
	QObject::connect(
		animate.data(), &BooleanInteraction::changedValue,
		mGUI.data(), &ModuleWindowDEMO::updateAnimateEnable);

	// Set up other events
	QObject::connect(
		mSlice.data(), &AnimatedStudySlice::changedSeriesIndex,
		mEndocardium.data(), &AnimatedModel::setFrame);
	QObject::connect(
		mSlice.data(), &AnimatedStudySlice::changedSeriesIndex,
		mEpicardium.data(), &AnimatedModel::setFrame);

	const Server* server = Server::getInstance();
	QObject::connect(
		server, &Server::changedClientIdentified,
		this, &ModuleHandlerDEMO::updateConnectionNumbers);
	QObject::connect(
		server, &Server::changedClientDisconnected,
		this, &ModuleHandlerDEMO::updateConnectionNumbers);
}

void ModuleHandlerDEMO::applyConfig() {
	QJsonObject config = this->loadConfig();

	// TODO: Use the loaded config to load default/saved configuration.
}

void ModuleHandlerDEMO::makeConfig() {
	QJsonObject config;

	// TODO: Save configuration to config object to save.
	
	this->saveConfig(config);
}