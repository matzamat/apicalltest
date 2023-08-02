#include "mainwindow.h"
#include "ui_mainwindow.h"

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
    // Получение текста из textEdit
    QString city = ui->textEdit->toPlainText();
    
    // Формирование URL для получения данных о городе
    QString cityst = "http://api.openweathermap.org/geo/1.0/direct?q=" + city + "&limit=1&appid=50f017c0a0b4e721843d6ba77e25eb44";
    QNetworkAccessManager Manager;
    QUrl APIurl1(cityst);
    QNetworkRequest request(APIurl1);

    // Отправка запроса и ожидание ответа
    QNetworkReply *reply = Manager.get(request);
    QEventLoop loop;
    QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    // Чтение ответа
    QString Response = reply->readAll();
    
    // Регистрация типа QJsonDocument для последующего использования
    qRegisterMetaType<QJsonDocument>();
    
    // Извлечение данных из JSON-ответа
    QJsonArray jsonArray = QJsonDocument::fromJson(Response.toUtf8()).array();
    QJsonObject jsonObject = jsonArray.at(0).toObject();
    QString country = jsonObject.value("country").toString();
    QString name = jsonObject.value("name").toString();

    // Формирование URL для получения данных о погоде
    QNetworkAccessManager Manager2;
    QString keke = "http://api.weatherapi.com/v1/current.json?key=3550410010fb4e9ea7c05942230108&q=" + name;
    QUrl APIurl2(keke);
    QNetworkRequest request2(APIurl2);
    QNetworkReply *reply2 = Manager2.get(request2);

    // Отправка запроса и ожидание ответа
    QEventLoop loop2;
    QObject::connect(reply2, &QNetworkReply::finished, &loop2, &QEventLoop::quit);
    loop2.exec();

    // Чтение ответа
    QString weather = reply2->readAll();
    
    // Извлечение данных из JSON-ответа
    QJsonObject jsonObject1 = QJsonDocument::fromJson(weather.toUtf8()).object();
    double temp_c = jsonObject1["current"].toObject()["temp_c"].toDouble();
    double wind_kph = jsonObject1["current"].toObject()["wind_kph"].toDouble();
    QString condition = jsonObject1["current"].toObject()["condition"].toObject()["text"].toString();
    int humidity = jsonObject1["current"].toObject()["humidity"].toInt();

    // Формирование строки с информацией о погоде
    QString answer = QString("Страна: \"%1\"\nГород: \"%2\"\nТемпература: %3°C\nВлажность: %4%\nОписание: %5\nСкорость ветра: %6м/с")
                        .arg(country)
                        .arg(name)
                        .arg(temp_c)
                        .arg(humidity)
                        .arg(condition)
                        .arg(wind_kph);

    // Вывод строки в textBrowser
    ui->textBrowser->setText(answer);
}
