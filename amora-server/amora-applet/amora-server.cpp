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


#include <iostream>
#include <QApplication>
#include <QMessageBox>

#include "applet.h"
#include "amora-server.h"

extern "C" {
#include <unistd.h>
#include <amora.h>
}

/* Required to be used by conn/disconnection callbacks */
static Applet *applet = NULL;


void client_connection(const char *client_name)
{
	/* TODO: use some UI element to display this */
	if (client_name)
		fprintf(stderr, "client connected: %s\n", client_name);
	if (applet)
		applet->setStatus(Applet::On);

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
				/* XXX libamora should provide the version */
				printf("experimental amora-applet, "
					"undefined version\n");
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
	char *name, *p = strdup(path);

	name = basename(p);

	printf("Usage: %s [-l logfile] [-h] [-v] [-i hci_number]\n"
	       "\n"
	       "  -h             show this help message and exit;\n"
	       "  -l logfile     set the log file path (default is disabled);\n"
	       "  -v             show version and exit.\n"
	       "  -i hci_number  set the bluetooth dongle device to use\n"
	       "\n", name);

	free(p);
}

Amora::~Amora()
{
	amora_context_delete(amora);
}

void Amora::run(void)
{
	int status;
	amora_start(amora);
	exit(status);
}

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	int result;

	if (!QSystemTrayIcon::isSystemTrayAvailable()) {
		QMessageBox::critical(0, QObject::tr("Amora Applet"),
				QObject::tr("No system tray available, exiting"));
		return 1;
	}

	Amora amora(argc, argv);
	amora.start();

	applet = new Applet();
	app.setQuitOnLastWindowClosed(false);
	result = app.exec();
	delete applet;
	return result;
}
