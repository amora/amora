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
 */


#include "applet.h"
#include "amora-server.h"

#include <QApplication>
#include <QMessageBox>

extern "C" {
#include <amora.h>
}

/* Required to be used by {dis,}connection callbacks */
static AmoraServer *server = NULL;

void client_connection(const char *client_name)
{
    if (!server)
        return;

    server->onConnection(client_name);
}

void client_disconnection(const char *client_name)
{
    if (!server)
        return;

    server->onDisconnection(client_name);
}

AmoraServer::AmoraServer(int argc, char *argv[])
    : _argc(argc), _argv(argv), logfile(0), bt_hci(-1), bt_channel(16)
{
    parse_args(argc, argv);

    amora = amora_context_new(logfile, bt_channel, bt_hci);

    if (!amora) {
        QString msg = "Error initializing the amora library.";
        if (logfile)
            msg += QString("\nCheck the log file for details (%1).").arg(logfile);
        else
            msg += QString("\nEnable logging (-l option) to see what's happening.");

        QMessageBox::critical(0, "Amora Applet", msg);
        ::exit(1);
    }

    amora_connection_callback(amora, client_connection);
    amora_disconnection_callback(amora, client_disconnection);
}

void AmoraServer::parse_args(int argc, char *argv[])
{
    int arg;

    if (argc > 5) {
        show_usage(argv[0]);
        ::exit(-1);
    }

    while ((arg = getopt(argc, argv, "l:i:hv")) != -1) {
        switch (arg) {
            case 'l':
                logfile = optarg;
                break;
            case 'h':
                show_usage(argv[0]);
                ::exit(0);
            case 'v':
                /* XXX libamora doesn't provide the current version */
                qWarning("amora-applet (unspecified version)");
                ::exit(0);
            case 'i':
                bt_hci = atoi(optarg);
                break;
            default:
                show_usage(argv[0]);
                ::exit(0);
        }
    }
}

void AmoraServer::show_usage(const char *path)
{
    qCritical("Usage: %s [-l logfile] [-h] [-v] [-i hci_number]\n"
           "\n"
           "  -h             show this help message and exit;\n"
           "  -l logfile     set the log file path (default is disabled);\n"
           "  -v             show version and exit.\n"
           "  -i hci_number  set the bluetooth dongle device to use\n"
           "\n", basename(path));
}

AmoraServer::~AmoraServer()
{
    // XXX: there's no amora_stop() yet :-(
    amora_context_delete(amora);
}

void AmoraServer::onConnection(const QString clientName)
{
    emit clientConnected(clientName);
}

void AmoraServer::onDisconnection(const QString clientName)
{
    emit clientDisconnected(clientName);
}

void AmoraServer::run(void)
{
    amora_start(amora);
    exit(0);
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    if (!QSystemTrayIcon::isSystemTrayAvailable()) {
        QMessageBox::critical(0, "Amora Applet",
                              "No system tray available, exiting");
        return 1;
    }

    AmoraServer s(argc, argv);
    server = &s;

    Applet applet;
    applet.bind(server);
    s.start();

    app.setQuitOnLastWindowClosed(false);
    return app.exec();
}
