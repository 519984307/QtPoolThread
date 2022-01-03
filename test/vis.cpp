#include "vis.h"
#include "ui_vis.h"

Vis::Vis(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::vis)
{
    ui->setupUi(this);
    numAvailableThreads = std::thread::hardware_concurrency() - 1;
    connect(ui->startBtn, SIGNAL(clicked()), this, SLOT(startBtn()));
    connect(ui->numThreads, SIGNAL(valueChanged(int)), this, SLOT(numThreadsChanged(int)));
    connect(ui->numTasks, SIGNAL(valueChanged(int)), this, SLOT(numThreadsChanged(int)));
    ui->numThreads->setMaximum(numAvailableThreads);
    ui->numThreads->setMinimum(1);
    numThreadsChanged(1);
    ui->numTasks->setValue(20);
}

Vis::~Vis()
{
    delete ui;
}

void Vis::startBtn()
{
    numStart++;
    if (numThreads < 1){
        QMessageBox::critical(this, tr("Error"), tr("Number of threads should be >= 1"));
        numThreads = 1;
    }
    else if (numThreads > numAvailableThreads){
        QMessageBox::critical(this, tr("Error"), tr("Number of threads should be less than available"));
        numThreads = numAvailableThreads;
    }
    int numThread_ = (numAvailableThreads != 0 && numThreads < numAvailableThreads ) ? numThreads :numAvailableThreads  ;
    ui->numThreads->setValue(numThread_);
    enableRun = true;
    emit enabledStart(numThread_);

}




void Vis::numThreadsChanged(int arg1)
{
    numThreads = ui->numThreads->value();
}

void Vis::numTasksChanged(int arg1)
{
    numTasks = ui->numThreads->value();
    emit numberTasks(numTasks);
}



void Vis::setResult(const std::string &str)
{
//    ui->result->clear();
    ui->result->text();
    ui->result->setText(ui->result->text()+ "\n" + QString::number(numStart) + ")" +str.c_str());
}

