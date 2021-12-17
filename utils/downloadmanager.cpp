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

bool DownloadManager::FileAlreadyExists(const QString filename, download_info info )
{
	QFile file(filename);
	if( file.exists() )
	{
		emit downloaded(filename, info, 0);
		return true;
	}
	return false;
}

void DownloadManager::append(const QString url, const QString filename, download_info info)
{
	if( FileAlreadyExists(download_dir+"/"+filename, info) || queueExists(filename, info) )
		return;

	downloadQueue.enqueue({QUrl::fromEncoded(url.toLocal8Bit()), filename, {info}});
}

bool DownloadManager::queueExists(const QString filename, download_info info)
{
	for( int i = 0; i < downloadQueue.length(); i++ )
	{
		if( downloadQueue[i].filename == filename )
		{
			downloadQueue[i].data.append( info );
			return true;
		}
	}
	return false;
}

void DownloadManager::freeDataInQueues()
{
	for( int i = 0; i < downloadQueue.length(); i++ )
		for( int j = 0; j < downloadQueue[i].data.length(); j++)
			downloadQueue[i].data[j].bDeleted = true;
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

	current_item = downloadQueue.dequeue();

	output.setFileName(download_dir+"/"+current_item.filename);
	if (!output.open(QIODevice::WriteOnly))
	{
		startNextDownload();
		return;
	}

	QNetworkRequest request(current_item.url);
	currentDownload = manager.get(request);

	connect(currentDownload, SIGNAL(finished()),
		SLOT(downloadFinished()));
	connect(currentDownload, SIGNAL(readyRead()),
		SLOT(downloadReadyRead()));
}

void DownloadManager::downloadFinished()
{
	output.close();	

	for( download_info info : current_item.data )
		emit downloaded( output.fileName(), info, 0);

	startNextDownload();
}

void DownloadManager::downloadReadyRead()
{
	output.write(currentDownload->readAll());
}
