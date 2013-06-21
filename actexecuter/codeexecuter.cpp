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

#include "codeexecuter.h"
#include "executer/codeinitializer.h"
#include "executer/scriptagent.h"
#include "actionfactory.h"
#include "actionpack.h"
#include "executer/codestdio.h"
#include "code/codetools.h"
#include "executer/codeactionaz.h"
#include "global.h"

#include <QFile>
#include <QScriptEngine>
#include <QScriptEngineDebugger>
#include <QMainWindow>
#include <QTimer>
#include <QTextStream>
#include <QStringList>
#include <QApplication>
#include <QSettings>
#include <QTranslator>

CodeExecuter::CodeExecuter(QObject *parent) :
    Executer(parent),
	mScriptEngine(new QScriptEngine(this)),
	mScriptAgent(new LibExecuter::ScriptAgent(mScriptEngine)),
	mScriptEngineDebugger(new QScriptEngineDebugger(this)),
	mDebuggerWindow(mScriptEngineDebugger->standardWindow())
{
	connect(mScriptEngineDebugger, SIGNAL(evaluationResumed()), this, SLOT(onEvaluationResumed()));
	connect(mScriptEngineDebugger, SIGNAL(evaluationSuspended()), this, SLOT(onEvaluationPaused()));
	connect(mScriptAgent, SIGNAL(executionStopped()), this, SLOT(stopExecution()));

	foreach(QString extension, mScriptEngine->availableExtensions())
		mScriptEngine->importExtension(extension);
	
	mScriptEngineDebugger->setAutoShowStandardWindow(false);
	mScriptEngineDebugger->attachTo(mScriptEngine);
	QScriptEngineAgent *debuggerAgent = mScriptEngine->agent();
	mScriptEngine->setAgent(mScriptAgent);
	mScriptAgent->setDebuggerAgent(debuggerAgent);
}

bool CodeExecuter::start(QIODevice *device, const QString &filename)
{
	if(!Executer::start(device, filename))
		return false;
	
	QString code = device->readAll();
	device->close();
	
	mScriptAgent->setContext(LibExecuter::ScriptAgent::ActionInit);
	LibExecuter::CodeInitializer::initialize(mScriptEngine, mScriptAgent, actionFactory());

	Code::CodeTools::addClassToScriptEngine<LibExecuter::CodeStdio>("Console", mScriptEngine);
	Code::CodeTools::addClassGlobalFunctionToScriptEngine("Console", &LibExecuter::CodeStdio::print, "print", mScriptEngine);
	Code::CodeTools::addClassGlobalFunctionToScriptEngine("Console", &LibExecuter::CodeStdio::println, "println", mScriptEngine);
	Code::CodeTools::addClassGlobalFunctionToScriptEngine("Console", &LibExecuter::CodeStdio::printWarning, "printWarning", mScriptEngine);
	Code::CodeTools::addClassGlobalFunctionToScriptEngine("Console", &LibExecuter::CodeStdio::printlnWarning, "printlnWarning", mScriptEngine);
	Code::CodeTools::addClassGlobalFunctionToScriptEngine("Console", &LibExecuter::CodeStdio::printError, "printError", mScriptEngine);
	Code::CodeTools::addClassGlobalFunctionToScriptEngine("Console", &LibExecuter::CodeStdio::printlnError, "printlnError", mScriptEngine);

	Code::CodeTools::addClassToScriptEngine<LibExecuter::CodeActionaz>("Actionaz", mScriptEngine);
	LibExecuter::CodeActionaz::setActExec(true);
	LibExecuter::CodeActionaz::setActionazVersion(Global::ACTIONAZ_VERSION);
	LibExecuter::CodeActionaz::setScriptVersion(Global::SCRIPT_VERSION);
	Code::CodeTools::addClassGlobalFunctionToScriptEngine("Actionaz", &LibExecuter::CodeActionaz::version, "version", mScriptEngine);
	Code::CodeTools::addClassGlobalFunctionToScriptEngine("Actionaz", &LibExecuter::CodeActionaz::scriptVersion, "scriptVersion", mScriptEngine);
	Code::CodeTools::addClassGlobalFunctionToScriptEngine("Actionaz", &LibExecuter::CodeActionaz::isActExec, "isActExec", mScriptEngine);
	Code::CodeTools::addClassGlobalFunctionToScriptEngine("Actionaz", &LibExecuter::CodeActionaz::isActionaz, "isActionaz", mScriptEngine);

	QSettings settings;
	QString locale = settings.value("locale").toString();

	if(locale.isEmpty())
	{
		locale = QLocale::system().name();

#ifdef Q_WS_WIN
		QString installerLanguage = settings.value("installerLanguage").toString();
		if(!installerLanguage.isEmpty())
		{
			if(installerLanguage == "english")
				locale = "en_US";
			else if(installerLanguage == "french")
				locale = "fr_FR";
		}
#endif
	}

	for(int actionPackIndex = 0; actionPackIndex < actionFactory()->actionPackCount(); ++actionPackIndex)
	{
		ActionTools::ActionPack *actionPack = actionFactory()->actionPack(actionPackIndex);

		QTranslator *actionTranslator = new QTranslator(this);
		actionTranslator->load(QString("%1/locale/actionpack%2_%3").arg(QApplication::applicationDirPath()).arg(actionPack->id()).arg(locale));
		QApplication::installTranslator(actionTranslator);
	}

	mScriptAgent->setContext(LibExecuter::ScriptAgent::Parameters);
	
	QScriptValue result = mScriptEngine->evaluate(code, filename);
	if(result.isError())
	{
		QTextStream stream(stdout);
		stream << QObject::tr("Uncaught exception: ") << result.toString() << "\n";
		stream << tr("Backtrace: ") << mScriptEngine->uncaughtExceptionBacktrace().join("\n") << "\n";
		stream.flush();
	}
	
	QTimer::singleShot(0, qApp, SLOT(quit()));
	
	return true;
}

void CodeExecuter::onEvaluationResumed()
{
	mDebuggerWindow->hide();
}

void CodeExecuter::onEvaluationPaused()
{
	mDebuggerWindow->show();
}

void CodeExecuter::stopExecution()
{
	mScriptEngine->abortEvaluation();
}
