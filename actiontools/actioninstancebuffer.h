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

#ifndef ACTIONINSTANCEBUFFER_H
#define ACTIONINSTANCEBUFFER_H

#include "actiontools_global.h"
#include "actioninstance.h"

#include <QSharedData>

namespace ActionTools
{
	class ActionInstanceBufferData : public QSharedData
	{
	public:
		ActionInstanceBufferData()									{}
		ActionInstanceBufferData(const ActionInstanceBufferData &other)
			: QSharedData(other),
			actionInstanceId(other.actionInstanceId)
		{
			action.copyActionDataFrom(other.action);
		}

		QString actionInstanceId;
		ActionInstance action;
	};

	class ACTIONTOOLSSHARED_EXPORT ActionInstanceBuffer
	{
	public:
		ActionInstanceBuffer(const QString &actionInstanceId, const ActionInstance &action)
		{
			d = new ActionInstanceBufferData();
			setActionInstanceId(actionInstanceId);
			setAction(action);
		}
		ActionInstanceBuffer(const QString &actionInstanceId, const QVariant &action)
		{
			d = new ActionInstanceBufferData();
			setActionInstanceId(actionInstanceId);
			setActionFromVariant(action);
		}
		ActionInstanceBuffer()
		{
			d = new ActionInstanceBufferData();
		}
		ActionInstanceBuffer(const ActionInstanceBuffer &other)
			: d(other.d)									{}

		void setActionInstanceId(const QString &actionInstanceId)	{ d->actionInstanceId = actionInstanceId; }
		void setAction(const ActionInstance &action)				{ d->action.copyActionDataFrom(action); }
		void setActionFromVariant(const QVariant &variant)			{ d->action.copyActionDataFrom(variant.value<ActionInstance>()); }

		QString actionInstanceId() const							{ return d->actionInstanceId; }
		ActionInstance action() const								{ return d->action; }
		QVariant actionAsVariant() const							{ return QVariant::fromValue<ActionInstance>(d->action); }

	private:
		QSharedDataPointer<ActionInstanceBufferData> d;
	};

	ACTIONTOOLSSHARED_EXPORT QDataStream &operator << (QDataStream &s, const ActionInstanceBuffer &actionInstanceBuffer);
	ACTIONTOOLSSHARED_EXPORT QDataStream &operator >> (QDataStream &s, ActionInstanceBuffer &actionInstanceBuffer);
}

Q_DECLARE_METATYPE(ActionTools::ActionInstanceBuffer)

#endif // ACTIONINSTANCEBUFFER_H
