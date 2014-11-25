/*
    Actiona
    Copyright (C) 2008-2014 Jonathan Mercier-Ganady

    Actiona is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Actiona is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program. If not, see <http://www.gnu.org/licenses/>.

    Contact : jmgr@jmgr.info
*/

#include "mailattachment.h"
#include "code/rawdata.h"

#include <QScriptValueIterator>

namespace Code
{
    QScriptValue MailAttachment::constructor(QScriptContext *context, QScriptEngine *engine)
    {
        MailAttachment *mailAttachment = new MailAttachment;

        QScriptValueIterator it(context->argument(0));

        while(it.hasNext())
        {
            it.next();

            if(it.name() == "contentType")
                mailAttachment->setContentType(it.value().toString());
            else if(it.name() == "content")
                mailAttachment->setContent(it.value());
        }

        return CodeClass::constructor(mailAttachment, context, engine);
    }

    QScriptValue MailAttachment::constructor(const QxtMailAttachment &attachment, QScriptEngine* engine)
    {
        return CodeClass::constructor(new MailAttachment(attachment), engine);
    }

    MailAttachment::MailAttachment()
        : CodeClass()
    {
    }

    MailAttachment::MailAttachment(const QxtMailAttachment& attachment)
        : CodeClass(),
          mAttachment(attachment)
    {
    }

    QScriptValue MailAttachment::content() const
    {
        return RawData::constructor(mContent, engine());
    }

    QScriptValue MailAttachment::setContent(const QScriptValue &content)
    {
        if(RawData *rawData = qobject_cast<RawData*>(content.toQObject()))
            mContent = rawData->byteArray();
        else
            mContent = content.toVariant().toByteArray();

        mAttachment.setContent(mContent);

        return thisObject();
    }
}
