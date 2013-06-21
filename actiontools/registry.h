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

#ifndef AT_REGISTRY_H
#define AT_REGISTRY_H

#include "actiontools_global.h"

#include <QVariant>

#ifdef Q_WS_WIN
#include <Windows.h>
#endif

namespace ActionTools
{
	class ACTIONTOOLSSHARED_EXPORT Registry
	{
	public:
		enum Key
		{
			ClassesRoot,
			CurrentConfig,
			CurrentUser,
			Users,
			LocalMachine,

			KeyCount
		};
		enum ReadResult
		{
			ReadOk,
			ReadCannotFindSubKey,
			ReadCannotFindValue,
			ReadInvalidValueType
		};
		enum WriteResult
		{
			WriteOk,
			WriteCannotFindSubKey,
			WriteCannotWriteValue
		};

#ifdef Q_WS_WIN
		static ReadResult read(QVariant &result, Key key, const QString &subkey, const QString &value = QString());
		static WriteResult write(const QVariant &data, Key key, const QString &subkey, const QString &value = QString());
#endif

	private:
		Registry();

#ifdef Q_WS_WIN
		static HKEY enumToKey(Key key);
#endif
	};
}

#endif // AT_REGISTRY_H
