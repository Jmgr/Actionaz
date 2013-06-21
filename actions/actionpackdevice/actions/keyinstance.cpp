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

#include "keyinstance.h"
#include "keyinput.h"

#ifdef Q_WS_WIN
#include <Windows.h>
#endif

#include <QTimer>

namespace Actions
{
	ActionTools::StringListPair KeyInstance::actions = qMakePair(
			QStringList() << "pressRelease" << "press" << "release",
			QStringList() << QT_TRANSLATE_NOOP("KeyInstance::actions", "Press and release") << QT_TRANSLATE_NOOP("KeyInstance::actions", "Press") << QT_TRANSLATE_NOOP("KeyInstance::actions", "Release"));
	ActionTools::StringListPair KeyInstance::types = qMakePair(
			QStringList() << "win32" << "directx",
			QStringList() << QT_TRANSLATE_NOOP("KeyInstance::actions", "Win32") << QT_TRANSLATE_NOOP("KeyInstance::actions", "DirectX"));

	KeyInstance::KeyInstance(const ActionTools::ActionDefinition *definition, QObject *parent)
		: ActionTools::ActionInstance(definition, parent),
		  mCtrl(false),
		  mAlt(false),
		  mShift(false),
		  mMeta(false),
		  mTimer(new QTimer(this))
	{
		connect(mTimer, SIGNAL(timeout()), this, SLOT(sendRelease()));
	}

	void KeyInstance::startExecution()
	{
		bool ok = true;

		mKey = evaluateString(ok, "key", "key");
		Action action = evaluateListElement<Action>(ok, actions, "action");
		mAmount = evaluateInteger(ok, "amount");
		mCtrl = evaluateBoolean(ok, "ctrl");
		mAlt = evaluateBoolean(ok, "alt");
		mShift = evaluateBoolean(ok, "shift");
		mMeta = evaluateBoolean(ok, "meta");
		Type type = evaluateListElement<Type>(ok, types, "type");
		mPause  = evaluateInteger(ok, "pause");

		if(mPause < 0)
			mPause = 0;
		
		if (!ok)
			return;
		
		if(action != PressReleaseAction)
			mAmount = 1;

		if(mAmount <= 0)
		{
			setCurrentParameter("amount");
			emit executionException(ActionTools::ActionException::BadParameterException, tr("Invalid key presses amount"));
			return;
		}

		mKeyboardDevice.setType(static_cast<KeyboardDevice::Type>(type));

		bool result = true;

		switch(action)
		{
		case PressAction:
			pressOrReleaseModifiers(true);

			result &= mKeyboardDevice.pressKey(mKey);
			break;
		case ReleaseAction:
			pressOrReleaseModifiers(false);

			result &= mKeyboardDevice.releaseKey(mKey);
			break;
		case PressReleaseAction:
			pressOrReleaseModifiers(true);

			result &= mKeyboardDevice.pressKey(mKey);

			mTimer->setSingleShot(true);
			mTimer->start(mPause);
			break;
		}

		if(!result)
		{
			emit executionException(FailedToSendInputException, tr("Unable to emulate key: failed to send input"));
			return;
		}

		if(action != PressReleaseAction)
			emit executionEnded();
	}

	void KeyInstance::stopExecution()
	{
		mTimer->stop();
	}

	void KeyInstance::stopLongTermExecution()
	{
		mKeyboardDevice.reset();
	}

	void KeyInstance::sendRelease()
	{
		pressOrReleaseModifiers(false);
		mKeyboardDevice.releaseKey(mKey);

		--mAmount;
		if (mAmount > 0)
			emit sendPressKey();
		else
			emit executionEnded();
	}

	void KeyInstance::sendPressKey()
	{
		bool result = true;

		pressOrReleaseModifiers(true);

		result &= mKeyboardDevice.pressKey(mKey);

		if(!result)
		{
			emit executionException(FailedToSendInputException, tr("Unable to emulate key: failed to send input"));
			return;
		}

		mTimer->start(mPause);
	}

	void KeyInstance::pressOrReleaseModifiers(bool press)
	{
		if(press)
		{
			if(mCtrl)
				mKeyboardDevice.pressKey("controlLeft");
			if(mAlt)
				mKeyboardDevice.pressKey("altLeft");
			if(mShift)
				mKeyboardDevice.pressKey("shiftLeft");
			if(mMeta)
				mKeyboardDevice.pressKey("metaLeft");
		}
		else
		{
			if(mCtrl)
				mKeyboardDevice.releaseKey("controlLeft");
			if(mAlt)
				mKeyboardDevice.releaseKey("altLeft");
			if(mShift)
				mKeyboardDevice.releaseKey("shiftLeft");
			if(mMeta)
				mKeyboardDevice.releaseKey("metaLeft");
		}
	}
}
