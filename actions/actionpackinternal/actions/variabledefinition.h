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

#ifndef VARIABLEDEFINITION_H
#define VARIABLEDEFINITION_H

#include "actiondefinition.h"
#include "variableinstance.h"
#include "variableparameterdefinition.h"
#include "textparameterdefinition.h"
#include "listparameterdefinition.h"
#include "colorparameterdefinition.h"
#include "positionparameterdefinition.h"
#include "groupdefinition.h"

namespace ActionTools
{
	class ActionPack;
	class ActionInstance;
}

namespace Actions
{
	class VariableDefinition : public QObject, public ActionTools::ActionDefinition
	{
	   Q_OBJECT

	public:
		explicit VariableDefinition(ActionTools::ActionPack *pack)
		: ActionDefinition(pack)
		{
			translateItems("VariableInstance::types", VariableInstance::types);

			ActionTools::VariableParameterDefinition *variable = new ActionTools::VariableParameterDefinition(ActionTools::Name("variable", tr("Variable")), this);
			variable->setCategory(ActionTools::ElementDefinition::INPUT);
			variable->setTooltip(tr("The variable name"));
			addElement(variable);

			ActionTools::ListParameterDefinition *type = new ActionTools::ListParameterDefinition(ActionTools::Name("type", tr("Type")), this);
			type->setTooltip(tr("The variable type"));
			type->setItems(VariableInstance::types);
			type->setDefaultValue(VariableInstance::types.second.at(VariableInstance::String));
			addElement(type);

			ActionTools::GroupDefinition *singleValueGroup = new ActionTools::GroupDefinition(this);
			singleValueGroup->setMasterList(type);
			singleValueGroup->setMasterValues(QStringList()
											  << VariableInstance::types.first.at(VariableInstance::String)
											  << VariableInstance::types.first.at(VariableInstance::Integer)
											  << VariableInstance::types.first.at(VariableInstance::Float)
											  );

			ActionTools::TextParameterDefinition *simpleValue = new ActionTools::TextParameterDefinition(ActionTools::Name("value", tr("Value")), this);
			simpleValue->setTooltip(tr("The variables new value"));
			singleValueGroup->addMember(simpleValue);

			addElement(singleValueGroup);

			ActionTools::GroupDefinition *colorValueGroup = new ActionTools::GroupDefinition(this);
			colorValueGroup->setMasterList(type);
			colorValueGroup->setMasterValues(QStringList()
											  << VariableInstance::types.first.at(VariableInstance::Color)
											  );

			ActionTools::ColorParameterDefinition *colorValue = new ActionTools::ColorParameterDefinition(ActionTools::Name("colorValue", tr("Color")), this);
			colorValue->setTooltip(tr("The variables new value"));
			colorValueGroup->addMember(colorValue);

			addElement(colorValueGroup);

			ActionTools::GroupDefinition *positionValueGroup = new ActionTools::GroupDefinition(this);
			positionValueGroup->setMasterList(type);
			positionValueGroup->setMasterValues(QStringList()
											  << VariableInstance::types.first.at(VariableInstance::Position)
											  );

			ActionTools::PositionParameterDefinition *positionValue = new ActionTools::PositionParameterDefinition(ActionTools::Name("positionValue", tr("Position")), this);
			positionValue->setTooltip(tr("The variables new value"));
			positionValueGroup->addMember(positionValue);

			addElement(positionValueGroup);

			addException(VariableInstance::ConversionFailedException, tr("Conversion failed"));
		}

		QString name() const													{ return QObject::tr("Variable"); }
		QString id() const														{ return "ActionVariable"; }
		ActionTools::Flag flags() const											{ return ActionDefinition::flags() | ActionTools::Official; }
		QString description() const												{ return QObject::tr("Set the value of a variable"); }
		ActionTools::ActionInstance *newActionInstance() const					{ return new VariableInstance(this); }
		ActionTools::ActionCategory category() const							{ return ActionTools::Internal; }
		QPixmap icon() const													{ return QPixmap(":/actions/icons/variable.png"); }

	private:
		Q_DISABLE_COPY(VariableDefinition)
	};
}

#endif // VARIABLEDEFINITION_H
