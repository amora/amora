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
 * $Id: about.h 344 2008-08-18 23:01:06Z ademar $
 */

#ifndef __ABOUT_H__
#define __ABOUT_H__

#include <QWidget>
#include <QDialog>
#include "ui_about.h"

class About : public QDialog, Ui::About
{
	Q_OBJECT

public:
	About(QWidget *parentW = 0);

};

#endif
