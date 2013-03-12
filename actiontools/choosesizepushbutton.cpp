/*
	Actionaz
	Copyright (C) 2008-2012 Jonathan Mercier-Ganady

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

#include "choosesizepushbutton.h"
#include "nativeeventfilteringapplication.h"

#include <QStylePainter>
#include <QStyleOptionButton>
#include <QMouseEvent>
#include <QMessageBox>
#include <QDesktopWidget>
#include <QMainWindow>
#include <QTimer>
#include <QDebug>

#ifdef Q_WS_X11
#include <QX11Info>
#include <X11/Xlib.h>
#endif

#ifdef Q_WS_WIN
#include <Windows.h>
#endif

namespace ActionTools
{
	ChooseSizePushButton::ChooseSizePushButton(QWidget *parent)
	: QPushButton(parent),
	mCrossIcon(new QPixmap(":/images/cross.png")),
	mSearching(false),
	mMainWindow(0)
#ifdef Q_WS_WIN
	,mPreviousCursor(NULL)
#endif
	{
#ifdef Q_WS_X11
		foreach(QWidget *widget, QApplication::topLevelWidgets())
		{
			if(QMainWindow *mainWindow = qobject_cast<QMainWindow*>(widget))
			{
				mMainWindow = mainWindow;
				break;
			}
		}
#endif

		setToolTip(tr("Target a position by clicking this button, moving the cursor to the desired position and releasing the mouse button."));
	}

	ChooseSizePushButton::~ChooseSizePushButton()
	{
		if(mSearching)
			stopMouseCapture();

		nativeEventFilteringApp->removeNativeEventFilter(this);

		delete mCrossIcon;
	}

	void ChooseSizePushButton::paintEvent(QPaintEvent *event)
	{
		if(mSearching)
		{
			QPushButton::paintEvent(event);
			return;
		}

		QStylePainter painter(this);
		QStyleOptionButton option;

		initStyleOption(&option);

		painter.drawControl(QStyle::CE_PushButton, option);
		painter.drawItemPixmap(rect(), Qt::AlignCenter, *mCrossIcon);
	}

	void ChooseSizePushButton::mousePressEvent(QMouseEvent *event)
	{
		QPushButton::mousePressEvent(event);

		mSearching = true;
		update();

#ifdef Q_WS_X11
		if(mMainWindow)
			mMainWindow->showMinimized();
#endif
#ifdef Q_WS_WIN
		foreach(QWidget *widget, qApp->topLevelWidgets())
			widget->setWindowOpacity(0.0f);
#endif

		QCursor newCursor(*mCrossIcon);

		emit chooseStarted();

#ifdef Q_WS_WIN
		mPreviousCursor = SetCursor(newCursor.handle());
#endif
#ifdef Q_WS_X11
		nativeEventFilteringApp->installNativeEventFilter(this);

		if(XGrabPointer(QX11Info::display(), DefaultRootWindow(QX11Info::display()), True, ButtonReleaseMask, GrabModeAsync, GrabModeAsync,
						None, newCursor.handle(), CurrentTime) != GrabSuccess)
		{
			QMessageBox::warning(this, tr("Choose a window"), tr("Unable to grab the pointer."));
			event->ignore();
		}
#endif
	}

#ifdef Q_WS_WIN
	void ChooseSizePushButton::mouseReleaseEvent(QMouseEvent *event)
	{
		QPushButton::mouseReleaseEvent(event);

		emit positionChosen(event->globalPos());

		stopMouseCapture();
	}
#endif

#ifdef Q_WS_X11
	bool ChooseSizePushButton::x11EventFilter(XEvent *event)
	{
		if(event->type == ButtonRelease)
		{
			emit positionChosen(QCursor::pos());

			stopMouseCapture();

			return true;
		}

		return false;
	}
#endif

	void ChooseSizePushButton::stopMouseCapture()
	{
		mSearching = false;
		update();

#ifdef Q_WS_WIN
		if(mPreviousCursor)
			SetCursor(mPreviousCursor);

		foreach(QWidget *widget, qApp->topLevelWidgets())
			widget->setWindowOpacity(1.0f);
#endif
#ifdef Q_WS_X11
		XUngrabPointer(QX11Info::display(), CurrentTime);

		nativeEventFilteringApp->removeNativeEventFilter(this);

		if(mMainWindow)
			mMainWindow->showNormal();
#endif
	}
}
