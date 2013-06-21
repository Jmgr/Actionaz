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

#ifndef PIXELCOLORINSTANCE_H
#define PIXELCOLORINSTANCE_H

#include "actioninstance.h"
#include "script.h"
#include "ifactionvalue.h"
#include "code/color.h"

#include <QPoint>
#include <QPixmap>
#include <QApplication>
#include <QDesktopWidget>
#include <QTimer>

namespace Actions
{
	class PixelColorInstance : public ActionTools::ActionInstance
	{
		Q_OBJECT

	public:
		enum Comparison
		{
			Equal,
			Darker,
			Lighter
		};

		PixelColorInstance(const ActionTools::ActionDefinition *definition, QObject *parent = 0)
			: ActionTools::ActionInstance(definition, parent), mComparison(Equal) {}

		static ActionTools::StringListPair comparisons;

		void startExecution()
		{
			bool ok = true;

			mPixelPosition = evaluatePoint(ok, "pixel", "position");
			mPixelColorValue = evaluateColor(ok, "pixel", "color");
			mComparison = evaluateListElement<Comparison>(ok, comparisons, "comparison");
			mIfTrue = evaluateIfAction(ok, "ifTrue");
			ActionTools::IfActionValue ifFalse = evaluateIfAction(ok, "ifFalse");
			mVariable = evaluateVariable(ok, "variable");
			int redTolerance = evaluateInteger(ok, "redTolerance");
			int greenTolerance = evaluateInteger(ok, "greenTolerance");
			int blueTolerance = evaluateInteger(ok, "blueTolerance");

			if(!ok)
				return;

			redTolerance = (255 * redTolerance) / 100;
			greenTolerance = (255 * greenTolerance) / 100;
			blueTolerance = (255 * blueTolerance) / 100;

			mMinimumColor = QColor(normalizeColor(mPixelColorValue.red() - redTolerance),
								   normalizeColor(mPixelColorValue.green() - greenTolerance),
								   normalizeColor(mPixelColorValue.blue() - blueTolerance));
			mMaximumColor = QColor(normalizeColor(mPixelColorValue.red() + redTolerance),
								   normalizeColor(mPixelColorValue.green() + greenTolerance),
								   normalizeColor(mPixelColorValue.blue() + blueTolerance));

			if(testPixel())
			{
				setCurrentParameter("ifTrue", "line");

				QString line = evaluateSubParameter(ok, mIfTrue.actionParameter());
				if(!ok)
					return;

				if(mIfTrue.action() == ActionTools::IfActionValue::GOTO)
					setNextLine(line);
				else if(mIfTrue.action() == ActionTools::IfActionValue::CALLPROCEDURE)
				{
					if(!callProcedure(line))
						return;
				}

				emit executionEnded();
			}
			else
			{
				setCurrentParameter("ifFalse", "line");

				QString line = evaluateSubParameter(ok, ifFalse.actionParameter());
				if(!ok)
					return;

				if(ifFalse.action() == ActionTools::IfActionValue::GOTO)
				{
					setNextLine(line);

					emit executionEnded();
				}
				else if(ifFalse.action() == ActionTools::IfActionValue::CALLPROCEDURE)
				{
					if(!callProcedure(line))
						return;

					emit executionEnded();
				}
				else if(ifFalse.action() == ActionTools::IfActionValue::WAIT)
				{
					connect(&mTimer, SIGNAL(timeout()), this, SLOT(checkPixel()));
					mTimer.setInterval(100);
					mTimer.start();
				}
				else
					emit executionEnded();
			}
		}

		void stopExecution()
		{
			mTimer.stop();
		}

	private slots:
		void checkPixel()
		{
			if(testPixel())
			{
				bool ok = true;

				QString line = evaluateSubParameter(ok, mIfTrue.actionParameter());
				if(!ok)
					return;

				if(mIfTrue.action() == ActionTools::IfActionValue::GOTO)
					setNextLine(line);
				else if(mIfTrue.action() == ActionTools::IfActionValue::CALLPROCEDURE)
				{
					if(!callProcedure(line))
						return;
				}

				mTimer.stop();
				emit executionEnded();
			}
		}

	private:
		QPoint mPixelPosition;
		QColor mPixelColorValue;
		Comparison mComparison;
		ActionTools::IfActionValue mIfTrue;
		QString mVariable;
		QTimer mTimer;
		QColor mMinimumColor;
		QColor mMaximumColor;

		bool testPixel()
		{
			QPixmap pixel = QPixmap::grabWindow(QApplication::desktop()->winId(), mPixelPosition.x(), mPixelPosition.y(), 1, 1);
			QColor pixelColor = pixel.toImage().pixel(0, 0);

            setVariable(mVariable, Code::Color::constructor(pixelColor, scriptEngine()));

			switch(mComparison)
			{
			case Equal:
				return (pixelColor.red() >= mMinimumColor.red() && pixelColor.red() <= mMaximumColor.red() &&
						pixelColor.green() >= mMinimumColor.green() && pixelColor.green() <= mMaximumColor.green() &&
						pixelColor.blue() >= mMinimumColor.blue() && pixelColor.blue() <= mMaximumColor.blue());
			case Darker:
				return (pixelColor.lightness() < mPixelColorValue.lightness());
			case Lighter:
				return (pixelColor.lightness() > mPixelColorValue.lightness());
			}

			return false;
		}

		static int normalizeColor(int value)
		{
			if(value < 0)
				value = 0;
			if(value > 255)
				value = 255;

			return value;
		}

		Q_DISABLE_COPY(PixelColorInstance)
	};
}

#endif // PIXELCOLORINSTANCE_H
