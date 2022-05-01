/*
    Actiona
    Copyright (C) 2005 Jonathan Mercier-Ganady

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

    Contact: jmgr@jmgr.info
*/

#include "backend/x11.hpp"

#include <QX11Info>

#include <X11/Xlib.h>

namespace Backend
{
    QString formatX11Error(int errorCode)
    {
        char errStr[256];
        XGetErrorText(QX11Info::display(), errorCode, errStr, 256);
        return QString::fromLatin1(errStr);
    }

    QString formatGrabError(int errorCode)
    {
        switch(errorCode)
        {
        case AlreadyGrabbed:
            return QStringLiteral("AlreadyGrabbed");
        case GrabInvalidTime:
            return QStringLiteral("GrabInvalidTime");
        case GrabNotViewable:
            return QStringLiteral("GrabNotViewable");
        case GrabFrozen:
            return QStringLiteral("GrabFrozen");
        default:
            return {};
        }
    }
}
