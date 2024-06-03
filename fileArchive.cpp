#include <QApplication>
#include <QWidget>
#include <QFile>
#include <QString>
#include <QTextStream>
#include <windows.h>
#include <iostream>
#include <QTime>
#include <QDate>

void openConsole() {
    AllocConsole();
    freopen("CONOUT$", "w", stdout);
    freopen("CONOUT$", "w", stderr);
    freopen("CONIN$", "r", stdin);
    SetConsoleOutputCP(65001);
}

struct data{
    QTime qt;
    QDate qd;
    int temperature;
    int pressure;
};

int load(QVector<data> &v, QFile &f){
    int x;
    f.open(QIODevice::ReadWrite);
    f.read((char*)&x, sizeof(int));
    v.resize(x);
    f.read((char*) &v[0], sizeof(data)*x);
    f.close();
    return 0;
}

int input(QVector<data> &v, QFile &f ){
    data td;
    std::cout<<"Enter temperature: "<< std::endl;
    std::cin>>td.temperature;
    std::cout<<"Enter pressure: "<<std::endl;
    std::cin>>td.pressure;
    td.qd = QDate::currentDate();
    td.qt = QTime::currentTime();
    v.push_back(td);

    f.open(QIODevice::ReadWrite);
    int x = v.length();
    f.write((char*)&x, sizeof(int));
    f.write((char*) &v[0], sizeof(data) * v.length());
    f.close();
    return 0;
}

int india(QVector<data> &v){
    std::cout<<"All datas: "<<std::endl;
    for (int i = 0; i < v.length(); ++i) {
        std::cout<<v[i].qd.toString("dd.MM.yyyy").toStdString()<<" ";
        std::cout<<v[i].qt.toString("hh.mm.ss").toStdString()<<" ";
        std::cout<<" Temperature: " << v[i].temperature;
        std::cout<<" Pressure: "<< v[i].pressure;
        std::cout<<std::endl;
    }
}

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    openConsole();



    QVector<data> information;
    QFile file("data.txt");



    load(information, file);
    india(information);

    while (true) {
        std::cout << "Choose action:" << std::endl;
        std::cout << "1: Enter new data" << std::endl;
        std::cout << "2: Show all data" << std::endl;
        std::cout << "3: Exit" << std::endl;
        int i;
        std::cin >> i;

        if (std::cin.fail()) {
            std::cin.clear(); // Сброс состояния ошибки
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Очистка некорректного ввода
            std::cout << "Invalid choice. Please try again." << std::endl;
            continue;
        }

        switch (i) {
        case 1:
            input(information, file);
            break;
        case 2:
            india(information);
            break;
        case 3:
            return 0; // Exit the application
        default:
            std::cout << "Invalid choice. Please try again." << std::endl;
            break;
        }
    }
    input(information,file);




    QWidget window;
    window.resize(320, 240);
    window.setWindowTitle("Qt Console Output Example");
    window.show();

    return app.exec();
}
