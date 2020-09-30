#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->menuBar->setNativeMenuBar(false);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if(!ui->paintWidget->getSaveState()){

        QMessageBox msgBox;
        msgBox.setText(tr("Do you want to save your work ?"));
        QAbstractButton* pButtonYes = msgBox.addButton(tr("Save"), QMessageBox::YesRole);
        QAbstractButton* pButtonNo = msgBox.addButton(tr("Do not save"), QMessageBox::NoRole);
        QAbstractButton* pButtonCancel = msgBox.addButton(tr("Cancel"), QMessageBox::RejectRole);

        msgBox.exec();

        if (msgBox.clickedButton()==pButtonYes) {
            ui->paintWidget->save();
            if(ui->paintWidget->getSaveState()){
                event->accept();
            } else{
                event->ignore();
            }
        }
        if (msgBox.clickedButton()==pButtonNo) {
            event->accept();
        }
        if (msgBox.clickedButton()==pButtonCancel) {
            event->ignore();
        }
    }
}

void MainWindow::displayModifiedInfo(QString title)
{
    setWindowTitle(title + "*");
}

void MainWindow::displaySavedInfo(QString title)
{
    setWindowTitle(title);
}

void MainWindow::displayHelp(){
    QMessageBox::information(this,
         tr("About Ensi-Mandala"),
         tr("Ensi-Mandala: An interactive tool for drawing Mandalas.\n\n"
         "(C) 2019 Timothee Soen & Cyprien De Slizewicz"),
         QMessageBox::Ok);
}


MainWindow::~MainWindow()
{
    delete ui;
}
