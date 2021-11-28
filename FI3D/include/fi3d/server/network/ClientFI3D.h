#pragma once
/*!
* @author	VelazcoJD
* @file		ClientFI3D.h
* @class	fi3d::ClientFI3D
* @brief	Functions as a TCP client with SSL/TLS support that encodes/decodes
*			messages using the FI3D messaging protocol.
* 
* This class provides functionality to parse through the received bytes and
* interpret it as a Message formated using the FI3D protocol. For a simpler
* implentation simply using the TCP procol, use the ClientTCP class instead.
* 
* For more information, see the FI3DMessagingProtocol document.
* 
* Each message received has the following format
* 
*		X XXXX XXXX XX...XX XX...XX
*       F  L1   L2     I       P
* 
* F : payload flag
* L1: info length
* L2: payload length (ommitted if payload flag is 0)
* I : info part of message, contains L1 bytes
* P : payload part of message, contains L2 bytes
*/

#include <fi3d/server/network/ClientTCP.h>
#include <fi3d/server/network/Message.h>

namespace fi3d {
class ClientFI3D : public ClientTCP {

	Q_OBJECT

signals:
	/// @brief Emitted when a message is received.
	void messageReceived(MessagePtr message) const;

private:
	/// @brief Whether the payload flag byte has been read.
	bool mReadPayloadFlag;

	///  @brief Whether the receiving message has a payload.
	bool mHasPayload;

	///  @brief The lengths of the info and payload parts of the message.
	int mInfoLength, mPayloadLength;

	/// @brief The message being received.
	MessagePtr mReceivingMessage;

	/// @brief The message being received.
	QByteArray mReceivingBytes;

public:
	/// @brief Constructor.
	ClientFI3D();

	/// @brief Destructor.
	~ClientFI3D();

public slots:
	/// @brief Sends the given message if there is an established connection.
	void sendMessage(MessagePtr message);

private slots:
	/// @brief Handles new bytes arriving.
	void onPacket();
};

/// @brief Alias for a smart pointer of this class.
using ClientFI3DPtr = QSharedPointer<ClientFI3D>;

}