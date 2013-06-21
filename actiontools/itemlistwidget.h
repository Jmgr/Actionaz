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

#ifndef ITEMLISTWIDGET_H
#define ITEMLISTWIDGET_H

#include "actiontools_global.h"

#include <QWidget>

namespace Ui
{
    class ItemListWidget;
}

namespace ActionTools
{
	class ItemListModel;
}

class QItemSelection;

namespace ActionTools
{
	class ACTIONTOOLSSHARED_EXPORT ItemListWidget : public QWidget
	{
		Q_OBJECT

	public:
		explicit ItemListWidget(QWidget *parent = 0);
		~ItemListWidget();

		void setItems(const QStringList &items);
		void appendItems(const QStringList &items);

		QStringList items() const;

	private slots:
		void on_addPushButton_clicked();
		void on_removePushButton_clicked();
		void on_moveUpPushButton_clicked();
		void on_moveDownPushButton_clicked();
		void selectionChanged(const QItemSelection &selected, const QItemSelection &deselected);
		void on_itemsListView_removeCurrentItem();
		void on_itemsListView_moveCurrentItem(bool up);

	private:
		void move(bool up);

		Ui::ItemListWidget *ui;
		ItemListModel *mModel;
	};
}

#endif // ITEMLISTWIDGET_H
