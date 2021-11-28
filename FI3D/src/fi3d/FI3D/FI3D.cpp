#include <fi3d/FI3D/FI3D.h>

#include <fi3d/logger/Logger.h>

#include <fi3d/FI3D/FI3DController.h>

#include <fi3d/data/Filer.h>

#include <fi3d/rendering/visuals/3D/slices/ESliceOrientation.h>
#include <fi3d/rendering/visuals/EVisual.h>

#include <fi3d/server/Server.h>

#include <QtWidgets/QApplication>

#include <QAction>
#include <QGamepadManager>
#include <QFile>
#include <QIcon>
#include <QProgressDialog>
#include <QSurfaceFormat>
#include <QVTKOpenGLWidget.h>


#ifdef FI3D_ASSETS_PATH
    const QString fi3d::FI3D::ASSETS_DIRECTORY = FI3D_ASSETS_PATH;
#else
    const QString fi3d::FI3D::ASSETS_DIRECTORY = QCoreApplication::applicationDirPath();
#endif

#ifdef FI3D_DATA_PATH
    const QString fi3d::FI3D::DATA_DIRECTORY = FI3D_DATA_PATH;
#else
    const QString fi3d::FI3D::DATA_DIRECTORY = QCoreApplication::applicationDirPath();
#endif

#ifdef FI3D_MODULES_ASSETS_PATH
	const QString fi3d::FI3D::MODULES_ASSETS_DIRECTORY = FI3D_MODULES_ASSETS_PATH;
#else
	const QString fi3d::FI3D::ASSETS_DIRECTORY = QCoreApplication::applicationDirPath();
#endif

#ifdef FI3D_MODULES_DATA_PATH
	const QString fi3d::FI3D::MODULES_DATA_DIRECTORY = FI3D_MODULES_DATA_PATH;
#else
	const QString fi3d::FI3D::DATA_DIRECTORY = QCoreApplication::applicationDirPath();
#endif

using namespace fi3d;

FI3D::FI3D() {}

FI3D::~FI3D() {}

int FI3D::run(int argc, char *argv[]) {
	QApplication qApplication(argc, argv);

	QGamepadManager::instance();
	qApplication.processEvents();

	QProgressDialog loaderMessage("Setting Utilities...", "Abort", 0, 100);
	loaderMessage.setCancelButton(Q_NULLPTR);	// no cancel button
	loaderMessage.setWindowModality(Qt::WindowModal);
	loaderMessage.show();
	QCoreApplication::processEvents();

	// Create the data directory if it does not exist.
	Filer::checkAndCreateDirectory(DATA_DIRECTORY);
	Filer::checkAndCreateDirectory(QObject::tr("%1/FI3D").arg(DATA_DIRECTORY));
	Filer::checkAndCreateDirectory(QObject::tr("%1/components").arg(DATA_DIRECTORY));
	Filer::checkAndCreateDirectory(QObject::tr("%1/modules").arg(DATA_DIRECTORY));

	// setup the logger
	Logger::init();
	QSurfaceFormat::setDefaultFormat(QVTKOpenGLWidget::defaultFormat());

	// Inform Qt of our variants, TODO: these 2 need to be removed.
	qRegisterMetaType<fi3d::ESliceOrientation>("ESliceOrientation");
	qRegisterMetaType<fi3d::EVisual>("EVisual");

    // Create data directories if they are non-existent
    if (Filer::checkAndCreateDirectory(DATA_DIRECTORY)) {
		QString dir = QObject::tr("%1/%2").arg(DATA_DIRECTORY).arg("FI3D");
        if(!Filer::checkAndCreateDirectory(dir)) {
            qWarning() << "Failed to create FI3D data directory. Several functionalities will not work.";
        }
    } else {
		qWarning() << "Failed to create data directory. Several functionalities will not work";
    }
		
	loaderMessage.setLabelText("Loading Pretty Interfaces...");
	loaderMessage.setValue(50);

	// Load the application theme
	QFile styleSheetFile(QObject::tr("%1/FI3D/theme.qss").arg(FI3D::ASSETS_DIRECTORY));
	styleSheetFile.open(QFile::ReadOnly);
	QString styleSheet = QLatin1String(styleSheetFile.readAll());
	qApplication.setStyleSheet(styleSheet);

	qInfo() << "Assets:" << ASSETS_DIRECTORY;
	qInfo() << "Data  :" << DATA_DIRECTORY;
	qInfo() << "Module Assets:" << MODULES_ASSETS_DIRECTORY;
	qInfo() << "Module Data  :" << MODULES_DATA_DIRECTORY;
	qInfo() << "SSL Compilation Version: " << QSslSocket::sslLibraryBuildVersionString();
	qInfo() << "SSL Current Version: " << QSslSocket::sslLibraryVersionString();
	qInfo() << "SSL Enabled: " << QSslSocket::supportsSsl();

	// Start the main FI3D application and main window
	FI3DController::init();
	FI3DController::showMainWindow();

	loaderMessage.setValue(100);

	int appCloseState = qApplication.exec();
    
	FI3DController::clean();
	fi3d::Logger::clean();
	return appCloseState;
}