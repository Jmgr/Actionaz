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

#ifndef TEXTINSTANCE_H
#define TEXTINSTANCE_H

#include "actioninstance.h"
#include "../keyboarddevice.h"

class QTimer;

namespace Actions
{
	class TextInstance : public ActionTools::ActionInstance
	{
		Q_OBJECT
	
	public:
		enum Exceptions
		{
			FailedToSendInputException = ActionTools::ActionException::UserException
		};
	
		TextInstance(const ActionTools::ActionDefinition *definition, QObject *parent = 0);
	
		void startExecution();
		void stopExecution();
		void stopLongTermExecution();

	private slots:
		void pressNextKey();
	
	private:
		KeyboardDevice mKeyboardDevice;
		QTimer *mTimer;
		QString mText;
		int mCurrentCharacter;
		
		Q_DISABLE_COPY(TextInstance)
	};
}

#endif // TEXTINSTANCE_H
