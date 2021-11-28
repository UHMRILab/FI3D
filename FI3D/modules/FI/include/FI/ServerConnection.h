#pragma once
/*!
* @author	VelazcoJD
* @file		ServerConnection.h
* @class	fi::ServerConnection
* @brief	Handles the connection with FI3D.
*/

#include <fi3d/server/network/ClientFI3D.h>

namespace fi {
class ServerConnection : public fi3d::ClientFI3D {

    Q_OBJECT

signals:
    /// @brief Emitted when the assigned password has changed.
    void changedPassword(const QString& password);

    /// @brief Emitted when the assigned client ID has changed.
    void changedClientID(const QString& clientID);

    /// @brief Emitted when the connection has been authenticated.
    void changedAuthenticated(const bool& isAuthenticated);

    /// @brief Emitted to give feedback on the connection status.
    void changedConnectionFeedback(const QString& connectionFeedback);

    /// @brief Emitted when a new application response is received.
    void newApplicationResponse(fi3d::MessagePtr response);

    /// @brief Emitted when a new data response is received.
    void newDataResponse(fi3d::MessagePtr data);

    /// @brief Emitted when a new module response is received.
    void newModuleResponse(fi3d::MessagePtr moduleInfo);

private:
    /// @brief The password to the server, defaults to "admin".
    QString mFI3DPassword;

    /// @brief The client ID assigned by the server.
    QString mClientID;

    /// @brief Whether the connection has been authenticated.
    bool mIsAuthenticated;

public:
    /// @brief Constructor.
    ServerConnection();

    /// @brief Destructor.
    ~ServerConnection();

protected slots:
    /// @brief Called when connection is lost.
    void onDisconnected();

    /// @brief Handles a message received.
    virtual void onMessage(fi3d::MessagePtr message);

    /// @brief Handles a server message.
    void onServerMessage(fi3d::MessagePtr message);

public slots:
    /// @breif Sends an application request.
    void sendApplicationRequest(QJsonObject& applicationParams, const QString& message = "");

    /// @brief Sends a module request.
    void sendModuleRequest(QJsonObject& moduleParams, const QString& message = "");

    /// @brief Sends a data request.
    void sendDataRequest(QJsonObject& dataParams, const QString& message = "");

    /// @brief Sets the password for the FI3D, closes connection if connected.
    void setConnectionPassword(const QString& password);

private:
    ///  @brief Creates a Message based on the given request with no payload.
    void sendRequest(QSharedPointer<QJsonObject> request);

public:
    /// @brief Gets the password of the FI3D.
    QString getConnectionPassword() const;

    /// @brief Gets the client ID assigned by the server.
    QString getClientID() const;

    /// @brief Gets whether the connection has been authenticated.
    bool isAuthenticated() const;
};

/// @brief Alias for a smart pointer of this class.
using ServerConnectionPtr = QSharedPointer<ServerConnection>;

}
