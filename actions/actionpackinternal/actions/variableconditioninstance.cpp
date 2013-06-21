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

#include "variableconditioninstance.h"

namespace Actions
{
	ActionTools::StringListPair VariableConditionInstance::comparisons = qMakePair(
			QStringList() << "equal" << "different" << "inferior" << "superior" << "inferiorEqual" << "superiorEqual" << "contains",
			QStringList()
			<< QT_TRANSLATE_NOOP("VariableConditionInstance::comparisons", "Equal (=)")
			<< QT_TRANSLATE_NOOP("VariableConditionInstance::comparisons", "Different (!=)")
			<< QT_TRANSLATE_NOOP("VariableConditionInstance::comparisons", "Inferior (<)")
			<< QT_TRANSLATE_NOOP("VariableConditionInstance::comparisons", "Superior (>)")
			<< QT_TRANSLATE_NOOP("VariableConditionInstance::comparisons", "Inferior or equal (<=)")
			<< QT_TRANSLATE_NOOP("VariableConditionInstance::comparisons", "Superior or equal (>=)")
			<< QT_TRANSLATE_NOOP("VariableConditionInstance::comparisons", "Contains"));
}
