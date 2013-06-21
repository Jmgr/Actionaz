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

#ifndef CODEDATETIMEEDIT_H
#define CODEDATETIMEEDIT_H

#include "actiontools_global.h"
#include "subparameter.h"
#include "abstractcodeeditor.h"

#include <QDateTimeEdit>

namespace ActionTools
{
	class CodeLineEdit;

	class ACTIONTOOLSSHARED_EXPORT CodeDateTimeEdit : public QDateTimeEdit, public AbstractCodeEditor
	{
		Q_OBJECT

	public:
		explicit CodeDateTimeEdit(QWidget *parent = 0);

		CodeLineEdit *codeLineEdit() const;
		bool isCode() const;
		void setCode(bool code);

		void setFromSubParameter(const SubParameter &subParameter);

		void openEditor(int line = -1, int column = -1);
		void setCompletionModel(QAbstractItemModel *completionModel);

	private slots:
		void codeChanged(bool code);

	private:
		QValidator::State validate(QString &text, int &pos) const;
		QString textFromDateTime(const QDateTime &dateTime) const;
		void contextMenuEvent(QContextMenuEvent *event);

		Q_DISABLE_COPY(CodeDateTimeEdit)
	};
}

#endif // CODEDATETIMEEDIT_H
