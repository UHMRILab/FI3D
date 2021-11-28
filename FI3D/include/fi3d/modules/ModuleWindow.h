#pragma once
/*!
* @author	VelazcoJD
* @file		ModuleWindow.h
* @class	fi3d::ModuleWindow
* @brief	Abstract class defining basic requirements for a tab page.
*
* Every module has a GUI to render its visuals and forms of input. This class 
* defines what all these modules have in common. 
*/

#include <fi3d/modules/ModuleElement.h>

#include <QMainWindow>
#include <QVTKOpenGLWidget.h>

namespace fi3d {
/// @brief Forward declare the IFeedbackEmitter interface.
class IFeedbackEmitter;

class ModuleWindow : public QMainWindow, public ModuleElement {

	Q_OBJECT

signals:
	/// @brief Propagates the global messages
	void feedbackGlobal(const QString& message);

	/// @brief Emits a command based on user input.
	void executeCommand(const QString& commandString);

protected:
	/// @brief Constructor.
	ModuleWindow();

public:
	/// @brief Destructor. 
	~ModuleWindow();

	/// @brief Connects the feedback emitter to display the feedback.
	virtual void connectFeedbackEmitter(IFeedbackEmitter* emitter);

	/// @brief Disconnects the feedback emmitter.
	virtual void disconnectFeedbackEmitter(IFeedbackEmitter* emitter);

public slots:
	/// @brief Displays the feedback for the user. 
	virtual void handleFeedback(const QString& message, const  QColor& color);

	/// @brief Handles general feedback.
	virtual void feedback(const QString& message);

	/// @brief Handles error feedback.
	virtual void feedbackError(const QString& message);

	/// @brief Handles warning feedback.
	virtual void feedbackWarning(const QString& message);

	/// @brief Handles custom-color feedback.
	virtual void feedbackColor(const QString& message, const Qt::GlobalColor& color);

protected:
	/// @brief Close the module if the ModuleWindow is closed.
	virtual void closeEvent(QCloseEvent* event) override;
};

/// @brief Alias for a smart pointer of this class.
using ModuleWindowPtr = QSharedPointer<ModuleWindow>;

}