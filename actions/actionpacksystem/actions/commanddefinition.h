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

#ifndef COMMANDDEFINITION_H
#define COMMANDDEFINITION_H

#include "actiondefinition.h"
#include "commandinstance.h"
#include "fileparameterdefinition.h"
#include "variableparameterdefinition.h"

namespace ActionTools
{
	class ActionPack;
	class ActionInstance;
}

namespace Actions
{
	class CommandDefinition : public QObject, public ActionTools::ActionDefinition
	{
	   Q_OBJECT

	public:
		explicit CommandDefinition(ActionTools::ActionPack *pack)
		: ActionDefinition(pack)
		{
			ActionTools::FileParameterDefinition *command = new ActionTools::FileParameterDefinition(ActionTools::Name("command", tr("Command")), this);
			command->setTooltip(tr("The command to execute"));
			addElement(command);

			ActionTools::TextParameterDefinition *parameters = new ActionTools::TextParameterDefinition(ActionTools::Name("parameters", tr("Parameters")), this);
			parameters->setTooltip(tr("The command's parameters"));
			addElement(parameters);

			ActionTools::FileParameterDefinition *workingDirectory = new ActionTools::FileParameterDefinition(ActionTools::Name("workingDirectory", tr("Working directory")), this);
			workingDirectory->setTooltip(tr("The command's working directory"));
			workingDirectory->setCaption(tr("Command working directory"));
			workingDirectory->setMode(ActionTools::FileEdit::DirectoryOpen);
			addElement(workingDirectory);

			ActionTools::VariableParameterDefinition *exitCode = new ActionTools::VariableParameterDefinition(ActionTools::Name("exitCode", tr("Exit code")), this);
			exitCode->setTooltip(tr("The command's exit code"));
			addElement(exitCode, 1);

			ActionTools::VariableParameterDefinition *processId = new ActionTools::VariableParameterDefinition(ActionTools::Name("processId", tr("Process id")), this);
			processId->setTooltip(tr("The command's process id"));
			addElement(processId, 1);

			ActionTools::VariableParameterDefinition *output = new ActionTools::VariableParameterDefinition(ActionTools::Name("output", tr("Output")), this);
			output->setTooltip(tr("The command's output"));
			addElement(output);

			ActionTools::VariableParameterDefinition *errorOutput = new ActionTools::VariableParameterDefinition(ActionTools::Name("errorOutput", tr("Error output")), this);
			errorOutput->setTooltip(tr("The command's error output"));
			addElement(errorOutput, 1);

			ActionTools::VariableParameterDefinition *exitStatus = new ActionTools::VariableParameterDefinition(ActionTools::Name("exitStatus", tr("Exit status")), this);
			exitStatus->setTooltip(tr("The command's exit status"));
			addElement(exitStatus, 1);

			addException(CommandInstance::FailedToStartException, tr("Failed to start the command"));
		}

		QString name() const													{ return QObject::tr("Command"); }
		QString id() const														{ return "ActionCommand"; }
		ActionTools::Flag flags() const											{ return ActionDefinition::flags() | ActionTools::Official; }
		QString description() const												{ return QObject::tr("Executes a command"); }
		ActionTools::ActionInstance *newActionInstance() const					{ return new CommandInstance(this); }
		ActionTools::ActionCategory category() const							{ return ActionTools::System; }
		QPixmap icon() const													{ return QPixmap(":/icons/command.png"); }
		QStringList tabs() const												{ return ActionDefinition::StandardTabs; }

	private:
		Q_DISABLE_COPY(CommandDefinition)
	};
}

#endif // COMMANDDEFINITION_H
