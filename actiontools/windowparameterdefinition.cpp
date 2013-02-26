/*
	Actionaz
	Copyright (C) 2008-2012 Jonathan Mercier-Ganady

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

#include "windowparameterdefinition.h"
#include "subparameter.h"
#include "windowedit.h"
#include "actioninstance.h"
#include "windowhandle.h"

namespace ActionTools
{
	WindowParameterDefinition::WindowParameterDefinition(const Name &name, QObject *parent)
		: ParameterDefinition(name, parent),
		mWindowEdit(0)
	{
	}

	void WindowParameterDefinition::buildEditors(Script *script, QWidget *parent)
	{
		ParameterDefinition::buildEditors(script, parent);

		mWindowEdit = new WindowEdit(parent);

		addEditor(mWindowEdit);
	}

	void WindowParameterDefinition::load(const ActionInstance *actionInstance)
	{
		mWindowEdit->setFromSubParameter(actionInstance->subParameter(name().original(), "value"));
	}

	void WindowParameterDefinition::save(ActionInstance *actionInstance)
	{
		actionInstance->setSubParameter(name().original(), "value", mWindowEdit->isCode(), mWindowEdit->text());
	}

	void WindowParameterDefinition::update(Script *script)
	{
		Q_UNUSED(script)

		QStringList windowTitles;

		foreach(const ActionTools::WindowHandle &windowHandle, ActionTools::WindowHandle::windowList())
		{
			QString title = windowHandle.title();
			if(title.isEmpty())
				continue;

			windowTitles << title;
		}

		windowTitles.sort();

		mWindowEdit->setWindowTitles(windowTitles);
	}
}
