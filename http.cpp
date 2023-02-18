#include "http.h"

http::http()
{

}
bool http::access_token(QString &token)
{
    //设置url
    QString url_token = QString(baidu_token).arg(AK).arg(SK);
    //发起请求，并获得返回结果
    QNetworkAccessManager manager;
    QNetworkRequest request;
    request.setUrl(url_token);
    request.setRawHeader("Content-Type","application/x-www-form-urlencoded");
    QNetworkReply *reply = manager.post(request,"");
    QEventLoop loop;
    connect(reply,&QNetworkReply::finished,&loop,&QEventLoop::quit);
    loop.exec();
    //读取返回结果
    if(reply != nullptr && reply->error() == QNetworkReply::NoError)
    {
        QByteArray array = reply->readAll();
        QJsonObject obj = QJsonDocument::fromJson(array).object();
        token = obj.value("access_token").toString();
        return true;
    }
    else
    {
        return false;
    }
}
//获取动物识别结果
bool http::post_sync(QString token,QByteArray &imageData,QString &name,QString &score)
{
    //设置url
    QString url_sync;
    if(token.isEmpty())
    {
        qDebug() << token;
        return false;
    }
    else
    {
        url_sync = QString(baidu_sync).arg(token);
    }
    //发起请求，并获得返回结果
    QNetworkAccessManager manager;
    QNetworkRequest request;
    request.setUrl(url_sync);
    request.setRawHeader("Content-Type","application/x-www-form-urlencoded");
    QNetworkReply *reply = manager.post(request,imageData);
    QEventLoop loop;
    connect(reply,&QNetworkReply::finished,&loop,&QEventLoop::quit);
    loop.exec();
    //读取返回结果
    if(reply != nullptr && reply->error() == QNetworkReply::NoError)
    {
        QByteArray array = reply->readAll();
        QJsonObject obj = QJsonDocument::fromJson(array).object();
        QJsonValue val = obj.value("result");
//        QJsonArray jsonArray = val.toArray();
//        QJsonArray arrayList = val.toArray();
        if(val.isArray())
        {
            QJsonValue first = val[0];
            //打印val【0】
            //qDebug() << "val[0]" << val[0].toObject().value("name").toString();
            name = first.toObject().value("name").toString();
            score = first.toObject().value("score").toString();
        }
        return true;
    }
    else
    {
        return false;
    }
}
