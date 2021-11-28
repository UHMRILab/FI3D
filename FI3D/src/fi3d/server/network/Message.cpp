#include <FI3D/server/network/Message.h>

using namespace fi3d;

Message::Message(QSharedPointer<QJsonObject> info, QSharedPointer<QByteArray> payload)
	: mInfo(info),
	mPayload(payload),
	mIsMessageEmpty(false),
	mHasPayload(true)
{
	if (mInfo.isNull()) {
		mInfo.reset(new QJsonObject());
	}

	if (mPayload.isNull()) {
		mPayload.reset(new QByteArray());
	}
}

Message::Message(QSharedPointer<QJsonObject> info)
	: mInfo(info),
	mPayload(new QByteArray()),
	mIsMessageEmpty(false),
	mHasPayload(false) {
	if (mInfo.isNull()) {
		mInfo.reset(new QJsonObject());
	}
}

Message::Message() 
	: mInfo(new QJsonObject()),
	mPayload(new QByteArray()),
	mIsMessageEmpty(true),
	mHasPayload(false)
{}

Message::~Message() {}

void Message::setInfoAndPayload(QSharedPointer<QJsonObject> info, QSharedPointer<QByteArray> payload) {
	mInfo = info;
	mPayload = payload;
	
	if (mInfo.isNull()) {
		mInfo.reset(new QJsonObject());
	}
	if (mPayload.isNull()) {
		mPayload.reset(new QByteArray());
	}

	mIsMessageEmpty = false;
	mHasPayload = true;
}

void Message::setInfoAndKeepPayload(QSharedPointer<QJsonObject> info) {
	mInfo = info;

	if (mInfo.isNull()) {
		mInfo.reset(new QJsonObject());
	}

	mIsMessageEmpty = false;
}

void Message::setPayloadAndKeepInfo(QSharedPointer<QByteArray> payload) {
	mPayload = payload;

	if (mPayload.isNull()) {
		mPayload.reset(new QByteArray());
	}

	mHasPayload = true;
}

void Message::setInfo(QSharedPointer<QJsonObject> info) {
	mInfo = info;

	if (mInfo.isNull()) {
		mInfo.reset(new QJsonObject());
	}

	if (mPayload->count() != 0) {
		mPayload.reset(new QByteArray());
	}

	mIsMessageEmpty = false;
	mHasPayload = false;
}

void Message::invalidate() {
	mInfo.reset(new QJsonObject());
	mPayload.reset(new QByteArray());
	mIsMessageEmpty = true;
	mHasPayload = false;
}

bool Message::isMessageValid() {
	return !mIsMessageEmpty;
}

bool Message::hasPayload() {
	return mHasPayload;
}

QSharedPointer<QJsonObject> Message::getInfo() {
	return mInfo;
}

QSharedPointer<QByteArray> Message::getPayload() {
	return mPayload;
}
