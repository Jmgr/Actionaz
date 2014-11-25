/*
    Actiona
    Copyright (C) 2008-2014 Jonathan Mercier-Ganady

    Actiona is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Actiona is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program. If not, see <http://www.gnu.org/licenses/>.

    Contact : jmgr@jmgr.info
*/

#include "screenshooter.h"
#include "windowhandle.h"

#include <QDesktopWidget>
#include <QApplication>
#include <QImage>
#include <QPainter>

#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
#include <QScreen>
#endif

#include <limits>

namespace ActionTools
{
    QPixmap ScreenShooter::captureScreen(int screenIndex)
    {
        QDesktopWidget *desktop = QApplication::desktop();

        if(screenIndex < 0 || screenIndex >= desktop->screenCount())
            return QPixmap();

        const QRect &screenGeometry = desktop->screenGeometry(screenIndex);

#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
        return QGuiApplication::primaryScreen()->grabWindow(0, screenGeometry.x(), screenGeometry.y(), screenGeometry.width(), screenGeometry.height());
#else
        return QPixmap::grabWindow(desktop->winId(), screenGeometry.x(), screenGeometry.y(), screenGeometry.width(), screenGeometry.height());
#endif
    }

    QList< QPair<QPixmap, QRect> > ScreenShooter::captureScreens()
    {
        QDesktopWidget *desktop = QApplication::desktop();
        QList< QPair<QPixmap, QRect> > result;

        for(int screenIndex = 0; screenIndex < desktop->screenCount(); ++screenIndex)
        {
            const QRect &screenGeometry = desktop->screenGeometry(screenIndex);

#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
            result.append(qMakePair(QGuiApplication::primaryScreen()->grabWindow(0, screenGeometry.x(), screenGeometry.y(), screenGeometry.width(), screenGeometry.height()), screenGeometry));
#else
            result.append(qMakePair(QPixmap::grabWindow(desktop->winId(), screenGeometry.x(), screenGeometry.y(), screenGeometry.width(), screenGeometry.height()), screenGeometry));
#endif
        }

        return result;
    }

    QList<QPair<QPixmap, QRect> > ScreenShooter::captureWindows(const QList<WindowHandle> &windows)
    {
        QDesktopWidget *desktop = QApplication::desktop();
        QList< QPair<QPixmap, QRect> > result;

        for(const WindowHandle &window: windows)
        {
            if(!window.isValid())
                continue;

            const QRect &windowGeometry = window.rect();

#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
            result.append(qMakePair(QGuiApplication::primaryScreen()->grabWindow(desktop->winId(), windowGeometry.x(), windowGeometry.y(), windowGeometry.width(), windowGeometry.height()), windowGeometry));
#else
            result.append(qMakePair(QPixmap::grabWindow(desktop->winId(), windowGeometry.x(), windowGeometry.y(), windowGeometry.width(), windowGeometry.height()), windowGeometry));
#endif
        }

        return result;
    }

    QPixmap ScreenShooter::captureWindow(WindowHandle window)
    {
        if(!window.isValid())
            return QPixmap();

        const QRect &windowGeometry = window.rect();

#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
        return QGuiApplication::primaryScreen()->grabWindow(0, windowGeometry.x(), windowGeometry.y(), windowGeometry.width(), windowGeometry.height());
#else
        QDesktopWidget *desktop = QApplication::desktop();

        return QPixmap::grabWindow(desktop->winId(), windowGeometry.x(), windowGeometry.y(), windowGeometry.width(), windowGeometry.height());
#endif
    }

    QPixmap ScreenShooter::captureAllScreens()
    {
        const QList< QPair<QPixmap, QRect> > &screens = captureScreens();
        QRect resultRect;
        QPoint minimalTopLeft(std::numeric_limits<int>::max(), std::numeric_limits<int>::max());

        typedef QPair<QPixmap, QRect> PixmapRectPair;
        for(const PixmapRectPair &screen: screens)
        {
            const QRect &screenRect = screen.second;

            resultRect = resultRect.united(screenRect);

            if(minimalTopLeft.x() > screenRect.x())
                minimalTopLeft.setX(screenRect.x());
            if(minimalTopLeft.y() > screenRect.y())
                minimalTopLeft.setY(screenRect.y());
        }

        QImage result(resultRect.width(), resultRect.height(), QImage::Format_RGB32);
        result.fill(Qt::black);

        {
            QPainter painter(&result);

            for(const PixmapRectPair &screen: screens)
            {
                const QRect &screenRect = screen.second;

                painter.drawPixmap(screenRect.x() - minimalTopLeft.x(), screenRect.y() - minimalTopLeft.y(), screen.first);
            }
        }

        return QPixmap::fromImage(result);
    }

    QPixmap ScreenShooter::captureRect(const QRect &rect)
    {
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
        return QGuiApplication::primaryScreen()->grabWindow(0, rect.x(), rect.y(), rect.width(), rect.height());
#else
        QDesktopWidget *desktop = QApplication::desktop();

        return QPixmap::grabWindow(desktop->winId(), rect.x(), rect.y(), rect.width(), rect.height());
#endif
    }
}
