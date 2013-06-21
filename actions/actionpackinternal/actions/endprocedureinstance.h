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

#ifndef ENDPROCEDUREINSTANCE_H
#define ENDPROCEDUREINSTANCE_H

#include "actioninstance.h"
#include "script.h"

namespace Actions
{
	class EndProcedureInstance : public ActionTools::ActionInstance
	{
		Q_OBJECT

	public:
		EndProcedureInstance(const ActionTools::ActionDefinition *definition, QObject *parent = 0)
			: ActionTools::ActionInstance(definition, parent)										{}

		void startExecution()
		{
			if(script()->hasProcedureCall())
				setNextLine(script()->popProcedureCall() + 2);//Lines start at 1
			else
			{
				emit executionException(ActionTools::ActionException::BadParameterException, tr("End procedure reached without a call"));

				return;
			}

			emit executionEnded();
		}

	private:
		Q_DISABLE_COPY(EndProcedureInstance)
	};
}

#endif // ENDPROCEDUREINSTANCE_H
