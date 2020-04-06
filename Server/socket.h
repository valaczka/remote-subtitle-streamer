/*
 * ---- Call of Suli ----
 *
 * socket.h
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

#ifndef SOCKET_H
#define SOCKET_H

#include <QWebSocket>
#include <QObject>

class Socket : public QObject
{
	Q_OBJECT

public:

	enum SocketType {
		SocketInvalid,
		SocketSender,
		SocketReceiver
	};

	Q_ENUM(SocketType)

	Q_PROPERTY(SocketType socketType READ socketType WRITE setSocketType NOTIFY socketTypeChanged)
	Q_PROPERTY(QWebSocket * socket READ socket WRITE setSocket NOTIFY socketChanged)

	explicit Socket(QWebSocket *socket, QObject *parent = nullptr);

	SocketType socketType() const { return m_socketType; }
	QWebSocket *socket() const { return m_socket; }

public slots:
	void onDisconnected();

	void setSocketType(SocketType socketType);
	void setSocket(QWebSocket * socket);
	void onTextMessageReceived(const QString &message);
	void sendText(const QString &text);

signals:
	void disconnected();
	void updateText(const QString &text);

	void socketTypeChanged(SocketType socketType);
	void socketChanged(QWebSocket * socket);

private:
	SocketType m_socketType;
	QWebSocket * m_socket;
};

#endif // SOCKET_H
