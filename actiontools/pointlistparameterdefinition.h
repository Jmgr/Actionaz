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

#ifndef POINTLISTPARAMETERDEFINITION_H
#define POINTLISTPARAMETERDEFINITION_H

#include "parameterdefinition.h"
#include "actiontools_global.h"

namespace ActionTools
{
	class PointListWidget;

	class ACTIONTOOLSSHARED_EXPORT PointListParameterDefinition : public ParameterDefinition
	{
		Q_OBJECT

	public:
		PointListParameterDefinition(const Name &name, QObject *parent);

		void buildEditors(Script *script, QWidget *parent);
		void load(const ActionInstance *actionInstance);
		void save(ActionInstance *actionInstance);

	private:
		PointListWidget *mPointListWidget;

		Q_DISABLE_COPY(PointListParameterDefinition)
	};
}

#endif // POINTLISTPARAMETERDEFINITION_H
