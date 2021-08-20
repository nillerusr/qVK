#include "downloadmanager.h"
#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QNetworkReply>

// TODO: multithreading downloading

DownloadManager::DownloadManager(QObject *parent) : QObject(parent)
{
	download_dir = ".";
	bDownloading = false;
}

bool DownloadManager::FileAlreadyExists(const QString filename)
{
	QFile file(filename);
	if( file.exists() )
	{
		emit downloaded(filename, 0);
		return true;
	}
	return false;
}

void DownloadManager::append(QList<QPair<QString, QString>> urls)
{
	QList<QPair<QString, QString>>::iterator s;

	for( s = urls.begin(); s != urls.end(); ++s )
	{
		if( FileAlreadyExists( download_dir+"/"+(*s).second ) || queueExists((*s).second) )
			continue;
	
		downloadQueue.enqueue({QUrl::fromEncoded((*s).first.toLocal8Bit()), (*s).second});
	}
}

void DownloadManager::append(const QString url, const QString filename)
{
	if( FileAlreadyExists(download_dir+"/"+filename) || queueExists(filename) )
		return;

	downloadQueue.enqueue({QUrl::fromEncoded(url.toLocal8Bit()), filename});
}

bool DownloadManager::queueExists(const QString filename)
{
	for( int i = 0; i < downloadQueue.length(); i++ )
	{
		if( downloadQueue[i].second == filename )
			return true;
	}
	return false;
}

void DownloadManager::setDownloadDirectory( QString downdir )
{
	download_dir = downdir;
	QDir dir(downdir);
	if (!dir.exists())
		dir.mkpath(".");
}

void DownloadManager::download()
{
	if( !bDownloading )
	{
		bDownloading = true;
		startNextDownload();
	}
}

void DownloadManager::startNextDownload()
{
	if (downloadQueue.isEmpty())
	{
		bDownloading = false;
		return;
	}

	QPair<QUrl, QString> item = downloadQueue.dequeue();

	output.setFileName(download_dir+"/"+item.second);
	if (!output.open(QIODevice::WriteOnly))
	{
		startNextDownload();
		return;
	}

	QNetworkRequest request(item.first);
	currentDownload = manager.get(request);

	connect(currentDownload, SIGNAL(finished()),
		SLOT(downloadFinished()));
	connect(currentDownload, SIGNAL(readyRead()),
		SLOT(downloadReadyRead()));
}

void DownloadManager::downloadFinished()
{
	output.close();	

	if (currentDownload->error())
		emit downloaded( output.fileName(), 1);
	else
		emit downloaded( output.fileName(), 0);

	startNextDownload();
}

void DownloadManager::downloadReadyRead()
{
	output.write(currentDownload->readAll());
}
