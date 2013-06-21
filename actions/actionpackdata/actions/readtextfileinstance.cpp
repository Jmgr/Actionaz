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

#include "readtextfileinstance.h"

#include <QFile>
#include <QTextStream>

namespace Actions
{
	ActionTools::StringListPair ReadTextFileInstance::modes = qMakePair(
			QStringList() << "full" << "selection",
			QStringList() << QT_TRANSLATE_NOOP("ReadTextFileInstance::modes", "Read the entire file") << QT_TRANSLATE_NOOP("ReadTextFileInstance::modes", "Read only a selection"));

	void ReadTextFileInstance::startExecution()
	{
		bool ok = true;

		QString filepath = evaluateString(ok, "file");
		QString variable = evaluateVariable(ok, "variable");
		Mode mode = evaluateListElement<Mode>(ok, modes, "mode");
		int firstline = evaluateInteger(ok, "firstline");
		int lastline = evaluateInteger(ok, "lastline");

		if(!ok)
			return;

		if(mode == Selection)
		{
			if(firstline < 1)
			{
				setCurrentParameter("firstline");
				emit executionException(ActionTools::ActionException::BadParameterException, tr("Invalid first line value : %1").arg(firstline));
				return;
			}

			if(lastline < 1)
			{
				setCurrentParameter("lastline");
				emit executionException(ActionTools::ActionException::BadParameterException, tr("Invalid last line value : %1").arg(lastline));
				return;
			}

			if(lastline < firstline)
			{
				setCurrentParameter("firstline");
				emit executionException(ActionTools::ActionException::BadParameterException, tr("The first line has to be smaller than the last line"));
				return;
			}
		}

		QFile file(filepath);
		if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
		{
			setCurrentParameter("file");
			emit executionException(CannotOpenFileException, tr("Cannot open file"));
			return;
		}

		//Line counting starts at 1
		--firstline;
		--lastline;

		QString result;
		QTextStream stream(&file);

		stream.setAutoDetectUnicode(true);

		if(mode == Full)
			result = stream.readAll();
		else
		{
			for(int line = 0; !stream.atEnd(); ++line)
			{
				QString readLine = stream.readLine();

				if(line >= firstline && line <= lastline)
				{
					if(!result.isEmpty())
						result += '\n';

					result += readLine;
				}

				if(line > lastline)
					break;
			}
		}

        setVariable(variable, result);

		file.close();

		emit executionEnded();
	}
}
