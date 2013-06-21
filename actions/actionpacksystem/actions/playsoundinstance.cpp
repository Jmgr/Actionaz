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

#include <QtGlobal>

#include "playsoundinstance.h"

namespace Actions
{
	PlaySoundInstance::PlaySoundInstance(const ActionTools::ActionDefinition *definition, QObject *parent)
		: ActionTools::ActionInstance(definition, parent),
		  mMediaPlayer(new QMediaPlayer(this)),
		  mMediaPlaylist(new QMediaPlaylist(this)),
		  mBlocking(false)
	{
		mMediaPlayer->setPlaylist(mMediaPlaylist);

		connect(mMediaPlayer, SIGNAL(stateChanged(QMediaPlayer::State)), this, SLOT(stateChanged(QMediaPlayer::State)));
	}

	PlaySoundInstance::~PlaySoundInstance()
	{
	}

	void PlaySoundInstance::startExecution()
	{
		bool ok = true;

		QString file = evaluateString(ok, "file");
		int volume = evaluateInteger(ok, "volume");
		mBlocking = evaluateBoolean(ok, "blocking");
		bool looping = evaluateBoolean(ok, "looping");
		bool isUrl = evaluateBoolean(ok, "url");
		int playbackRate = evaluateInteger(ok, "playbackRate");

		if(!ok)
			return;

		mMediaPlaylist->clear();

		QUrl url;

		if(isUrl)
			url = QUrl(file);
		else
			url = QUrl::fromLocalFile(file);

		if(!mMediaPlaylist->addMedia(url))
		{
			emit executionException(ActionTools::ActionException::BadParameterException, tr("Unable to load file %1: %2").arg(file).arg(mMediaPlayer->errorString()));
			return;
		}

		mMediaPlaylist->setPlaybackMode(looping ? QMediaPlaylist::Loop : QMediaPlaylist::CurrentItemOnce);

		mMediaPlayer->setPlaybackRate(playbackRate / 100.0f);
		mMediaPlayer->setVolume(volume);
		mMediaPlayer->play();

		if(mMediaPlayer->error() != QMediaPlayer::NoError)
		{
			emit executionException(ActionTools::ActionException::BadParameterException, tr("Unable to play file %1: %2").arg(file).arg(mMediaPlayer->errorString()));
			return;
		}

		if(!mBlocking)
			emit executionEnded();
	}

	void PlaySoundInstance::pauseExecution()
	{
		mMediaPlayer->pause();
	}

	void PlaySoundInstance::resumeExecution()
	{
		mMediaPlayer->play();
	}

	void PlaySoundInstance::stopExecution()
	{
		mMediaPlayer->stop();
	}

	void PlaySoundInstance::stopLongTermExecution()
	{
		mMediaPlayer->stop();
	}

	void PlaySoundInstance::stateChanged(QMediaPlayer::State state)
	{
		if(state == QMediaPlayer::StoppedState)
			emit executionEnded();
	}
}
