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
// This file uses some code lines from the Ofi Labs X2 project
// Copyright (C) 2010 Ariya Hidayat <ariya.hidayat@gmail.com>
// Licensed under GNU/GPLv3

#include "codehighlighter.h"
#include "keywords.h"

#include <QFile>
#include <QDebug>

namespace ActionTools
{
	CodeHighlighter::CodeHighlighter(QTextDocument *parent)
		: QSyntaxHighlighter(parent)
	{
		mFormats[KeywordFormat].setForeground(Qt::darkBlue);
		mFormats[KeywordFormat].setFontWeight(QFont::Bold);

		mFormats[ReservedFormat].setForeground(Qt::red);
		mFormats[ReservedFormat].setFontWeight(QFont::Bold);
		mFormats[ReservedFormat].setFontStrikeOut(true);

		mFormats[CodeObjectsFormat].setForeground(Qt::darkBlue);
		mFormats[CodeObjectsFormat].setFontWeight(QFont::Bold);

		mFormats[OperatorFormat].setForeground(Qt::red);

		mFormats[NumberFormat].setForeground(Qt::darkMagenta);

		mFormats[CommentFormat].setForeground(Qt::darkGreen);

		mFormats[StringFormat].setForeground(Qt::darkRed);

		foreach(const QString &keyword, usedKeywords)
			mUsedKeywords.insert(keyword);

		foreach(const QString &keyword, reservedKeywords)
			mReservedKeywords.insert(keyword);
	}

	void CodeHighlighter::highlightBlock(const QString &text)
	{
		// parsing state
		enum
		{
			Start = 0,
			Number = 1,
			Identifier = 2,
			String = 3,
			Comment = 4,
			Regex = 5
		};

		QList<int> bracketPositions;

		int blockState = previousBlockState();
		int bracketLevel = blockState >> 4;
		int state = blockState & 15;
		if (blockState < 0)
		{
			bracketLevel = 0;
			state = Start;
		}

		int start = 0;
		int i = 0;
		while (i <= text.length())
		{
			QChar ch = (i < text.length()) ? text.at(i) : QChar();
			QChar next = (i < text.length() - 1) ? text.at(i + 1) : QChar();

			switch (state)
			{

			case Start:
				start = i;
				if (ch.isSpace())
				{
					++i;
				}
				else if (ch.isDigit())
				{
					++i;
					state = Number;
				}
				else if (ch.isLetter() || ch == '_')
				{
					++i;
					state = Identifier;
				}
				else if (ch == '\'' || ch == '\"')
				{
					++i;
					state = String;
				}
				else if (ch == '/' && next == '*')
				{
					++i;
					++i;
					state = Comment;
				}
				else if (ch == '/' && next == '/')
				{
					i = text.length();
					setFormat(start, text.length(), mFormats[CommentFormat]);
				}
				else if (ch == '/' && next != '*')
				{
					++i;
					state = Regex;
				}
				else
				{
					if (!QString("(){}[]").contains(ch))
						setFormat(start, 1, mFormats[OperatorFormat]);
					if (ch =='{' || ch == '}')
					{
						bracketPositions += i;
						if (ch == '{')
							bracketLevel++;
						else
							bracketLevel--;
					}
					++i;
					state = Start;
				}
				break;

			case Number:
				if (ch.isSpace() || !ch.isDigit())
				{
					setFormat(start, i - start, mFormats[NumberFormat]);
					state = Start;
				}
				else
					++i;
				break;

			case Identifier:
				if (ch.isSpace() || !(ch.isDigit() || ch.isLetter() || ch == '_'))
				{
					QString token = text.mid(start, i - start).trimmed();
					if (mUsedKeywords.contains(token))
						setFormat(start, i - start, mFormats[KeywordFormat]);
					else if (mReservedKeywords.contains(token))
						setFormat(start, i - start, mFormats[ReservedFormat]);
					else if (mCodeObjects.contains(token))
						setFormat(start, i - start, mFormats[CodeObjectsFormat]);
					state = Start;
				}
				else
					++i;
				break;

			case String:
				if (ch == text.at(start))
				{
					QChar prev = (i > 0) ? text.at(i - 1) : QChar();
					if (prev != '\\')
					{
						++i;
						setFormat(start, i - start, mFormats[StringFormat]);
						state = Start;
					}
					else
						++i;
				}
				else
					++i;
				break;

			case Comment:
				if (ch == '*' && next == '/')
				{
					++i;
					++i;
					setFormat(start, i - start, mFormats[CommentFormat]);
					state = Start;
				}
				else
					++i;
				break;

			case Regex:
				if (ch == '/')
				{
					QChar prev = (i > 0) ? text.at(i - 1) : QChar();
					if (prev != '\\')
					{
						++i;
						setFormat(start, i - start, mFormats[StringFormat]);
						state = Start;
					}
					else
						++i;
				}
				else
					++i;
				break;

			default:
				state = Start;
				break;
			}
		}

		if (state == Comment)
			setFormat(start, text.length(), mFormats[CommentFormat]);
		else
			state = Start;
	}

	void CodeHighlighter::addCodeObject(const QString &name)
	{
		mCodeObjects.insert(name);
	}
}
