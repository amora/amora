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
 * $Id: amora-server.h 346 2008-08-19 16:34:50Z ademar $
 */

#ifndef __AMORA_H__
#define __AMORA_H__

extern "C" {
#include <amora.h>
}

#include <QThread>

class Amora : public QThread
{
private:
	int _argc;
	char **_argv;
	struct amora_s *amora;
	char *logfile;
	int bt_channel;
	int bt_hci;

	void run(void);
	void parse_args(int argc, char *argv[]);
	void show_usage(const char *path);

public:
	Amora(int argc, char **argv);
	~Amora();
};

#endif
