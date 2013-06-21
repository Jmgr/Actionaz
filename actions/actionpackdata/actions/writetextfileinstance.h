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

#ifndef WRITETEXTFILEINSTANCE_H
#define WRITETEXTFILEINSTANCE_H

#include "actioninstance.h"
#include "stringlistpair.h"

namespace Actions
{
	class WriteTextFileInstance : public ActionTools::ActionInstance
	{
		Q_OBJECT
		Q_ENUMS(Mode)

	public:
		enum Mode
		{
			Truncate,
			Append
		};
		enum Exceptions
		{
			CannotWriteFileException = ActionTools::ActionException::UserException
		};

		WriteTextFileInstance(const ActionTools::ActionDefinition *definition, QObject *parent = 0)
			: ActionTools::ActionInstance(definition, parent)												{}

		static ActionTools::StringListPair modes;

		void startExecution();

	private:
		Q_DISABLE_COPY(WriteTextFileInstance)
	};
}

#endif // WRITETEXTFILEINSTANCE_H
