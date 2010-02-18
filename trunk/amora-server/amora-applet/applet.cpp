/*
 * Copyright (C) 2008-2010
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
 * The systray example found on the Qt4 documentation was used
 * as a reference for this implementation:
 * http://doc.trolltech.com/4.3/desktop-systray-window-h.html
 *
 */


#include "applet.h"
#include "about.h"
#include "amora-server.h"

#include <QAction>
#include <QMenu>
#include <QSystemTrayIcon>
#include <QIcon>
#include <QString>

#include <unistd.h>


Applet::Applet()
    : cellphone(0)
{
    quitAction = new QAction(tr("&Quit"), this);
    connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));

    aboutAction = new QAction(tr("&About"), this);
    connect(aboutAction, SIGNAL(triggered()), SLOT(about()));

    menu = new QMenu(this);
    menu->addAction(aboutAction);
    menu->addSeparator();
    menu->addAction(quitAction);

    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setContextMenu(menu);

    connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this, SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));

    setStatus(Start);
    trayIcon->show();
}


Applet::~Applet()
{
    delete quitAction;
    delete aboutAction;
    delete trayIcon;
    delete menu;
}

void Applet::setStatus(int st)
{
    QIcon iconStart(":/images/amora_start.png");
    QIcon iconOn(":/images/amora_bluetooth.png");
    QIcon iconOff(":/images/amora_exclamation.png");

    switch(st) {
    case Start:
        if (cellphone) {
            trayIcon->setIcon(iconOn);
            trayIcon->setToolTip("Amora Server (On)");
            status = On;
            return;
        }
        cellphone = 0;
        trayIcon->setIcon(iconStart);
        trayIcon->setToolTip("Amora Server (Started)");
        status = Start;
        break;

    case On:
        cellphone++;
        trayIcon->setIcon(iconOn);
        trayIcon->setToolTip("Amora Server (On)");
        status = On;
        break;

    case Off:
        cellphone--;
        trayIcon->setIcon(iconOff);
        trayIcon->setToolTip("Amora Server (Off)");
        status = Off;
        break;
    }

    qWarning("cellphones: %d", cellphone);
}


void Applet::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason) {
        case QSystemTrayIcon::Trigger:
        case QSystemTrayIcon::DoubleClick:
            showMessage("Amora is on", "amora");
            break;
        case QSystemTrayIcon::MiddleClick:
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

void Applet::iconStatus(int change)
{
    setStatus(change);
}

void Applet::bind(Amora *amora_server)
{
    amora = amora_server;
    connect(amora, SIGNAL(changeStatus(int)),
            SLOT(iconStatus(int)));
}
