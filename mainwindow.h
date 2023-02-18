#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>
#include <QBuffer>
#include <QTextCodec>
#include <QDebug>
#include "http.h"
#include "database.h"
#include <QDateTime>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    //查询token是否到期
    void queryTokenDate();

private slots:
    //打开显示图片，转换图片的槽函数
    void openImgSlot();
    //获取access_token的槽函数
    void access_tokenSlot();
    //开始识别按钮
    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
    QString token = nullptr;//用于获取access_token的槽函数
    QByteArray imageData;
    //数据库
    DataBase db;
    int db_id = 1;//数据库id
    QString db_date;//数据库日期
    QString db_token;//数据库token
};
#endif // MAINWINDOW_H
