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

#ifndef SCREENPOSITIONPARAMETERDEFINITION_H
#define SCREENPOSITIONPARAMETERDEFINITION_H

#include "parameterdefinition.h"
#include "actiontools_global.h"
#include "windowpositionedit.h"
#include "booleanedit.h"

namespace ActionTools
{
	class BooleanEdit;
	class WindowPositionEdit;

	class ACTIONTOOLSSHARED_EXPORT ScreenPositionParameterDefinition : public ParameterDefinition
	{
		Q_OBJECT

	public:
		ScreenPositionParameterDefinition(const Name &name, QObject *parent);

		void buildEditors(Script *script, QWidget *parent);
		void load(const ActionInstance *actionInstance);
		void save(ActionInstance *actionInstance);
		void setDefaultValues(ActionInstance *actionInstance);
		Qt::Orientation editorsOrientation() const								{ return Qt::Vertical; }

		void setDefaultScreenPosition(const QString &position)					{ mDefaultScreenPosition = position; }
		QString defaultScreenPosition() const									{ return mDefaultScreenPosition; }

		void setDefaultWindowVisible(const bool &visible)						{ mDefaultWindowVisible = visible; }
		QString defaultWindowVisible() const									{ if(mDefaultWindowVisible) return "true"; else return "false"; }

	private:
		QVariant defaultValue(QVariant defaultValue = QVariant()) const			{ Q_UNUSED(defaultValue); return QVariant(); }
		void setDefaultValue(const QVariant &defaultValue)						{ Q_UNUSED(defaultValue); }

		WindowPositionEdit *mWindowPositionEdit;
		BooleanEdit *mBooleanEdit;
		QString mDefaultScreenPosition;
		bool mDefaultWindowVisible;

		Q_DISABLE_COPY(ScreenPositionParameterDefinition)
	};
}

#endif // SCREENPOSITIONPARAMETERDEFINITION_H
