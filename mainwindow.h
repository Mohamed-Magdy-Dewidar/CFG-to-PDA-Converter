// #ifndef MAINWINDOW_H
// #define MAINWINDOW_H

// #include <QMainWindow>
// #include "CFG.h"
// #include "PDA.h"




// QT_BEGIN_NAMESPACE
// namespace Ui {
// class MainWindow;
// }
// QT_END_NAMESPACE

// class MainWindow : public QMainWindow
// {
//     Q_OBJECT

// public:
//     MainWindow(QWidget *parent = nullptr);
//     ~MainWindow();

// private:
//     Ui::MainWindow *ui;
//     CFG currentCFG;
//     PDA currentPDA;

//     void updatePdaDisplay();
// };
// #endif // MAINWINDOW_H


#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include "CFG.h"
#include "PDA.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void loadCFGFromFile();
    void clearInput();
    void validateCFG();
    void convertToPDA();

private:
    Ui::MainWindow *ui;
    QGraphicsScene *m_pdaScene;
    CFG currentCFG;

    // UI setup
    void setupCFGInputArea();
    void setupPDAOutputArea();

    // PDA rendering
    void displayPDA(PDA* pda);
    void drawPDAStates();
    void drawPDATransitions(PDA* pda);
    void addArrowhead(QPointF point, double size, const QBrush& brush);
    void addArrowhead(QPointF start, QPointF end, double size, const QBrush& brush);
    QString generatePDAText(PDA* pda);
};

#endif // MAINWINDOW_H
