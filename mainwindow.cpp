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
    // Get the entered city from the text edit widget
    QString city = ui->textEdit->toPlainText();

    // Create a network manager object
    QNetworkAccessManager Manager;

    // Construct the API URL with the city parameter
    QString apiUrl = "http://api.weatherapi.com/v1/current.json?key=3550410010fb4e9ea7c05942230108&q=" + city;
    QUrl APIurl(apiUrl);

    // Create a network request with the API URL
    QNetworkRequest request(APIurl);

    // Send the GET request and get the reply
    QNetworkReply *reply = Manager.get(request);

    // Create an event loop to wait for the reply to finish
    QEventLoop loop;
    
    // Connect the finished signal of the reply to the quit slot of the event loop
    QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    
    // Start the event loop to wait for the reply
    loop.exec();

    // Read the response data from the reply
    QString weather = reply->readAll();

    // Parse the JSON response
    QJsonObject jsonObject = QJsonDocument::fromJson(weather.toUtf8()).object();
    
    // Extract the required information from the JSON object
    QString country = jsonObject["location"].toObject()["country"].toString();
    QString name = jsonObject["location"].toObject()["name"].toString();
    double temp_c = jsonObject["current"].toObject()["temp_c"].toDouble();
    double wind_kph = jsonObject["current"].toObject()["wind_kph"].toDouble();
    QString condition = jsonObject["current"].toObject()["condition"].toObject()["text"].toString();
    int humidity = jsonObject["current"].toObject()["humidity"].toInt();

    // Create the answer string with the extracted information
    QString answer = QString("Страна: \"%1\"\nГород: \"%2\"\nТемпература: %3°C\nВлажность: %4%\nОписание: %5\nСкорость ветра: %6м/с")
                        .arg(country)
                        .arg(name)
                        .arg(temp_c)
                        .arg(humidity)
                        .arg(condition)
                        .arg(wind_kph);

    // Set the answer string as the text in the text browser widget
    ui->textBrowser->setText(answer);
}
