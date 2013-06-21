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

#include "mousedevice.h"

#include <QCursor>

#ifdef Q_WS_X11
#include <QX11Info>
#include <X11/Xlib.h>
#include <X11/extensions/XTest.h>
#endif

#ifdef Q_WS_WIN
#include <Windows.h>
#endif

MouseDevice::MouseDevice()
{
	for(int i = 0; i < ButtonCount; ++i)
		mPressedButtons[i] = false;
}

MouseDevice::~MouseDevice()
{
	reset();
}

void MouseDevice::reset()
{
	for(int i = 0; i < ButtonCount; ++i)
	{
		if(mPressedButtons[i])
			releaseButton(static_cast<Button>(i));
	}
}

bool MouseDevice::isButtonPressed(Button button) const
{
#ifdef Q_WS_WIN
	switch(button)
	{
	case LeftButton:
		return (GetAsyncKeyState(GetSystemMetrics(SM_SWAPBUTTON) ? VK_RBUTTON : VK_LBUTTON) & 0x8000);
	case MiddleButton:
		return (GetAsyncKeyState(VK_MBUTTON) & 0x8000);
	case RightButton:
		return (GetAsyncKeyState(GetSystemMetrics(SM_SWAPBUTTON) ? VK_LBUTTON : VK_RBUTTON) & 0x8000);
	default:
		return false;
	}
#endif
#ifdef Q_WS_X11
	Window unusedWindow;
	int unusedInt;
	unsigned int buttonMask;
	if(!XQueryPointer(QX11Info::display(),
					  XDefaultRootWindow(QX11Info::display()),
					  &unusedWindow,
					  &unusedWindow,
					  &unusedInt,
					  &unusedInt,
					  &unusedInt,
					  &unusedInt,
					  &buttonMask))
		return false;

	switch(button)
	{
	case LeftButton:
		return (buttonMask & Button1Mask);
	case MiddleButton:
		return (buttonMask & Button2Mask);
	case RightButton:
		return (buttonMask & Button3Mask);
	default:
		return false;
	}
#endif
}

QPoint MouseDevice::cursorPosition() const
{
	return QCursor::pos();
}

void MouseDevice::setCursorPosition(const QPoint &position) const
{
	QCursor::setPos(position);
}

bool MouseDevice::buttonClick(Button button)
{
	return (pressButton(button) && releaseButton(button));
}

bool MouseDevice::pressButton(Button button)
{
	mPressedButtons[button] = true;

#ifdef Q_WS_X11
	if(!XTestFakeButtonEvent(QX11Info::display(), toX11Button(button), True, CurrentTime))
		return false;
	
	XFlush(QX11Info::display());
#endif
	
#ifdef Q_WS_WIN
	INPUT input;
	input.type = INPUT_MOUSE;
	input.mi.dx = 0;
	input.mi.dy = 0;
	input.mi.mouseData = 0;
	input.mi.dwFlags = toWinButton(button, true);
	input.mi.time = 0;

	if(!SendInput(1, &input, sizeof(INPUT)))
		return false;
#endif
	
	return true;
}

bool MouseDevice::releaseButton(Button button)
{
	mPressedButtons[button] = false;

#ifdef Q_WS_X11
	if(!XTestFakeButtonEvent(QX11Info::display(), toX11Button(button), False, CurrentTime))
		return false;
	
	XFlush(QX11Info::display());
#endif
	
#ifdef Q_WS_WIN
	INPUT input;
	input.type = INPUT_MOUSE;
	input.mi.dx = 0;
	input.mi.dy = 0;
	input.mi.mouseData = 0;
	input.mi.dwFlags = toWinButton(button, false);
	input.mi.time = 0;

	if(!SendInput(1, &input, sizeof(INPUT)))
		return false;
#endif
	
	return true;
}

bool MouseDevice::wheel(int intensity) const
{
#ifdef Q_WS_X11
	int button;
	if(intensity < 0)
	{
		button = Button5;
		intensity = -intensity;
	}
	else
		button = Button4;

	bool result = true;

	for(int i = 0; i < intensity; ++i)
	{
		result &= XTestFakeButtonEvent(QX11Info::display(), button, True, CurrentTime);
		result &= XTestFakeButtonEvent(QX11Info::display(), button, False, CurrentTime);
		
		XFlush(QX11Info::display());
	}

	if(!result)
		return false;
#endif
	
#ifdef Q_WS_WIN
	INPUT input;

	input.type = INPUT_MOUSE;
	input.mi.dwFlags = MOUSEEVENTF_WHEEL;
	input.mi.time = 0;
	input.mi.dwExtraInfo = 0;
	input.mi.mouseData = intensity * WHEEL_DELTA;

	if(!SendInput(1, &input, sizeof(INPUT)))
		return false;
#endif
	
	return true;
}

#ifdef Q_WS_X11
int MouseDevice::toX11Button(Button button) const
{
	return button + 1;
}
#endif

#ifdef Q_WS_WIN
int MouseDevice::toWinButton(Button button, bool press) const
{
	switch(button)
	{
	case LeftButton:
		return press ? MOUSEEVENTF_LEFTDOWN : MOUSEEVENTF_LEFTUP;
	case MiddleButton:
		return press ? MOUSEEVENTF_MIDDLEDOWN : MOUSEEVENTF_MIDDLEUP;
	case RightButton:
		return press ? MOUSEEVENTF_RIGHTDOWN : MOUSEEVENTF_RIGHTUP;
	}
	return -1;
}
#endif
