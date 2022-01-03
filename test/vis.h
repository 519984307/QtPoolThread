#pragma once

#include <QWidget>
#include <QMessageBox>
#include <thread>

namespace Ui {
class vis;
}

class Vis : public QWidget
{
    Q_OBJECT

public:
    explicit Vis(QWidget *parent = nullptr);
    ~Vis();
    bool enableRun{false};
    int numThreads{-1};
    int numAvailableThreads{-1};
    Ui::vis *ui;
    int numStart{0};
    int numTasks{-1};

public slots:
    void startBtn();
    void numThreadsChanged(int arg1);
    void numTasksChanged(int arg1);
    void setResult(const std::string &str);

signals:
    void enabledStart(const int &value);
    void numberTasks(const int &value);

};

