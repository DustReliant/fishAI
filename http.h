#ifndef HTTP_H
#define HTTP_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QEventLoop>
#include <QMessageBox>
#include <QJsonDocument>
#include <QJsonObject>

const QString baidu_token = "https://aip.baidubce.com/oauth/2.0/token?grant_type=client_credentials&client_id=%1&client_secret=%2";
const QString AK = "ZgZ2Oh9u6tY8PDjxvK0KS620";
const QString SK = "Gm9lqNOVXoUw4BY1UaqTBvjDxDPtSdvL";
const QString baidu_sync = "https://aip.baidubce.com/rest/2.0/image-classify/v1/animal?access_token=%1";

class http:QObject
{
    Q_OBJECT
public:
    http();
    //获取access_token
    static bool access_token(QString &token);
    //获取动物识别结果
    static bool post_sync(QString token,QByteArray &imageData,QString &name,QString &score);
private:


};

#endif // HTTP_H
