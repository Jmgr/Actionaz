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

#ifndef POINT_H
#define POINT_H

#include "actiontools_global.h"
#include "codeclass.h"

#include <QObject>
#include <QScriptValue>
#include <QScriptEngine>
#include <QPoint>

namespace Code
{
	class ACTIONTOOLSSHARED_EXPORT Point : public CodeClass
	{
		Q_OBJECT
		Q_PROPERTY(int x READ x WRITE setX)
		Q_PROPERTY(int y READ y WRITE setY)
		
	public:
		static QScriptValue constructor(QScriptContext *context, QScriptEngine *engine);
		static QScriptValue constructor(const QPoint &point, QScriptEngine *engine);
		static QPoint parameter(QScriptContext *context, QScriptEngine *engine);

		static void registerClass(QScriptEngine *scriptEngine);
		
		Point();
		Point(const Point &other);
		Point(const QPoint &point);
		
		Point &operator=(Point other);
		Point &operator=(QPoint point);
		
		void swap(Point &other);
		void swap(QPoint &point);
		
		const QPoint &point() const;

		int x() const;
		int y() const;
		
	public slots:
		QScriptValue clone() const;
		bool equals(const QScriptValue &other) const;
		QString toString() const;
		QScriptValue setX(int x);
		QScriptValue setY(int y);
		
	private:
		QPoint mPoint;
	};
}

#endif // POINT_H
