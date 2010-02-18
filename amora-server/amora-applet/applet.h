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

class QString;
class QAction;
class QMenu;
class Amora;

enum applet_status {
    Start = 0,
    On = 1,
    Off = 2
};

class Applet : public QWidget
{
    Q_OBJECT

public:
    Applet();
    ~Applet();

    void showMessage(QString message, QString title="Amora Server");
    void bind(Amora *amora_server);

private slots:
    void iconStatus(int change);
    void iconActivated(QSystemTrayIcon::ActivationReason reason);
    void about();

private:
    int cellphone;
    uint status;
    Amora *amora;
    QAction *quitAction;
    QAction *aboutAction;

    QSystemTrayIcon *trayIcon;
    QMenu *menu;
    void setStatus(int st);
};

#endif
