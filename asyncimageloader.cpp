// asyncimageloader.cpp
#include "asyncimageloader.h"
#include <QImage>
AsyncImageLoader::AsyncImageLoader(QObject* parent)
    : QObject(parent)
{
    m_networkManager = new QNetworkAccessManager(this);
    connect(m_networkManager, &QNetworkAccessManager::finished, this, &AsyncImageLoader::onImageDownloaded);
}

void AsyncImageLoader::loadImage(const QUrl& imageUrl)
{
    QNetworkRequest request(imageUrl);
    m_networkManager->get(request);
}

void AsyncImageLoader::onImageDownloaded(QNetworkReply* reply)
{
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray imageData = reply->readAll();
        QImage image;
        image.loadFromData(imageData);
        emit imageLoaded(image);
    } else {
        qDebug() << "Image download failed:" << reply->errorString();
    }
    reply->deleteLater();
}
