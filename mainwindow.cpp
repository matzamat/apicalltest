#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QString>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QEventLoop>

Q_DECLARE_METATYPE(QJsonDocument)

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    QNetworkAccessManager Manager;
    QString city = ui->textEdit->toPlainText();
    QString cityst = "http://api.openweathermap.org/geo/1.0/direct?q=" + city + "&limit=1&appid=50f017c0a0b4e721843d6ba77e25eb44";
    QUrl APIurl1(cityst);
    QNetworkRequest request(APIurl1);
    qDebug()<<"Call http" << cityst;

    QNetworkReply *reply = Manager.get(request);

    QEventLoop loop;
    QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    if (reply != nullptr) {
        QString Response = reply->readAll();
        qDebug()<<"Api reply" << Response;

        qRegisterMetaType<QJsonDocument>();
        QJsonArray jsonArray = QJsonDocument::fromJson(Response.toUtf8()).array();
        QJsonObject jsonObject = jsonArray.at(0).toObject();
        QString country = jsonObject.value("country").toString();
        QString name = jsonObject.value("name").toString();

                QNetworkAccessManager Manager2;
                QString keke = "http://api.weatherapi.com/v1/current.json?key=3550410010fb4e9ea7c05942230108&q=" + name;
                qDebug()<<"api call" << keke;
                QUrl APIurl2(keke);
                QNetworkRequest request2(APIurl2);
                QNetworkReply *reply2 = Manager2.get(request2);
                QEventLoop loop2;
                QObject::connect(reply2, &QNetworkReply::finished, &loop2, &QEventLoop::quit);
                loop2.exec();
                if (reply2 != nullptr) {
                    QString weather = reply2->readAll();
                    qDebug() << "Api reply: " << weather;
                    QJsonObject jsonObject = QJsonDocument::fromJson(weather.toUtf8()).object();

                    double temp_c = jsonObject["current"].toObject()["temp_c"].toDouble();
                    double wind_kph = jsonObject["current"].toObject()["wind_kph"].toDouble();
                    QString condition = jsonObject["current"].toObject()["condition"].toObject()["text"].toString();
                    int humidity = jsonObject["current"].toObject()["humidity"].toInt();
                    qDebug() << "ТЕМПЕРАТУРА" << temp_c << wind_kph << condition << humidity;

                    QString answer1 = QString("Страна: %1\nГород: %2\nТемпература: %3\nСкорость ветра: %4\nОписание: %5\nВлажность: %6")
                        .arg(country)
                        .arg(name)
                        .arg(temp_c)
                        .arg(wind_kph)
                        .arg(condition)
                        .arg(humidity);

                    ui->textBrowser->setText(answer1);
                  } else {
                      ui->textBrowser->setText("Invalid response");
                  }



                }



}
