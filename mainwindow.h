#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QColorDialog>
#include <QCloseEvent>
#include <QMessageBox>

#include "paintwidget.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    void closeEvent(QCloseEvent *event);
    ~MainWindow();

public slots:
    void displayModifiedInfo(QString);
    void displaySavedInfo(QString);
    void displayHelp();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
