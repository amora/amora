/*
 * Copyright (C) 2008
 * Ademar de Souza Reis Jr. <ademar@ademar.org>
 * GPLv2+
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version
 * 2 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See file COPYING for details.
 *
 * It used as a reference the example found on the QT4 documentation:
 * http://doc.trolltech.com/4.3/desktop-systray-window-h.html
 *
 * $Id: applet.cpp 344 2008-08-18 23:01:06Z ademar $
 */


#include <QtGui>
#include "applet.h"
#include "about.h"

#include <unistd.h>


Applet::Applet()
{
	quitAction = new QAction(tr("&Quit"), this);
	connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));

	aboutAction = new QAction(tr("&About"), this);
	connect(aboutAction, SIGNAL(triggered()), this, SLOT(about()));

	menu = new QMenu(this);
	menu->addAction(aboutAction);
	menu->addSeparator();
	menu->addAction(quitAction);

	trayIcon = new QSystemTrayIcon(this);
	trayIcon->setContextMenu(menu);

	connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
			this, SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));

	setStatus(Applet::On);

	trayIcon->show();
}


Applet::~Applet()
{
	delete quitAction;
	delete aboutAction;
	delete trayIcon;
	delete menu;
}

void Applet::setStatus(enum Status st)
{
	QIcon iconOn(":/amora_bluetooth.png");
	QIcon iconOff(":/amora_exclamation.png");

	switch(st) {
	case Applet::On:
		trayIcon->setIcon(iconOn);
		trayIcon->setToolTip("Amora Server (On)");
		status = Applet::On;
		break;
	case Applet::Off:
		trayIcon->setIcon(iconOff);
		trayIcon->setToolTip("Amora Server (Off)");
		status = Applet::Off;
		break;
	}
}


void Applet::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
	switch (reason) {
		case QSystemTrayIcon::Trigger:
		case QSystemTrayIcon::DoubleClick:
			showMessage("Hello world", "amora");
			break;
		case QSystemTrayIcon::MiddleClick:
			if (status == Applet::Off)
				setStatus(Applet::On);
			else
				setStatus(Applet::Off);
			break;
		default:
			;
	}
}

void Applet::showMessage(QString message, QString title)
{
	trayIcon->showMessage(title, message,
			QSystemTrayIcon::MessageIcon(QSystemTrayIcon::Information));
}

void Applet::about()
{
	About dialog;
	dialog.exec();
}
