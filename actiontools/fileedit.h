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

#ifndef FILEEDIT_H
#define FILEEDIT_H

#include "actiontools_global.h"
#include "subparameter.h"
#include "abstractcodeeditor.h"

#include <QWidget>

namespace Ui
{
	class FileEdit;
}

namespace ActionTools
{
	class CodeLineEdit;

	class ACTIONTOOLSSHARED_EXPORT FileEdit : public QWidget, public AbstractCodeEditor
	{
		Q_OBJECT

	public:
		enum Mode
		{
			FileOpen,
			FileSave,
			DirectoryOpen,
			DirectorySave
		};

		explicit FileEdit(QWidget *parent = 0);
		~FileEdit();

		void setText(const QString &text);
		QString text() const;
		CodeLineEdit *codeLineEdit() const;
		bool isCode() const;
		void setCode(bool code);
		void setFromSubParameter(const SubParameter &subParameter);
		void openEditor(int line = -1, int column = -1);
		void setCompletionModel(QAbstractItemModel *completionModel);

		void setMode(Mode mode)						{ mMode = mode; }
		void setCaption(const QString &caption)		{ mCaption = caption; }
		void setFilter(const QString &filter)		{ mFilter = filter; }
		void setDirectory(const QString &directory)	{ mDirectory = directory; }

		Mode mode()	const							{ return mMode; }
		QString caption() const						{ return mCaption; }
		QString filter() const						{ return mFilter; }
		QString directory() const					{ return mDirectory; }

	private slots:
		void on_browse_clicked();

	private:
		Ui::FileEdit *ui;
		Mode mMode;
		QString mCaption;
		QString mFilter;
		QString mDirectory;

		Q_DISABLE_COPY(FileEdit)
	};
}

#endif // FILEEDIT_H
