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

#include "codeinitializer.h"
#include "executer.h"
#include "code/codetools.h"
#include "code/codeclass.h"
#include "code/image.h"
#include "code/rawdata.h"
#include "code/algorithms.h"
#include "code/color.h"
#include "code/point.h"
#include "code/size.h"
#include "code/rect.h"
#include "code/processhandle.h"
#include "code/window.h"
#include "actionpack.h"
#include "actionfactory.h"
#include "codeexecution.h"
#include "codestdio.h"

#include <QScriptEngine>
#include <QFile>
#include <QUiLoader>

namespace LibExecuter
{
	static QScriptValue loadUIFunction(QScriptContext *context, QScriptEngine *engine)
	{
		QString filename = context->argument(0).toString();
		QFile file(filename);
		if(!file.open(QIODevice::ReadOnly))
		{
			Code::CodeClass::throwError(context, engine, "LoadFileError", QObject::tr("Unable to load UI file %1").arg(filename));
			return context->thisObject();
		}

		return engine->newQObject(QUiLoader().load(&file), QScriptEngine::ScriptOwnership);
	}

	static QScriptValue includeFunction(QScriptContext *context, QScriptEngine *engine)
	{
		QString filename = context->argument(0).toString();
		QFile file(filename);
		if(!file.open(QIODevice::ReadOnly))
		{
			Code::CodeClass::throwError(context, engine, "IncludeFileError", QObject::tr("Unable to include file %1").arg(filename));
			return context->thisObject();
		}

		QString fileContent = file.readAll();
		file.close();

		QScriptContext *parent = context->parentContext();
		if(parent)
		{
			context->setActivationObject(parent->activationObject());
			context->setThisObject(parent->thisObject());
		}

		return engine->evaluate(fileContent, filename);
	}

	void CodeInitializer::initialize(QScriptEngine *scriptEngine, ScriptAgent *scriptAgent, ActionTools::ActionFactory *actionFactory)
	{
		scriptEngine->setProcessEventsInterval(50);

		QScriptValue loadUIFunc = scriptEngine->newFunction(&loadUIFunction);
		scriptEngine->globalObject().setProperty("loadUI", loadUIFunc);

		QScriptValue includeFunc = scriptEngine->newFunction(&includeFunction);
		scriptEngine->globalObject().setProperty("include", includeFunc);

		Code::Window::registerClass(scriptEngine);
		Code::RawData::registerClass(scriptEngine);
		Code::Image::registerClass(scriptEngine);
		Code::Algorithms::registerClass(scriptEngine);
		Code::Color::registerClass(scriptEngine);
		Code::Point::registerClass(scriptEngine);
		Code::Size::registerClass(scriptEngine);
		Code::Rect::registerClass(scriptEngine);
		Code::ProcessHandle::registerClass(scriptEngine);

		CodeExecution::setScriptAgent(scriptAgent);
		Code::CodeTools::addClassToScriptEngine<CodeExecution>("Execution", scriptEngine);
		Code::CodeTools::addClassGlobalFunctionToScriptEngine("Execution", &CodeExecution::pause, "pause", scriptEngine);
		Code::CodeTools::addClassGlobalFunctionToScriptEngine("Execution", &CodeExecution::sleep, "sleep", scriptEngine);
		Code::CodeTools::addClassGlobalFunctionToScriptEngine("Execution", &CodeExecution::stop, "stop", scriptEngine);

		Code::CodeTools::addClassToScriptEngine<CodeStdio>("Stdio", scriptEngine);
		Code::CodeTools::addClassGlobalFunctionToScriptEngine("Stdio", &CodeStdio::print, "print", scriptEngine);
		Code::CodeTools::addClassGlobalFunctionToScriptEngine("Stdio", &CodeStdio::println, "println", scriptEngine);
		Code::CodeTools::addClassGlobalFunctionToScriptEngine("Stdio", &CodeStdio::printWarning, "printWarning", scriptEngine);
		Code::CodeTools::addClassGlobalFunctionToScriptEngine("Stdio", &CodeStdio::printlnWarning, "printlnWarning", scriptEngine);
		Code::CodeTools::addClassGlobalFunctionToScriptEngine("Stdio", &CodeStdio::printError, "printError", scriptEngine);
		Code::CodeTools::addClassGlobalFunctionToScriptEngine("Stdio", &CodeStdio::printlnError, "printlnError", scriptEngine);

		int actionPackCount = actionFactory->actionPackCount();
		for(int actionPackIndex = 0; actionPackIndex < actionPackCount; ++actionPackIndex)
		{
			ActionTools::ActionPack *actionPack = actionFactory->actionPack(actionPackIndex);

			actionPack->codeInit(scriptEngine);
		}
	}
}
