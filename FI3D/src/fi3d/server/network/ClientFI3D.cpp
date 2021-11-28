#include <fi3d/server/network/ClientFI3D.h>

#include <QJsonDocument>
#include <QtEndian>

using namespace fi3d;

ClientFI3D::ClientFI3D()
	: ClientTCP(),
	mReadPayloadFlag(false),
	mHasPayload(false),
	mInfoLength(-1),
	mPayloadLength(-1),
	mReceivingMessage(new Message()),
	mReceivingBytes()
{
	QObject::connect(
		this, &ClientFI3D::readyRead,
		this, &ClientFI3D::onPacket);
}

ClientFI3D::~ClientFI3D() {}

void ClientFI3D::sendMessage(MessagePtr message) {
	qDebug() << "Enter";
	QSharedPointer<QJsonObject> infoJson = message->getInfo();
	QJsonDocument doc(*infoJson.data());
	QByteArray info = doc.toJson(QJsonDocument::JsonFormat::Compact);

	QSharedPointer<QByteArray> payload = message->getPayload();

	int infoCount = info.count();
	int payloadCount = payload->count();

	int byteCount = sizeof(qint32) + 1;
	byteCount += infoCount;
	if (message->hasPayload()) {
		byteCount += sizeof(qint32);
	}

	QByteArray bytes;
	bytes.reserve(byteCount);

	bytes.append(message->hasPayload());
	bytes.append((const char*)&infoCount, sizeof(qint32));
	if (message->hasPayload()) {
		bytes.append((const char*)&payloadCount, sizeof(qint32));
	}
	bytes.append(info);

	this->write(bytes);
	if (message->hasPayload()) {
		this->write(*payload.data());
	}

	qDebug() << "Message sent: WithPayload=" << message->hasPayload() <<
		"InfoLen=" << infoCount << ", PayloadLen=" << payloadCount;
	qDebug() << "Exit";
}

void ClientFI3D::onPacket() {
	qDebug() << "Enter";
	int packetSize = this->bytesAvailable();
	qDebug() << "Available bytes:" << packetSize;
	while (packetSize != 0) {
		if (!mReadPayloadFlag) {
			mHasPayload = (bool)this->read(1).at(0);
			mReadPayloadFlag = true;

			qDebug() << "New message being received. Payload=" << mHasPayload;
			packetSize -= 1;
			if (packetSize == 0) {
				break;
			}
		}

		if (mInfoLength == -1) {
			if (packetSize < 4) {
				break;
			}

			mInfoLength = qFromLittleEndian<qint32>(this->read(4));
			mReceivingBytes.reserve(mInfoLength);

			qDebug() << "New message info length:" << mInfoLength;
			packetSize -= 4;
			if (packetSize == 0) {
				break;
			}
		}

		if (mHasPayload && mPayloadLength == -1) {
			if (packetSize < 4) {
				break;
			}

			mPayloadLength = qFromLittleEndian<qint32>(this->read(4));

			qDebug() << "New message payload length:" << mPayloadLength;
			packetSize -= 4;
			if (packetSize == 0) {
				continue;
			}
		}

		if (mInfoLength != 0) {
			QByteArray receivedBytes = this->read(mInfoLength);
			mInfoLength -= receivedBytes.count();
			packetSize -= receivedBytes.count();

			mReceivingBytes.append(receivedBytes);

			if (mInfoLength == 0) {
				QJsonParseError errorMessage;
				QJsonDocument response = QJsonDocument::fromJson(QString(mReceivingBytes).toUtf8(), &errorMessage);
				if (errorMessage.error != QJsonParseError::NoError) {
					qWarning() << "Bad info message received because:" << errorMessage.errorString();
				} else {
					QSharedPointer<QJsonObject> info(new QJsonObject(response.object()));
					mReceivingMessage->setInfoAndKeepPayload(info);
				}

				qDebug() << "New message info:\n" << response;

				mReceivingBytes.clear();
				if (mHasPayload) {
					mReceivingBytes.reserve(mPayloadLength);
				}

				if (packetSize == 0 && mHasPayload) {
					break;
				}
			} else {
				break;
			}
		}

		if (mHasPayload && mPayloadLength != 0) {
			QByteArray receivedBytes = this->read(mPayloadLength);
			mPayloadLength -= receivedBytes.count();
			packetSize -= receivedBytes.count();

			mReceivingBytes.append(receivedBytes);
			if (mPayloadLength == 0) {
				QSharedPointer<QByteArray> payload(new QByteArray(mReceivingBytes));
				mReceivingMessage->setPayloadAndKeepInfo(payload);
				qDebug() << "New message payload fully received:" << payload->count();
			} else {
				break;
			}
		}

		qDebug() << "Message fully received";
		emit messageReceived(mReceivingMessage);

		mReadPayloadFlag = false;
		mHasPayload = false;
		mInfoLength = -1;
		mPayloadLength = -1;
		mReceivingMessage.reset(new Message());
		mReceivingBytes.clear();
	}
	qDebug() << "Exit";
}