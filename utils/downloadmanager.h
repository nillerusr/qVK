#ifndef DOWNLOADMANAGER_H
#define DOWNLOADMANAGER_H

#include <QFile>
#include <QObject>
#include <QQueue>
#include <QTime>
#include <QUrl>
#include <QNetworkAccessManager>
#include <QNetworkReply>


struct download_info
{
	download_info( void *data )
	{
		this->bDeleted = false;
		this->data = data;
	}	
	
	bool bDeleted;
	void *data;
};

struct download_queue_params
{
	QUrl url;
	QString filename;
	QList<download_info> data;
};


class DownloadManager: public QObject
{
    Q_OBJECT
public:
    DownloadManager(QObject *parent = 0);

    void append(const QString url, const QString filename, download_info info);
	void setDownloadDirectory( QString downdir );
	void download();
	bool queueExists(const QString filename, download_info info);
	bool FileAlreadyExists( const QString filename, download_info info);
	void freeDataInQueues();
	
private slots:
    void startNextDownload();
    void downloadFinished();
    void downloadReadyRead();

signals:
	void downloaded( QString filename, download_info info, int error );

private:
    QNetworkAccessManager manager;
    QQueue<download_queue_params> downloadQueue;
    QNetworkReply *currentDownload;
	download_queue_params current_item;
	
    QFile output;
	QString download_dir;
	bool bDownloading;
};

#endif // DOWNLOADMANAGER_H
