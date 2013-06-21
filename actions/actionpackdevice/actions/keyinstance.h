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

#ifndef KEYINSTANCE_H
#define KEYINSTANCE_H

#include "actioninstance.h"
#include "../keyboarddevice.h"
#include "stringlistpair.h"

class QTimer;

namespace Actions
{
	class KeyInstance : public ActionTools::ActionInstance
	{
		Q_OBJECT

	public:
		enum Action
		{
			PressReleaseAction,
			PressAction,
			ReleaseAction
		};
		enum Type
		{
			Win32Type,
			DirectXType
		};
		enum Exceptions
		{
			FailedToSendInputException = ActionTools::ActionException::UserException,
			InvalidActionException
		};

		KeyInstance(const ActionTools::ActionDefinition *definition, QObject *parent = 0);
		
		static ActionTools::StringListPair actions;
		static ActionTools::StringListPair types;

		void startExecution();
		void stopExecution();
		void stopLongTermExecution();

	private slots:
		void sendRelease();
		void sendPressKey();

	private:
		void pressOrReleaseModifiers(bool press);

		KeyboardDevice mKeyboardDevice;
		QString mKey;
		bool mCtrl;
		bool mAlt;
		bool mShift;
		bool mMeta;
		int  mAmount;
		int  mPause;
		QTimer *mTimer;

		Q_DISABLE_COPY(KeyInstance)
	};
}

#endif // KEYINSTANCE_H
