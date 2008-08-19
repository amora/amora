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
 * $Id: about.cpp 344 2008-08-18 23:01:06Z ademar $
 */


#include "about.h"
#include "ui_about.h"

About::About(QWidget *parentW) : QDialog(parentW)
{
	ui = new Ui::About();
	ui->setupUi(this);
}

About::~About()
{
	delete ui;
}
