/*
 * ---- Call of Suli ----
 *
 * server.cpp
 *
 * Created on: 2020. 04. 06.
 *     Author: Valaczka János Pál <valaczka.janos@piarista.hu>
 *
 * Server
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

#include <QUrl>

#include "server.h"

Server::Server(QObject *parent) : QObject(parent)
{
	m_server = new QWebSocketServer("Remote subtitle streamer server", QWebSocketServer::NonSecureMode);
	m_server->setMaxPendingConnections(15);

	connect(m_server, SIGNAL(newConnection()), this, SLOT(onNewConnection()));

}


/**
 * @brief Server::~Server
 */


Server::~Server()
{
	qDeleteAll(m_clients.begin(), m_clients.end());
	delete m_server;
}


/**
 * @brief Server::start
 */

bool Server::start()
{
	if (m_server->listen(QHostAddress::Any, 10101))
	{
		qInfo().noquote() << tr("Listening on ")+m_server->serverUrl().toString();
	} else {
		qCritical().noquote() << QString(tr("Cannot listen"));
		return false;
	}

	return true;
}



void Server::setServer(QWebSocketServer *server)
{
	if (m_server == server)
		return;

	m_server = server;
	emit serverChanged(m_server);
}


/**
 * @brief Server::onTextUpdate
 * @param text
 */

void Server::onTextUpdate(const QString &text)
{
	foreach (Socket *s, m_clients) {
		if (s->socketType() == Socket::SocketReceiver) {
			s->sendText(text);
		}
	}
}



/**
 * @brief Server::onNewConnection
 */

void Server::onNewConnection()
{
	QWebSocket *pSocket = m_server->nextPendingConnection();

	Socket *socket = new Socket(pSocket);

	connect(socket, SIGNAL(disconnected()), this, SLOT(onSocketDisconnected()));
	connect(socket, &Socket::updateText, this, &Server::onTextUpdate);

	m_clients << socket;
}


/**
 * @brief Server::onSocketDisconnected
 */

void Server::onSocketDisconnected()
{
	Socket *handler = qobject_cast<Socket *>(sender());
	if (handler)
	{
		m_clients.removeAll(handler);
		handler->deleteLater();
	}
}
