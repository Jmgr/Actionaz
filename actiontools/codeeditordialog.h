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

#ifndef CODEEDITORDIALOG_H
#define CODEEDITORDIALOG_H

#include "actiontools_global.h"

#include <QDialog>

namespace Ui
{
	class CodeEditorDialog;
}

class QAbstractItemModel;

namespace ActionTools
{
	class ACTIONTOOLSSHARED_EXPORT CodeEditorDialog : public QDialog
	{
		Q_OBJECT
	public:
		CodeEditorDialog(QAbstractItemModel *completionModel, QWidget *parent = 0);
		~CodeEditorDialog();

		void setText(const QString &text);
		void setCode(bool code);
		void setAllowTextCodeChange(bool allowTextCodeChange);
		void setCurrentLine(int line);
		void setCurrentColumn(int column);

		QString text() const;
		bool isCode() const;

	public slots:
		void accept();

	private slots:
		void on_codePushButton_toggled(bool checked);
		void on_checkSyntax_clicked();
		void swapCode();

	private:
		void showSyntaxCheckError();
		void moveCursorToLine(int line);

		Ui::CodeEditorDialog *ui;

		Q_DISABLE_COPY(CodeEditorDialog)
	};
}

#endif // CODEEDITORDIALOG_H
