/*
	Actionaz
	Copyright (C) 2008-2013 Jonathan Mercier-Ganady

	Actionaz is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	Actionaz is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program. If not, see <http://www.gnu.org/licenses/>.

	Contact : jmgr@jmgr.info
*/

#include "newactiontreewidget.h"

#include <QMimeData>

NewActionTreeWidget::NewActionTreeWidget(QWidget *parent)
	: QTreeWidget(parent)
{
}

Qt::DropActions NewActionTreeWidget::supportedDropActions() const
{
	return Qt::CopyAction;
}

QMimeData *NewActionTreeWidget::mimeData(const QList<QTreeWidgetItem *> items) const
{
	QMimeData *mimeData = new QMimeData();
	QByteArray data;

	if(items.count() != 1)
		return 0;

	data.append(items.at(0)->data(0, ActionIdRole).toString());

	mimeData->setData("application/actionaz.add.action", data);

	return mimeData;
}
