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

#ifndef WINDOWHANDLE_H
#define WINDOWHANDLE_H

#include <QWidget>
#include <QMetaType>

#include "actiontools_global.h"

namespace ActionTools
{
	class ACTIONTOOLSSHARED_EXPORT WindowHandle
	{
	public:
		WindowHandle() : mValue(0)								{}
		WindowHandle(WId handle) : mValue(handle)				{}

		bool isValid() const									{ return (mValue != 0); }
		void setInvalid()										{ mValue = 0; }
		WId value() const										{ return mValue; }

		operator bool() const									{ return isValid(); }
		bool operator ==(const WindowHandle &other) const		{ return (other.mValue == mValue); }
		bool operator !=(const WindowHandle &other) const		{ return (other.mValue != mValue); }

		QString title() const;
		QString classname() const;
		QRect rect(bool useBorders = true) const;
		int processId() const;
		bool close() const;
		bool killCreator() const;
		bool setForeground() const;
		bool minimize() const;
		bool maximize() const;
		bool move(QPoint position) const;
		bool resize(QSize size, bool useBorders = true) const;
		bool isActive() const;

		static WindowHandle foregroundWindow();
		static QList<WindowHandle> windowList();
		static WindowHandle findWindow(const QString &title);
		static WindowHandle findWindow(const QRegExp &regExp);
		static QList<WindowHandle> findWindows(const QString &title);
		static QList<WindowHandle> findWindows(const QRegExp &regExp);

	private:
		WId mValue;
	};
}

Q_DECLARE_METATYPE(ActionTools::WindowHandle)

#endif // WINDOWHANDLE_H
