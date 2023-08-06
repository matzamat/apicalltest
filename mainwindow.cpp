#include "mainwindow.h"
#include "ui_mainwindow.h"

Q_DECLARE_METATYPE(QJsonDocument)

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QObject::connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(Buttonclicked()));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::Buttonclicked()
{
    QString city = ui->textEdit->toPlainText();
    QNetworkAccessManager Manager;
    QString keke = "http://api.weatherapi.com/v1/current.json?key=3550410010fb4e9ea7c05942230108&q=" + city;
    QUrl APIurl(keke);
    QNetworkRequest request(APIurl);
    QNetworkReply *reply = Manager.get(request);

    QEventLoop loop;
    QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    QString weather = reply->readAll();
    QJsonObject jsonObject = QJsonDocument::fromJson(weather.toUtf8()).object();
    QString country = jsonObject["location"].toObject()["country"].toString();
    QString name = jsonObject["location"].toObject()["name"].toString();
    double temp_c = jsonObject["current"].toObject()["temp_c"].toDouble();
    double wind_kph = jsonObject["current"].toObject()["wind_kph"].toDouble();
    QString condition = jsonObject["current"].toObject()["condition"].toObject()["text"].toString();
    int humidity = jsonObject["current"].toObject()["humidity"].toInt();

    QString answer = QString("Страна: \"%1\"\nГород: \"%2\"\nТемпература: %3°C\nВлажность: %4%\nОписание: %5\nСкорость ветра: %6м/с")
                        .arg(country)
                        .arg(name)
                        .arg(temp_c)
                        .arg(humidity)
                        .arg(condition)
                        .arg(wind_kph);

    ui->textBrowser->setText(answer);

}
