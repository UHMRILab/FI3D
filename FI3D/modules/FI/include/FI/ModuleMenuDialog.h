#pragma once
/*!
* @author	VelazcoJD
* @file		ModuleMenuDialog.h
* @class	fi::ModuleMenuDialog
* @brief	Initial GUI displaying the available and active modules.
*/

#include <fi3d/modules/ModuleElement.h>

#include <QDialog>

namespace Ui {
class ModuleMenuDialog;
}

namespace fi {
class ModuleMenuDialog : public QDialog, public fi3d::ModuleElement {
    
    Q_OBJECT

signals:
    /// @brief Emitted when the dialog is closing.
    void dialogClosing();

private:
    ///  @brief The GUI elements.
    Ui::ModuleMenuDialog* mGUI;

    ///  @brief Whether the available module buttons have been populated.
    bool mAvailableModulesSet;

public:
    /// @brief Constructor.
    ModuleMenuDialog(QWidget* parent = nullptr);

    /// @brief Destructor.
    ~ModuleMenuDialog();

public:
    /// @brief Sets the available modules.
    void setAvailableModules(const QList<QString>& availableModules);

    /// @brief Sets the active modules.
    void setActiveModules(const QList<QString>& activeModules);

private:
    /// @brief Clears the buttons representing available modules.
    void clearAvailableModuleButtons();

    /// @brief Clears the buttons representing active modules.
    void clearActiveModuleButtons();

private slots:
    /// @brief Handles the user clicking on an available module button.
    void onAvailableModuleClick();

    /// @brief Handles the user clicking on an active module button.
    void onActivateModuleClick();

    /// @brief Handles the user clicking on a stop module button.
    void onStopModuleClick();

    /// @brief Handles the user clicking on the connect button.
    void onConnectClick();

    /// @brief Handles the user clicking on the disconnect button.
    void onDisconnectClick();

protected:
    /// @brief Handles the dialog closing.
    virtual void closeEvent(QCloseEvent* event) override;

    /// @brief Listens for the enter key to connect to the server.
    virtual void keyPressEvent(QKeyEvent* event) override;

public slots:
    /// @brief Provides a message feedback to the user.
    void setConnectionFeedback(const QString& feedback);

    /*!
    *	@name GUI Update Functions.
    *	@brief Updates GUI elements.
    */
    /// @{
    void updateIP(const QString& IP);
    void updatePort(const int& port);
    void updatePassword(const QString& password);
    void updateConnected();
    void updateDisconnected();
    /// @}
};

/// @brief Alias for a smart pointer of this class.
using ModuleMenuDialogPtr = QSharedPointer<ModuleMenuDialog>;

}
