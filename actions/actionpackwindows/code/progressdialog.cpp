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

#include "progressdialog.h"

#include <QScriptValueIterator>
#include <QProgressDialog>

namespace Code
{
	QScriptValue ProgressDialog::constructor(QScriptContext *context, QScriptEngine *engine)
	{
		ProgressDialog *progressDialog = new ProgressDialog;
		progressDialog->setupConstructorParameters(context, engine, context->argument(0));

		QScriptValueIterator it(context->argument(0));

		while(it.hasNext())
		{
			it.next();

			if(it.name() == "value")
				progressDialog->mProgressDialog->setValue(it.value().toInt32());
			else if(it.name() == "labelText")
				progressDialog->mProgressDialog->setLabelText(it.value().toString());
			else if(it.name() == "minimum")
				progressDialog->mProgressDialog->setMinimum(it.value().toInt32());
			else if(it.name() == "maximum")
				progressDialog->mProgressDialog->setMaximum(it.value().toInt32());
			else if(it.name() == "range")
			{
				progressDialog->mProgressDialog->setMinimum(it.value().property("minimum").toInt32());
				progressDialog->mProgressDialog->setMaximum(it.value().property("maximum").toInt32());
			}
			else if(it.name() == "onCanceled")
				progressDialog->mOnCanceled = it.value();
		}

		return CodeClass::constructor(progressDialog, context, engine);
	}
	
	ProgressDialog::ProgressDialog()
		: BaseWindow(),
		mProgressDialog(new QProgressDialog)
	{
		setWidget(mProgressDialog);

		connect(mProgressDialog, SIGNAL(canceled()), this, SLOT(canceled()));
	}
	
	ProgressDialog::~ProgressDialog()
	{
		delete mProgressDialog;
	}

	int ProgressDialog::value() const
	{
		return mProgressDialog->value();
	}
	
	QScriptValue ProgressDialog::setValue(int value)
	{
		mProgressDialog->setValue(value);

		return thisObject();
	}
	
	QScriptValue ProgressDialog::setLabelText(const QString &labelText)
	{
		mProgressDialog->setLabelText(labelText);
		
		return thisObject();
	}
	
	QScriptValue ProgressDialog::setMinimum(int minimum)
	{
		mProgressDialog->setMinimum(minimum);
		
		return thisObject();
	}

	QScriptValue ProgressDialog::setMaximum(int maximum)
	{
		mProgressDialog->setMaximum(maximum);
		
		return thisObject();
	}

	QScriptValue ProgressDialog::setRange(int minimum, int maximum)
	{
		mProgressDialog->setMinimum(minimum);
		mProgressDialog->setMaximum(maximum);
		
		return thisObject();
	}
	
	QScriptValue ProgressDialog::show()
	{
		mProgressDialog->open();

		return thisObject();
	}

	int ProgressDialog::showModal()
	{
		return mProgressDialog->exec();
	}
	
	void ProgressDialog::canceled()
	{
		if(mProgressDialog->isVisible() && mOnCanceled.isValid())
			mOnCanceled.call(thisObject());
	}
}
