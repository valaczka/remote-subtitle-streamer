/*
 * ---- Call of Suli ----
 *
 * receiver.cpp
 *
 * Created on: 2020. 04. 06.
 *     Author: Valaczka János Pál <valaczka.janos@piarista.hu>
 *
 * Receiver
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


#include <QSettings>
#include <QFile>

#include "receiver.h"

Receiver::Receiver(QObject *parent) : QObject(parent)
{
	QSettings s;

	m_fileName = s.value("file", QString("data.txt")).toString();
}


/**
 * @brief Receiver::getUrl
 * @return
 */

QUrl Receiver::getUrl()
{
	QSettings s;

	return s.value("url", QUrl("ws://vjpvm.piarista.hu:10101")).toUrl();
}


/**
 * @brief Receiver::setUrl
 * @param url
 */

void Receiver::setUrl(const QUrl &url)
{
	if (url.isEmpty())
		return;

	QSettings s;
	s.setValue("url", url);
}

void Receiver::setFileName(QString fileName)
{
	if (m_fileName == fileName)
		return;

	m_fileName = fileName;
	emit fileNameChanged(m_fileName);
}


/**
 * @brief Receiver::saveFile
 */

void Receiver::saveFile()
{
	if (m_fileName.isEmpty())
		return;

	QSettings s;
	s.setValue("file", m_fileName);
}


/**
 * @brief Receiver::parseMessage
 * @param message
 * @return
 */

QString Receiver::parseMessage(const QString &message)
{
	QString stream = message;
	QString label = message;

	label.replace(" * ", "\n");
	stream.replace(" * ", " ");

	if (!m_fileName.isEmpty()) {
		QFile f(m_fileName);
		if (f.open(QIODevice::WriteOnly)) {
			f.write(stream.toUtf8());
			f.close();
		}
	}

	return label;
}


/**
 * @brief Receiver::getSize
 * @return
 */

int Receiver::getSize()
{
	QSettings s;
	int size = s.value("size", 18).toInt();
	if (size<18)
		size = 18;

	return size;
}


/**
 * @brief setSize
 * @param size
 */

void Receiver::setSize(const int &size)
{
	QSettings s;
	s.setValue("size", size);
}
