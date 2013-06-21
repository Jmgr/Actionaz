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

#ifndef ITEMLISTVIEW_H
#define ITEMLISTVIEW_H

#include "actiontools_global.h"

#include <QListView>

namespace ActionTools
{
	class ACTIONTOOLSSHARED_EXPORT ItemListView : public QListView
	{
		Q_OBJECT
	public:
		explicit ItemListView(QWidget *parent = 0);

	protected:
		virtual void keyPressEvent(QKeyEvent *event);
		virtual void dragMoveEvent(QDragMoveEvent *event);
		virtual void dragLeaveEvent(QDragLeaveEvent* event);
		virtual void dropEvent(QDropEvent* event);
		virtual void paintEvent(QPaintEvent* event);

	signals:
		void removeCurrentItem();
		void moveCurrentItem(bool up);

	private:
		QRect mDropIndicator;
	};
}

#endif // ITEMLISTVIEW_H
