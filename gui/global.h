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

#ifndef GLOBAL_H
#define GLOBAL_H

#include "version.h"

namespace Global
{
	const Tools::Version	ACTIONAZ_VERSION =	Tools::Version(VERSION_TO_STRING(ACT_VERSION));
	const Tools::Version	SCRIPT_VERSION =	Tools::Version(VERSION_TO_STRING(ACT_SCRIPT_VERSION));
	const QString			UPDATE_URL =		"http://updates.actionaz.org/";
	const int				UPDATE_TIMEOUT =	5000;
	const QString			CONNECTIVITY_URL =	"http://www.actionaz.org/";

	QString currentOS();
	QString currentOSType();
	int currentOSBits();
}

#endif // GLOBAL_H
