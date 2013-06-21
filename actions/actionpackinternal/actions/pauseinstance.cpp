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

#include "pauseinstance.h"

namespace Actions
{
	ActionTools::StringListPair PauseInstance::units = qMakePair(
			QStringList() << "milliseconds" << "seconds" << "minutes" << "hours" << "days",
			QStringList()
			<< QT_TRANSLATE_NOOP("PauseInstance::units", "Milliseconds")
			<< QT_TRANSLATE_NOOP("PauseInstance::units", "Seconds")
			<< QT_TRANSLATE_NOOP("PauseInstance::units", "Minutes")
			<< QT_TRANSLATE_NOOP("PauseInstance::units", "Hours")
			<< QT_TRANSLATE_NOOP("PauseInstance::units", "Days"));
}


