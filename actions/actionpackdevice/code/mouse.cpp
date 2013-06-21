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

#include "mouse.h"
#include "code/point.h"

#include <QScriptValueIterator>

namespace Code
{
	QScriptValue Mouse::constructor(QScriptContext *context, QScriptEngine *engine)
	{
		Mouse *mouse = new Mouse;

		QScriptValueIterator it(context->argument(0));

		while(it.hasNext())
		{
			it.next();

			if(it.name() == "onMotion")
				mouse->mOnMotion = it.value();
			else if(it.name() == "onWheel")
				mouse->mOnWheel = it.value();
			else if(it.name() == "onButtonPressed")
				mouse->mOnButtonPressed = it.value();
			else if(it.name() == "onButtonReleased")
				mouse->mOnButtonReleased = it.value();
		}

		return CodeClass::constructor(mouse, context, engine);
	}
	
	Mouse::Mouse()
		: CodeClass()
	{
	}

	Mouse::~Mouse()
	{
	}

	QScriptValue Mouse::position() const
	{
		return Point::constructor(mMouseDevice.cursorPosition(), engine());
	}

	QScriptValue Mouse::move() const
	{
		mMouseDevice.setCursorPosition(Point::parameter(context(), engine()));
		
		return thisObject();
	}
	
	bool Mouse::isButtonPressed(Button button) const
	{
		return mMouseDevice.isButtonPressed(static_cast<MouseDevice::Button>(button));
	}
	
	QScriptValue Mouse::press(Button button)
	{
		if(!mMouseDevice.pressButton(static_cast<MouseDevice::Button>(button)))
			throwError("PressButtonError", tr("Unable to press the button"));
		
		return thisObject();
	}

	QScriptValue Mouse::release(Button button)
	{
		if(!mMouseDevice.releaseButton(static_cast<MouseDevice::Button>(button)))
			throwError("ReleaseButtonError", tr("Unable to release the button"));
		
		return thisObject();
	}

	QScriptValue Mouse::click(Button button)
	{
		if(!mMouseDevice.buttonClick(static_cast<MouseDevice::Button>(button)))
			throwError("ClickError", tr("Unable to emulate a button click"));
		
		return thisObject();
	}
	
	QScriptValue Mouse::wheel(int intensity) const
	{
		if(!mMouseDevice.wheel(intensity))
			throwError("WheelError", tr("Unable to emulate the wheel"));
		
		return thisObject();
	}

	void Mouse::mouseMotion(int x, int y)
	{
		if(mOnMotion.isValid())
			mOnMotion.call(thisObject(), QScriptValueList() << x << y);
	}

	void Mouse::mouseWheel(int intensity)
	{
		if(mOnWheel.isValid())
			mOnWheel.call(thisObject(), QScriptValueList() << intensity);
	}

	void Mouse::mouseButtonPressed(ActionTools::SystemInput::Button button)
	{
		if(mOnButtonPressed.isValid())
			mOnButtonPressed.call(thisObject(), QScriptValueList() << button);
	}

	void Mouse::mouseButtonReleased(ActionTools::SystemInput::Button button)
	{
		if(mOnButtonReleased.isValid())
			mOnButtonReleased.call(thisObject(), QScriptValueList() << button);
	}
}
