#ifndef DATABASE_H
#define DATABASE_H

/**********************存储date,token*****************/
#include <QDialog>
#include <QSqlDatabase>
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlError>
#include <QDebug>
#include <QMessageBox>

namespace Ui {
class DataBase;
}

class DataBase : public QDialog
{
    Q_OBJECT

public:
    explicit DataBase(QWidget *parent = nullptr);
    ~DataBase();

public:
    void createDB(void);
    void createTable(void);
    void queryTable(void);
    //插入数据（date，token）
    void insertData(int id,QString date,QString token);
    //修改数据
    void updateData(int id,QString date,QString token);
    //查询并返回date，token
    void query(QString &date,QString &token);

private:
    Ui::DataBase *ui;
   //QMapDataBase db;
    QSqlDatabase db;
    QSqlQueryModel model;
};

#endif // DATABASE_H
