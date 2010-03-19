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

#ifndef __AMORA_SERVER_H__
#define __AMORA_SERVER_H__

#include <QThread>

struct amora_s;

class AmoraServer : public QThread
{
    Q_OBJECT

public:
    AmoraServer(int argc, char **argv);
    virtual ~AmoraServer();
    struct amora_s *amora;

signals:
    void clientDisconnected(const QString name);
    void clientConnected(const QString name);

public slots:
    void onConnection(const QString clientName);
    void onDisconnection(const QString clientName);

private:
    int _argc;
    char **_argv;
    char *logfile;
    int bt_hci;
    int bt_channel;

    void run(void);
    void parse_args(int argc, char *argv[]);
    void show_usage(const char *path);

    Q_DISABLE_COPY(AmoraServer);
};

#endif
