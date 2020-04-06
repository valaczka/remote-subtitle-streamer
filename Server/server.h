/*
 * ---- Call of Suli ----
 *
 * server.h
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

#ifndef SERVER_H
#define SERVER_H

#include <QtWebSockets/QtWebSockets>
#include <QObject>

#include "socket.h"

class Server : public QObject
{
	Q_OBJECT

	Q_PROPERTY(QWebSocketServer * server READ server WRITE setServer NOTIFY serverChanged)

	QWebSocketServer * m_server;

public:
	explicit Server(QObject *parent = nullptr);
	~Server();


	QWebSocketServer * server() const { return m_server; }

	bool start();

public slots:
	void setServer(QWebSocketServer * server);
	void onTextUpdate(const QString &text);

signals:
	void serverChanged(QWebSocketServer * server);

private slots:
	void onNewConnection();
	void onSocketDisconnected();

private:
	QList<Socket *> m_clients;
};

#endif // SERVER_H
