#include "database.h"
#include "ui_database.h"
#include <QSqlDatabase>
DataBase::DataBase(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DataBase)
{
    ui->setupUi(this);
}

DataBase::~DataBase()
{
    delete ui;
}

//创建数据库
void DataBase::createDB(void)
{
    //创建驱动
    db = QSqlDatabase::addDatabase("QSQLITE");
    //设置数据库名称
    db.setDatabaseName("baiduToken.db");
    //打开数据库
    if(db.open() == true){
        qDebug() << "创建/打开数据库成功！";
    }
    else {
        qDebug() << "创建/打开数据库失败！";
    }
}

//创建数据表
void DataBase::createTable(void)
{
    QSqlQuery query;
    QString str = QString("CREATE TABLE baiduToken ("
                          "id INT PRIMARY KEY NOT NULL,"
                          "date DATE NOT NULL,"
                          "token TEXT NOT NULL)");
    if(query.exec(str) == false){
        qDebug() << str;
    }
    else{
        qDebug() << u8"创建/打开数据表成功";
    }
}

//查询并显示数据表
void DataBase::queryTable(void)
{
    QString str = QString("SELECT * FROM baiduToken");
    model.setQuery(str);
    ui->tableView->setModel(&model);
}

//插入数据按钮(id，date,token)
void DataBase::insertData(int id,QString date,QString token)
{
    int dbId = id;
    QString dbDate = date;
    if(dbDate == ""){
        qDebug() << "时间获取错误";
        return;
    }
    QString dbToken = token;
    if(dbToken == ""){
        qDebug() << "token输入错误";
        return;
    }
    QSqlQuery query;
    QString str = QString("INSERT INTO baiduToken VALUES(%1,'%2','%3')").arg(dbId).arg(dbDate).arg(dbToken);
    if(query.exec(str) == false){
        qDebug() << str<< "插入数据失败";;
    }
    else{
        qDebug() << "插入数据成功";
        queryTable();
    }
}

//修改数据按钮
void DataBase::updateData(int id,QString date,QString token)
{
    QSqlQuery query;
    int dbId = id;
    QString dbDate = date;
    QString dbToken = token;
    if((dbDate == "") || (dbToken == "")){
        qDebug() << u8"修改数据错误";
        return;
    }
    QString str1 = QString("UPDATE baiduToken SET date='%1' WHERE id=%2").arg(dbDate).arg(dbId);
    QString str2 = QString("UPDATE baiduToken SET token='%1' WHERE id=%2").arg(dbToken).arg(dbId);
    if((query.exec(str1) == false) || (query.exec(str2) == false)){
        qDebug() << u8"修改数据失败";
        qDebug() << str1 << str2;
    }
    else{
        qDebug() << u8"修改数据成功";
        queryTable();
    }
}

//查询并返回date，token
void DataBase::query(QString &date,QString &token)
{
    QSqlQuery query;
    query.exec("SELECT date, token FROM baiduToken");
    while (query.next()) {
        date = query.value(0).toString();
        token = query.value(1).toString();
        qDebug() << u8"查询数据库并返回date/token" <<date << token;
    }
}
