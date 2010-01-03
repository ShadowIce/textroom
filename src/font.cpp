/****************************************************************************
** Copyright (C) 2008 Petar Toushkov <peter dot toushkov at gmail.com>
** Copyright (C) 2008 Omer Bahri Gordebak <gordebak at gmail.com>
**
** Additional help, code and insights by:
** adamvert - from http://ubuntuforums.org/
** zebulon M - from http://ubuntuforums.org/
**
** Artwork by Edward Solorukhin <arch1000@gmail.com>
**
** Parts of the following code are from the Phrasis project:
**
** http://code.google.com/p/phrasis/
**
** and are
**
** Copyright (C) 2006 Jacob R Rideout <jacob@jacobrideout.net>
** All rights reserved.
**
** This file may be used under the terms of the GNU General Public
** License version 2.0 as published by the Free Software Foundation
** and appearing in the file LICENSE.GPL included in the packaging of
** this file.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#include <QtGui>

#include "font.h"

SelectFont::SelectFont(QWidget *parent) : QDialog(parent)

{
	ui.setupUi(this);
	connect(ui.okPushButton, SIGNAL(clicked()), this, SLOT(accept()));
	connect(ui.cancelPushButton, SIGNAL(clicked()), this, SLOT(reject()));
}

QString SelectFont::useSelectFont(QWidget *parent)
{
	SelectFont *sf = new SelectFont(parent);
	sf->setModal(true);
	QString family;
	if (sf->exec() == QDialog::Accepted)
	{
		family = sf->ui.fontComboBox->currentText();
	}
	else
	{
		family = "";
	}
	return family;
}
