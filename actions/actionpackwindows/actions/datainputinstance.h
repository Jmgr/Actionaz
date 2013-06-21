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

#ifndef DATAINPUTINSTANCE_H
#define DATAINPUTINSTANCE_H

#include "actioninstance.h"
#include "stringlistpair.h"

class QInputDialog;

namespace Actions
{
	class DataInputInstance : public ActionTools::ActionInstance
	{
		Q_OBJECT

	public:
		enum DataType
		{
			TextType,
			IntegerType,
			DecimalType
		};

		DataInputInstance(const ActionTools::ActionDefinition *definition, QObject *parent = 0);

		static ActionTools::StringListPair dataTypes;

		void startExecution();
		void stopExecution();

	private slots:
		void dataEntered(int);
		void dataEntered(double);
		void dataEntered(const QString &value);
		void canceled();

	private:
		QInputDialog *mInputDialog;
		QString mVariable;
		DataType mDataType;

		Q_DISABLE_COPY(DataInputInstance)
	};
}

#endif // DATAINPUTINSTANCE_H
