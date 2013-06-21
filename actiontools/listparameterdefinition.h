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

#ifndef LISTPARAMETERDEFINITION_H
#define LISTPARAMETERDEFINITION_H

#include "itemsparameterdefinition.h"
#include "actiontools_global.h"

namespace ActionTools
{
	class CodeComboBox;

	class ACTIONTOOLSSHARED_EXPORT ListParameterDefinition : public ItemsParameterDefinition
	{
		Q_OBJECT

	public:
		ListParameterDefinition(const Name &name, QObject *parent);
		virtual ~ListParameterDefinition()	{}

		virtual void buildEditors(Script *script, QWidget *parent);
		void load(const ActionInstance *actionInstance);
		void save(ActionInstance *actionInstance);

		CodeComboBox *codeComboBox() const										{ return mComboBox; }
		
	signals:
		void editorBuilt();

	protected:
		CodeComboBox *mComboBox;

	private:
		Q_DISABLE_COPY(ListParameterDefinition)

	};
}

#endif // LISTPARAMETERDEFINITION_H
