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

#ifndef MULTIDATAINPUTINSTANCE_H
#define MULTIDATAINPUTINSTANCE_H

#include "actioninstance.h"

class QDialog;
class QGridLayout;
class QAbstractButton;
class QComboBox;
class QListWidget;
class QButtonGroup;

namespace Actions
{
	class MultiDataInputInstance : public ActionTools::ActionInstance
	{
		Q_OBJECT
		Q_ENUMS(Mode)

	public:
		enum Mode
		{
			ComboBoxMode,
			EditableComboBoxMode,
			ListMode,
			CheckboxMode,
			RadioButtonMode
		};

		MultiDataInputInstance(const ActionTools::ActionDefinition *definition, QObject *parent = 0);

		static ActionTools::StringListPair modes;

		void startExecution();
		void stopExecution();

	private slots:
		void accepted();
		void rejected();
		void listItemSelectionChanged();
		void checkboxChecked(QAbstractButton *checkbox);

	private:
		template<class T>
		QGridLayout *createRadioButtonsOrCheckboxes(const QString &defaultValue, bool exclusive);
		void saveSelectedRadioButtonOrCheckBox();
		void closeDialog();

		QDialog *mDialog;
		Mode mMode;
		QString mVariable;
		QStringList mItems;
		int mMinimumChoiceCount;
		int mMaximumChoiceCount;
		QComboBox *mComboBox;
		QListWidget *mListWidget;
		QButtonGroup *mButtonGroup;

		Q_DISABLE_COPY(MultiDataInputInstance)
	};
}

#endif // MULTIDATAINPUTINSTANCE_H
