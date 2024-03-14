// asyncimageloader.h
#ifndef ASYNCIMAGELOADER_H
#define ASYNCIMAGELOADER_H

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QObject>
#include <QUrl>

class AsyncImageLoader : public QObject {
    Q_OBJECT
public:
    explicit AsyncImageLoader(QObject* parent = nullptr);
    void loadImage(const QUrl& imageUrl);

signals:
    void imageLoaded(const QImage& image);

private slots:
    void onImageDownloaded(QNetworkReply* reply);

private:
    QNetworkAccessManager* m_networkManager;
};

#endif // ASYNCIMAGELOADER_H
