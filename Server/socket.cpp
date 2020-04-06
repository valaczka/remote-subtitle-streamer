/*
 * ---- Call of Suli ----
 *
 * socket.cpp
 *
 * Created on: 2020. 04. 06.
 *     Author: Valaczka János Pál <valaczka.janos@piarista.hu>
 *
 * Socket
 *
 *  This file is part of Call of Suli.
 *
 *  Call of Suli is free software: you can redistribute it and/or modify
 *  it under the terms of the MIT license.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "socket.h"

Socket::Socket(QWebSocket *socket, QObject *parent)
	: QObject(parent)
	, m_socketType(SocketInvalid)
	, m_socket(socket)
{
	Q_ASSERT(socket);

	qInfo().noquote() << tr("Client connected: ") << this << m_socket->peerAddress().toString() << m_socket->peerPort();

	connect(m_socket, SIGNAL(disconnected()), this, SLOT(onDisconnected()));
	connect(m_socket, &QWebSocket::textMessageReceived, this, &Socket::onTextMessageReceived);
}



/**
 * @brief Socket::onDisconnected
 */

void Socket::onDisconnected()
{
	qInfo().noquote() << tr("Client disconnected:") << this << m_socket->peerAddress().toString() << m_socket->peerPort();
	emit disconnected();
}



void Socket::setSocketType(Socket::SocketType socketType)
{
	if (m_socketType == socketType)
		return;

	m_socketType = socketType;
	emit socketTypeChanged(m_socketType);
}

void Socket::setSocket(QWebSocket *socket)
{
	if (m_socket == socket)
		return;

	m_socket = socket;
	emit socketChanged(m_socket);
}


/**
 * @brief Socket::onTextMessageReceived
 * @param message
 */

void Socket::onTextMessageReceived(const QString &message)
{
	qDebug().noquote() << this << message;

	if (message.startsWith("SOCKET:")) {
		QString p = message;
		p.remove(0, QString("SOCKET:").length());

		if (p == "SENDER")
			setSocketType(SocketSender);
		else if (p == "RECEIVER") {
			setSocketType(SocketReceiver);
			m_socket->sendTextMessage("");
		}

		qDebug() << "SOCKET TYPE" << m_socketType;
	} else if (m_socketType == SocketSender) {
		emit updateText(message);
	}
}


/**
 * @brief Socket::onSendText
 * @param text
 */

void Socket::sendText(const QString &text)
{
	if (m_socketType == SocketReceiver) {
		qInfo().noquote() << "SEND:" << text;
		m_socket->sendTextMessage(text);
	}
}

