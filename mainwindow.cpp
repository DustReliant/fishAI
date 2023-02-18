#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
   // this->setFixedSize(1050,750);
    //ui->label->setGeometry(10,10,1024,768);
    this->setWindowTitle(u8"鱼类图像识别系统");
    ui->progressBar->setValue(0);
    //数据库创建
    db.createDB();
    db.createTable();

    connect(ui->actiontoken,&QAction::triggered,[=]{
        db.queryTable();
        db.show();
    });
    connect(ui->action,&QAction::triggered,this,&MainWindow::openImgSlot);
    connect(ui->pushButton_2,&QPushButton::clicked,this,&MainWindow::openImgSlot);
    connect(ui->action_2,&QAction::triggered,this,&MainWindow::access_tokenSlot);

    //自动判断token是否到期
    this->queryTokenDate();
}

MainWindow::~MainWindow()
{
    delete ui;
}

//打开显示图片，转换图片的槽函数
void MainWindow::openImgSlot()
{
    //打开显示图片
    QString fileName = QFileDialog::getOpenFileName(this,u8"打开动物图片",QCoreApplication::applicationFilePath());
    ui->lineEdit_2->setText(fileName);
    if(fileName.isEmpty())
    {
        QMessageBox::warning(this,u8"提示",u8"请选择图片");
    }
    else
    {
        QImage image(fileName);
        ui->label->setPixmap(QPixmap::fromImage(image));
        //清空imageData
        imageData.clear();
        //转换图片为base64
        QByteArray ba;
        QBuffer buff(&ba);
        buff.open(QIODevice::WriteOnly);    //开辟缓存区到字节数组
        image.save(&buff,"jpg");            //将图片按照jpg格式存入字节数组
        QByteArray base64 = ba.toBase64();   //对图片base64编码（不包含编码头）
        //qDebug() << "base64:" << base64;
        //对图片urlencode
        QTextCodec *codec = QTextCodec::codecForName("UTF-8");
        imageData = "image=" + codec->fromUnicode(base64).toPercentEncoding();
        //qDebug() << "urlencode:" << imageData;
        //qDebug("");
    }

}

//获取access_token的槽函数
void MainWindow::access_tokenSlot()
{
    //返回为false则报警，返回true则修改数据库token，和日期
    if(http::access_token(token) == false){
        QMessageBox::warning(this,"Erron",u8"获取access_token失败！");
    }
    else{
        //修改数据库日期和token
        QString currentdate = QDateTime::currentDateTime().toString("dd.MM.yyyy");
        db.updateData(db_id,currentdate,token);
    }
}

//开始识别按钮
void MainWindow::on_pushButton_clicked()
{
    //获取并返回数据库date，token
    db.query(db_date,db_token);
    qDebug() << "token:" << db_token;
    QString name,score;                 //名字和置信度
    http::post_sync(db_token,imageData,name,score);      //获取检测结果
    qDebug() << "name(score)" << name << "(" << score << ")";

    QTextCharFormat fmt;
    //QPainter p(this);
    fmt.setForeground(Qt::red);//设置选中行的字体颜色
    fmt.setFont(QFont(u8"正楷",20,QFont::Bold,false));//参数依次是字体，大小，字体的粗细，以及是否斜体
    ui->textEdit->mergeCurrentCharFormat(fmt);

    //ui->textEdit->textColor();


    QString strName = u8"识别名称：" + name ;
    QString strScore = u8"识别概率：" + score +"\n\n\n";
    ui->textEdit->append(u8"本次识别的结果为：");
    ui->textEdit->append(strName);
    ui->textEdit->append(strScore);

    ui->progressBar->setWindowModality(Qt::WindowModal); // 设置模态显示
   ui->progressBar->setRange(0, 100);
   for (int i = 1; i < 100 + 1; i++)
   {
       ui->progressBar->setValue(i);
   }


    //ui->lineEdit->setText(name + "(" + score + ")");
}

//查询token是否到期
void MainWindow::queryTokenDate()
{
    qDebug() << u8"开始自动检查token日期";
    //获取并返回数据库date，token
    db.query(db_date,db_token);
    //为空则插入数据
    if(db_date.isEmpty() || db_token.isEmpty()){
        db_date = QDateTime::currentDateTime().toString("dd.MM.yyyy");
        this->access_tokenSlot();
        db.insertData(db_id,db_date,token);
    }
    //有数据则判断是否到期
    else{
        //当前日期
        QString currentdate = QDateTime::currentDateTime().toString("dd.MM.yyyy");
        QStringList list = currentdate.split(".");
        int dd = list[0].toInt();
        int MM = list[1].toInt();
        int yyyy = list[2].toInt();
        //数据库日期
        QStringList list2 = db_date.split(".");
        int dd2 = list2[0].toInt();
        int MM2 = list2[1].toInt();
        int yyyy2 = list2[2].toInt();
        if(yyyy - yyyy2 > 0){
            this->access_tokenSlot();
            qDebug() << u8"自动获取token";
        }
        else if((MM - MM2 != 0)){
            this->access_tokenSlot();
            qDebug() << u8"自动获取token";
        }
        else if (dd - dd2 > 29) {
            this->access_tokenSlot();
            qDebug() << u8"自动获取token";
        }
    }

}
