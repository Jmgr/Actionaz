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

#ifndef VARIABLEINSTANCE_H
#define VARIABLEINSTANCE_H

#include "actioninstance.h"
#include "script.h"
#include "code/color.h"
#include "code/point.h"

#include <QPoint>

namespace Actions
{
	class VariableInstance : public ActionTools::ActionInstance
	{
		Q_OBJECT
		Q_ENUMS(Type)

	public:
		enum Type
		{
			String,
			Integer,
			Float,
			Color,
            Position
		};
		enum Exceptions
		{
			ConversionFailedException = ActionTools::ActionException::UserException
		};

		VariableInstance(const ActionTools::ActionDefinition *definition, QObject *parent = 0)
			: ActionTools::ActionInstance(definition, parent)										{}

		static ActionTools::StringListPair types;

		void startExecution()
		{
			bool ok = true;

			QString variable = evaluateString(ok, "variable");
			QString value = evaluateString(ok, "value");
			QColor colorValue = evaluateColor(ok, "colorValue");
			QPoint positionValue = evaluatePoint(ok, "positionValue");
			Type type = evaluateListElement<Type>(ok, types, "type");

			if(!ok)
				return;

			switch(type)
			{
			case String:
				{
                    setVariable(variable, value);
				}
				break;
			case Integer:
				{
					bool ok;
                    setVariable(variable, value.toInt(&ok));
					if(!ok)
					{
						emit executionException(ConversionFailedException, tr("Cannot evaluate the value as an integer"));
						return;
					}
				}
				break;
			case Float:
				{
					bool ok;
                    setVariable(variable, value.toFloat(&ok));
					if(!ok)
					{
						emit executionException(ConversionFailedException, tr("Cannot evaluate the value as a floating number"));
						return;
					}
				}
				break;
			case Color:
				{
                    setVariable(variable, Code::Color::constructor(colorValue, scriptEngine()));
				}
				break;
			case Position:
				{
                    setVariable(variable, Code::Point::constructor(positionValue, scriptEngine()));
				}
				break;
			}

			emit executionEnded();
		}

	private:
		Q_DISABLE_COPY(VariableInstance)
	};
}

#endif // VARIABLEINSTANCE_H
