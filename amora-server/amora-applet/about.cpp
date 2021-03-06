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

#include "about.h"

#include <QWidget>
#include <QDialog>
#include <QLayout>


About::About(QWidget *parentW)
    : QDialog(parentW)
{
    setupUi(this);
    layout()->setSizeConstraint(QLayout::SetFixedSize);
}
