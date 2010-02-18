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
 * $Id: amora-server.cpp 346 2008-08-19 16:34:50Z ademar $
 */


#include "applet.h"
#include "amora-server.h"

#include <QApplication>
#include <QMessageBox>
#include <QTimer>

extern "C" {
#include <amora.h>
}

/* Required to be used by conn/disconnection callbacks */
static Amora *amora_server = NULL;

void client_connection(const char *client_name)
{
    /* TODO: use some UI element to display this */
    if (client_name)
        qWarning("client connected: %s", client_name);

    if (amora_server)
        amora_server->emitSignal(On);
}

void client_disconnection(const char *client_name)
{
    /* TODO: amora lib should return client name */
    (void)client_name;

    /* TODO: applet must have an internal counter and do himself
     * the animation to show disconnection. There is no guarantee that
     * the signals will be delivered in order.
     */
    if (amora_server) {
        amora_server->emitSignal(Off);
        sleep(1);
        amora_server->emitSignal(Start);
    }
}

void Amora::emitSignal(int change)
{
    emit changeStatus(change);
}

Amora::Amora(int argc, char *argv[]): _argc(argc), _argv(argv)
{
    logfile = NULL;
    bt_hci = -1;
    bt_channel = 16;

    parse_args(argc, argv);

    amora = amora_context_new(logfile, bt_channel, bt_hci);
    if (amora == NULL) {
        ::exit(1);
    }

    amora_connection_callback(amora, client_connection);
    amora_disconnection_callback(amora, client_disconnection);
}

void Amora::parse_args(int argc, char *argv[])
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
                /* XXX libamora should provide the current version */
                qWarning("amora-applet (experimental)");
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

void Amora::show_usage(const char *path)
{
    qCritical("Usage: %s [-l logfile] [-h] [-v] [-i hci_number]\n"
           "\n"
           "  -h             show this help message and exit;\n"
           "  -l logfile     set the log file path (default is disabled);\n"
           "  -v             show version and exit.\n"
           "  -i hci_number  set the bluetooth dongle device to use\n"
           "\n", basename(path));
}

Amora::~Amora()
{
    amora_context_delete(amora);
}

void Amora::run(void)
{
    amora_start(amora);
    exit(0);
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    if (!QSystemTrayIcon::isSystemTrayAvailable()) {
        QMessageBox::critical(0, QObject::tr("Amora Applet"),
                              QObject::tr("No system tray available, exiting"));
        return 1;
    }

    Amora server(argc, argv);
    amora_server = &server;

    Applet applet;
    applet.bind(amora_server);
    server.start();

    app.setQuitOnLastWindowClosed(false);
    return app.exec();
}
