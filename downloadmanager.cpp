#include "downloadmanager.h"
#include <QCoreApplication>
#include <QDir>
#include <QNetworkReply>

// TODO: multithreading downloading

DownloadManager::DownloadManager(QObject *parent) : QObject(parent)
{
	download_dir = ".";
	bDownloading = false;
}

void DownloadManager::append(QList<QPair<QString, QString>> urls)
{
	QList<QPair<QString, QString>>::iterator s;

	for( s = urls.begin(); s != urls.end(); ++s )
		downloadQueue.enqueue({QUrl::fromEncoded((*s).first.toLocal8Bit()), (*s).second});
}

void DownloadManager::append(const QString url, const QString filename)
{
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

	if( item.second == "PlLXnNg6tuY" )
	{
		qDebug() << item.first;
	}
	
	output.setFileName(download_dir+"/"+item.second);
    if (!output.open(QIODevice::WriteOnly)) {
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
