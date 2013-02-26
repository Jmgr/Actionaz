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

#ifndef READENVIRONMENTDEFINITION_H
#define READENVIRONMENTDEFINITION_H

#include "actiondefinition.h"
#include "readenvironmentinstance.h"
#include "variableparameterdefinition.h"
#include "listparameterdefinition.h"
#include "environmentvariableparameterdefinition.h"
#include "groupdefinition.h"

namespace ActionTools
{
	class ActionPack;
	class ActionInstance;
}

namespace Actions
{
	class ReadEnvironmentVariableDefinition : public QObject, public ActionTools::ActionDefinition
	{
	   Q_OBJECT

	public:
		explicit ReadEnvironmentVariableDefinition(ActionTools::ActionPack *pack)
		: ActionDefinition(pack)
		{
			ActionTools::VariableParameterDefinition *variable = new ActionTools::VariableParameterDefinition(ActionTools::Name("variable", tr("Variable")), this);
			variable->setTooltip(tr("The variable used to store the selected information from your system environment"));
			addElement(variable);

			ActionTools::ListParameterDefinition *mode = new ActionTools::ListParameterDefinition(ActionTools::Name("mode", tr("Mode")), this);
			mode->setTooltip(tr("The environment read mode"));
			mode->setItems(ReadEnvironmentVariableInstance::modes);
			mode->setDefaultValue(ReadEnvironmentVariableInstance::modes.second.at(ReadEnvironmentVariableInstance::oneVariable));
			addElement(mode);

			ActionTools::GroupDefinition *selectionMode = new ActionTools::GroupDefinition(this);
			selectionMode->setMasterList(mode);
			selectionMode->setMasterValues(QStringList() << ReadEnvironmentVariableInstance::modes.first.at(ReadEnvironmentVariableInstance::oneVariable));

			ActionTools::EnvironmentVariableParameterDefinition *environmentVariableName = new ActionTools::EnvironmentVariableParameterDefinition(ActionTools::Name("environmentVariableName", tr("Environment Variable")), this);
			environmentVariableName->setTooltip(tr("The specific environment variable to read"));
			selectionMode->addMember(environmentVariableName);

			addElement(selectionMode);
		}

		QString name() const													{ return QObject::tr("Read environment variables"); }
		QString id() const														{ return "ActionReadEnvironmentVariable"; }
		ActionTools::Flag flags() const											{ return ActionDefinition::flags() | ActionTools::Official; }
		QString description() const												{ return QObject::tr("Read the system environment"); }
		ActionTools::ActionInstance *newActionInstance() const					{ return new ReadEnvironmentVariableInstance(this); }
		ActionTools::ActionCategory category() const							{ return ActionTools::Data; }
		QPixmap icon() const													{ return QPixmap(":/icons/readenvironment.png"); }

	private:
		Q_DISABLE_COPY(ReadEnvironmentVariableDefinition)
	};
}

#endif // READENVIRONMENTDEFINITION_H
