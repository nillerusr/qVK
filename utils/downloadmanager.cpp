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

bool DownloadManager::FileAlreadyExists(const QString filename, QWidget *widget )
{
	QFile file(filename);
	if( file.exists() )
	{
		emit downloaded(filename, {widget}, 0);
		return true;
	}
	return false;
}


void DownloadManager::append(const QString url, const QString filename, QWidget *widget)
{
	if( FileAlreadyExists(download_dir+"/"+filename, widget) || queueExists(filename, widget) )
		return;

	downloadQueue.enqueue({QUrl::fromEncoded(url.toLocal8Bit()), filename, {widget}});
}

bool DownloadManager::queueExists(const QString filename, QWidget *widget)
{
	for( int i = 0; i < downloadQueue.length(); i++ )
	{
		if( downloadQueue[i].filename == filename )
		{
			downloadQueue[i].widgets.append( widget );
			return true;
		}
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

	for( QWidget *widget : current_item.widgets )
		emit downloaded( output.fileName(), widget, 0);

	startNextDownload();
}

void DownloadManager::downloadReadyRead()
{
	output.write(currentDownload->readAll());
}
