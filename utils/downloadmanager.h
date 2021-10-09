#ifndef DOWNLOADMANAGER_H
#define DOWNLOADMANAGER_H

#include <QFile>
#include <QObject>
#include <QQueue>
#include <QTime>
#include <QUrl>
#include <QNetworkAccessManager>
#include <QNetworkReply>

struct download_queue_params
{
	QUrl url;
	QString filename;
	QList<QWidget*> widgets;
};

class DownloadManager: public QObject
{
    Q_OBJECT
public:
    DownloadManager(QObject *parent = 0);

    void append(const QString url, const QString filename, QWidget *widget);
	void setDownloadDirectory( QString downdir );
	void download();
	bool queueExists(const QString filename, QWidget *widget);
	bool FileAlreadyExists( const QString filename, QWidget *widget );

private slots:
    void startNextDownload();
    void downloadFinished();
    void downloadReadyRead();

signals:
	void downloaded( QString filename, QWidget* widget, int error );

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
