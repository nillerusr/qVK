#ifndef DOWNLOADMANAGER_H
#define DOWNLOADMANAGER_H

#include <QFile>
#include <QObject>
#include <QQueue>
#include <QTime>
#include <QUrl>
#include <QNetworkAccessManager>
#include <QNetworkReply>

class DownloadManager: public QObject
{
    Q_OBJECT
public:
    DownloadManager(QObject *parent = 0);

    void append(QList<QPair<QString, QString>> urls);
    void append(const QString url, const QString filename);
	void setDownloadDirectory( QString downdir );
	void download();
	bool queueExists(const QString filename);
	
private slots:
    void startNextDownload();
    void downloadFinished();
    void downloadReadyRead();

signals:
	void downloaded( QString filename, int error );

private:
    QNetworkAccessManager manager;
    QQueue<QPair<QUrl, QString>> downloadQueue;
    QNetworkReply *currentDownload;
    QFile output;
	QString download_dir;
	bool bDownloading;
};


#endif // DOWNLOADMANAGER_H
