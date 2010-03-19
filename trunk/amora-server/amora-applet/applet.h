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

#ifndef __APPLET_H__
#define __APPLET_H__

#include <QWidget>
#include <QSystemTrayIcon>

class QStringList;

class QString;
class QAction;
class QMenu;
class AmoraServer;

class Applet : public QWidget
{
    Q_OBJECT

public:
    Applet();
    virtual ~Applet();

    void bind(AmoraServer *amora_server);
    void setStatus();
    void showStatusDetails();

private slots:
    void iconActivated(QSystemTrayIcon::ActivationReason reason);
    void about();

    void handleDisconnection(const QString client_name);
    void handleConnection(const QString client_name);

private:
    QStringList *phones;
    AmoraServer *amora;
    QAction *quitAction;
    QAction *aboutAction;

    QIcon *iconNormal;
    QIcon *iconConnected;

    QSystemTrayIcon *trayIcon;
    QMenu *menu;
};

#endif
