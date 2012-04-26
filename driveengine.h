#ifndef DRIVEENGINE_H
#define DRIVEENGINE_H

#include <QObject>
#include "oauth2.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QSslError>

class DriveEngine : public QObject
{
    Q_OBJECT
public:
    explicit DriveEngine(QObject *parentObj = 0);
    ~DriveEngine();

public:
    void init(void);
    
public slots:    
    void slotStartLogin(void);

private slots:
    void slotReplyFinished(QNetworkReply* reply);
    void slotGetFile(void);
    void slotReadyRead();
    void slotError(QNetworkReply::NetworkError error);
    void slotSslErrors( const QList<QSslError>& errors);

private:
    OAuth2 oAuth2;
    QNetworkAccessManager* networkAccessManager;
    QObject* parent;
    QNetworkReply *reply;
    QNetworkRequest request;
    QString replyStr;
};

#endif // DRIVEENGINE_H
