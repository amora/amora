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
#include <QStringList>


Applet::Applet()
{
    quitAction = new QAction("&Quit", this);
    connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));

    aboutAction = new QAction("&About", this);
    connect(aboutAction, SIGNAL(triggered()), SLOT(about()));

    menu = new QMenu(this);
    menu->addAction(aboutAction);
    menu->addSeparator();
    menu->addAction(quitAction);

    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setContextMenu(menu);

    iconNormal = new QIcon(":/images/amora.png");
    iconConnected = new QIcon(":/images/amora_connected.png");

    phones = new QStringList();

    setStatus();

    connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this, SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));

    trayIcon->show();
}

Applet::~Applet()
{
    delete phones;
    delete iconNormal;
    delete iconConnected;
}


void Applet::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason) {
        case QSystemTrayIcon::Trigger:
        case QSystemTrayIcon::DoubleClick:
            Applet::showStatusDetails();
            break;
        default:
            ;
    }
}

void Applet::setStatus()
{
    if (phones->empty()) {
        trayIcon->setIcon(*iconNormal);
        trayIcon->setToolTip("Amora server (waiting)");
    }
    else {
        trayIcon->setIcon(*iconConnected);
        trayIcon->setToolTip("Amora Server (connected)");
    }
}

void Applet::showStatusDetails()
{
    if (!phones->empty())
        trayIcon->showMessage("Amora", QString("The following device(s) are connected:\n * %1").arg(phones->join("\n * ")));
    else
        trayIcon->showMessage("Amora", "Waiting for connections");
}

void Applet::handleConnection(const QString client_name)
{
    trayIcon->showMessage("Amora", QString("New connection from %1").arg(client_name),
                          QSystemTrayIcon::Information, 1500);
    phones->append(client_name);
    setStatus();
}

void Applet::handleDisconnection(const QString client_name)
{
    trayIcon->showMessage("Amora", QString("%1 has been disconnected").arg(client_name),
                          QSystemTrayIcon::Information, 1500);
    phones->removeOne(client_name);
    setStatus();
}

void Applet::about()
{
    About dialog;
    dialog.exec();
}

void Applet::bind(AmoraServer *amora_server)
{
    amora = amora_server;
    connect(amora, SIGNAL(clientDisconnected(const QString)),
            SLOT(handleDisconnection(const QString)));
    connect(amora, SIGNAL(clientConnected(const QString)),
            SLOT(handleConnection(const QString)));
}
