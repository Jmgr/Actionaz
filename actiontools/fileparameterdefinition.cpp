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

#include "fileparameterdefinition.h"
#include "subparameter.h"
#include "fileedit.h"
#include "actioninstance.h"

namespace ActionTools
{
	FileParameterDefinition::FileParameterDefinition(const Name &name, QObject *parent)
		: ParameterDefinition(name, parent),
		mFileEdit(0)
	{
	}

	void FileParameterDefinition::buildEditors(Script *script, QWidget *parent)
	{
		ParameterDefinition::buildEditors(script, parent);

		mFileEdit = new FileEdit(parent);

		mFileEdit->setMode(mMode);
		mFileEdit->setCaption(mCaption);
		mFileEdit->setFilter(mFilter);
		mFileEdit->setDirectory(mDirectory);

		addEditor(mFileEdit);
	}

	void FileParameterDefinition::load(const ActionInstance *actionInstance)
	{
		mFileEdit->setFromSubParameter(actionInstance->subParameter(name().original(), "value"));
	}

	void FileParameterDefinition::save(ActionInstance *actionInstance)
	{
		actionInstance->setSubParameter(name().original(), "value", mFileEdit->isCode(), mFileEdit->text());
	}
}
