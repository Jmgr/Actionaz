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

#include "textinstance.h"

#include <QTimer>

namespace Actions
{
	TextInstance::TextInstance(const ActionTools::ActionDefinition *definition, QObject *parent)
		: ActionTools::ActionInstance(definition, parent),
		  mTimer(new QTimer(this)),
		  mCurrentCharacter(0)
	{
		connect(mTimer, SIGNAL(timeout()), this, SLOT(pressNextKey()));

		mTimer->setSingleShot(false);
	}

	void TextInstance::startExecution()
	{
		bool ok = true;
	
		mText = evaluateString(ok, "text");
		int pause  = evaluateInteger(ok, "pause");

		if(pause < 0)
			pause = 0;

		mTimer->setInterval(pause);
		mCurrentCharacter = 0;
	
		if(!ok)
			return;
		
		if(pause == 0)
		{
			if(!mKeyboardDevice.writeText(mText))
			{
				emit executionException(FailedToSendInputException, tr("Unable to write the text"));
				return;
			}

			QTimer::singleShot(1, this, SIGNAL(executionEnded()));
		}
		else
		{
			pressNextKey();

			mTimer->start();
		}
	}

	void TextInstance::stopExecution()
	{
		mTimer->stop();
	}

	void TextInstance::stopLongTermExecution()
	{
		mKeyboardDevice.reset();
	}

	void TextInstance::pressNextKey()
	{
		if(!mKeyboardDevice.writeText(QString(mText.at(mCurrentCharacter))))
		{
			mTimer->stop();
			emit executionException(FailedToSendInputException, tr("Unable to write the text"));
			return;
		}

		++mCurrentCharacter;
		if(mCurrentCharacter >= mText.size())
		{
			mTimer->stop();

			QTimer::singleShot(1, this, SIGNAL(executionEnded()));

			return;
		}
	}
}
