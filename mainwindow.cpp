#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "CFGParser.h"
#include "PdaBuilder.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <QGraphicsScene>
#include <QGraphicsTextItem>
#include <QGraphicsEllipseItem>
#include <QGraphicsLineItem>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_pdaScene(nullptr)
{
    ui->setupUi(this);

    setupCFGInputArea();
    setupPDAOutputArea();

    connect(ui->btnLoadCFG, &QPushButton::clicked, this, &MainWindow::loadCFGFromFile);
    connect(ui->btnClear, &QPushButton::clicked, this, &MainWindow::clearInput);
    connect(ui->btnValidate, &QPushButton::clicked, this, &MainWindow::validateCFG);
    connect(ui->btnConvert, &QPushButton::clicked, this, &MainWindow::convertToPDA);
}

MainWindow::~MainWindow()
{
    delete ui;
    if (m_pdaScene) delete m_pdaScene;
}

void MainWindow::setupCFGInputArea()
{
    QString sampleCFG =
        "Variables: S A B\n"
        "Terminals: a b\n"
        "Start: S\n"
        "Productions:\n"
        "S -> aB | bA\n"
        "A -> a | bAA | ε\n"
        "B -> b | aBB";

    ui->txtCFGInput->setPlainText(sampleCFG);

    // ui->txtCFGInput->setStyleSheet("font-family: monospace; font-size: 14px; background-color: #f9f9f9;");
    ui->txtCFGInput->setStyleSheet(
        "QPlainTextEdit {"
        "   font-family: 'Consolas', 'Courier New', monospace;"
        "   font-size: 14px;"
        "   background-color: #e8f5e9;"  // Light green background
        "   color: #2e7d32;"            // Dark green text
        "   padding: 8px;"
        "   border: 2px solid #81c784;" // Medium green border
        "   border-radius: 6px;"
        "   selection-background-color: #1b5e20;"  // Dark green selection
        "   selection-color: #ffffff;"
        "}"
        "QPlainTextEdit:focus {"
        "   border: 2px solid #2e7d32;"  // Darker green when focused
        "   background-color: #f1f8e9;"   // Brighter background on focus
        "}"
        "QScrollBar:vertical {"
        "   background: #c8e6c9;"         // Light green track
        "   width: 12px;"
        "   margin: 0px;"
        "}"
        "QScrollBar::handle:vertical {"
        "   background: #66bb6a;"         // Medium green handle
        "   min-height: 30px;"
        "   border-radius: 6px;"
        "}"
        );
}




void MainWindow::setupPDAOutputArea()
{
    m_pdaScene = new QGraphicsScene(this);
    ui->graphicsView->setScene(m_pdaScene);
    ui->graphicsView->setRenderHint(QPainter::Antialiasing);

    ui->graphicsView->setStyleSheet(
        "QGraphicsView {"
        "   background-color: #f1f8e9;"  // Light green background
        "   border: 2px solid #81c784;"  // Medium green border
        "   border-radius: 6px;"
        "}"
        );

    ui->txtPDAOutput->setReadOnly(true);
    ui->txtPDAOutput->setStyleSheet(
        "QPlainTextEdit {"
        "   font-family: 'Consolas', 'Courier New', monospace;"
        "   font-size: 14px;"
        "   background-color: #e8f5e9;"  // Light green background
        "   color: #2e7d32;"             // Dark green text
        "   padding: 8px;"
        "   border: 2px solid #81c784;"  // Medium green border
        "   border-radius: 6px;"
        "   selection-background-color: #1b5e20;"  // Dark green selection
        "   selection-color: #ffffff;"
        "}"
        "QScrollBar:vertical {"
        "   background: #c8e6c9;"       // Light green track
        "   width: 12px;"
        "}"
        "QScrollBar::handle:vertical {"
        "   background: #66bb6a;"       // Medium green handle
        "   min-height: 30px;"
        "   border-radius: 6px;"
        "}"
        );
}

void MainWindow::loadCFGFromFile()
{
    QString fileName = QFileDialog::getOpenFileName(
        this,
        "Open CFG File",
        "",
        "Text Files (*.txt);;All Files (*)"
        );

    if (fileName.isEmpty()) return;

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(
            this,
            "Error",
            "Could not open file",
            QMessageBox::Ok,
            QMessageBox::Ok
            );
        return;
    }

    QTextStream in(&file);
    ui->txtCFGInput->setPlainText(in.readAll());
    file.close();
}

void MainWindow::clearInput()
{
    // ui->txtCFGInput->clear();
    ui->txtPDAOutput->clear();
    if (m_pdaScene) m_pdaScene->clear();
}

void MainWindow::validateCFG()
{
    QString input = ui->txtCFGInput->toPlainText().trimmed();
    if (input.isEmpty()) {
        QMessageBox::warning(this, "Validation", "Input is empty!");
        return;
    }

    QStringList lines = input.split('\n', Qt::SkipEmptyParts);
    if (lines.size() < 4) {
        QMessageBox::critical(this, "Validation Error",
                              "Invalid format. Expected:\n"
                              "Variables: ...\n"
                              "Terminals: ...\n"
                              "Start: ...\n"
                              "Productions: ...");
        return;
    }

    // Basic structure validation
    if (!lines[0].startsWith("Variables:") ||
        !lines[1].startsWith("Terminals:") ||
        !lines[2].startsWith("Start:")) {
        QMessageBox::critical(this, "Validation Error",
                              "First three lines must begin with:\n"
                              "Variables:\nTerminals:\nStart:");
        return;
    }

    // Production rules validation
    bool productionsValid = true;
    // for (int i = 3; i < lines.size(); i++) {
    //     if (!lines[i].contains("->") || !lines[i].contains("→")) {
    //         productionsValid = false;
    //         break;
    //     }
    // }

    if (!productionsValid) {
        QMessageBox::critical(this, "Validation Error",
                              "Production rules must use '->' or '→'");
        return;
    }

    // If all checks passed
    QMessageBox::information(this, "Validation",
                             "CFG structure appears valid!\n"
                             "Note: This checks syntax only, not semantic correctness.");
}
void MainWindow::convertToPDA()
{
    try {
        CFG* cfg = CFGParser::parseFromString(ui->txtCFGInput->toPlainText());
        PDA* pda = PdaBuilder::buildFromCFG(*cfg);

        displayPDA(pda);

        delete cfg;
        delete pda;
    } catch (const std::exception& e) {
        QMessageBox::critical(
            this,
            "Conversion Error",
            e.what(),
            QMessageBox::Ok,
            QMessageBox::Ok
            );
    }
}

void MainWindow::displayPDA(PDA* pda)
{
    m_pdaScene->clear();
    ui->txtPDAOutput->clear();

    drawPDAStates();
    drawPDATransitions(pda);

    QString pdaText = generatePDAText(pda);
    ui->txtPDAOutput->setPlainText(pdaText);
}

void MainWindow::drawPDAStates()
{
    // State styling
    QPen statePen(QColor("#1b5e20"), 2);  // Dark green border
    QBrush stateBrush(QColor("#e8f5e9"));  // Light green fill
    QFont stateFont("Arial", 10, QFont::Bold);

    // State dimensions and positions
    const int stateDiameter = 70;
    const int q0X = 50, q1X = 250, q2X = 450;
    const int stateY = 100;

    // Draw q0 state
    QGraphicsEllipseItem* q0 = m_pdaScene->addEllipse(
        q0X, stateY, stateDiameter, stateDiameter, statePen, stateBrush);
    QGraphicsTextItem* q0Text = m_pdaScene->addText("q0");
    q0Text->setDefaultTextColor(QColor("#1b5e20"));
    q0Text->setFont(stateFont);
    q0Text->setPos(q0X + stateDiameter/2 - 10, stateY + stateDiameter/2 - 10);

    // Draw q1 state
    QGraphicsEllipseItem* q1 = m_pdaScene->addEllipse(
        q1X, stateY, stateDiameter, stateDiameter, statePen, stateBrush);
    QGraphicsTextItem* q1Text = m_pdaScene->addText("q1");
    q1Text->setDefaultTextColor(QColor("#1b5e20"));
    q1Text->setFont(stateFont);
    q1Text->setPos(q1X + stateDiameter/2 - 10, stateY + stateDiameter/2 - 10);

    // Draw q2 accept state (with double circle)
    QGraphicsEllipseItem* q2 = m_pdaScene->addEllipse(
        q2X, stateY, stateDiameter, stateDiameter, statePen, stateBrush);
    QGraphicsEllipseItem* q2Double = m_pdaScene->addEllipse(
        q2X-3, stateY-3, stateDiameter+6, stateDiameter+6, statePen);
    QGraphicsTextItem* q2Text = m_pdaScene->addText("q2");
    q2Text->setDefaultTextColor(QColor("#1b5e20"));
    q2Text->setFont(stateFont);
    q2Text->setPos(q2X + stateDiameter/2 - 10, stateY + stateDiameter/2 - 10);
}

void MainWindow::drawPDATransitions(PDA* pda)
{
    // Transition styling
    QPen transitionPen(QColor("#2e7d32"), 2, Qt::SolidLine, Qt::RoundCap);
    QFont labelFont("Consolas", 9);
    int yOffset = 0;
    const int q1CenterX = 285; // q1 X position + radius

    // Arrowhead style
    QBrush arrowBrush(QColor("#1b5e20"));
    const double arrowSize = 10;

    // q0 → q1 transition
    QGraphicsLineItem* l1 = m_pdaScene->addLine(
        120, 135, 250, 135, transitionPen);
    // addArrowhead(l1->line().p2(), arrowSize, arrowBrush);
    addArrowhead(l1->line().p1(), l1->line().p2(), arrowSize, arrowBrush);







    QGraphicsTextItem* t1 = m_pdaScene->addText("ε, ε → SZ");
    t1->setDefaultTextColor(QColor("#1b5e20"));
    t1->setFont(labelFont);
    t1->setPos(160, 110);



    // Process q1→q1 transitions
    auto transitions = pda->getTransitions();
    const int maxTransitions = 5; // Limit for clean display

    for (auto stateIt = transitions.begin(); stateIt != transitions.end(); ++stateIt) {
        if (stateIt.key() != "q1") continue;

        const auto& stateTransitions = stateIt.value();
        int transitionCount = 0;

        for (auto transIt = stateTransitions.begin();
             transIt != stateTransitions.end() && transitionCount < maxTransitions;
             ++transIt)
        {
            const auto& inputStackPair = transIt.key();
            const auto& targetList = transIt.value();

            for (const auto& target : targetList) {
                if (target.first != "q1") continue;

                // Create curved self-loop arrow
                QPainterPath path;
                path.moveTo(q1CenterX, 170 + yOffset);
                path.quadTo(q1CenterX + 50, 190 + yOffset, q1CenterX, 210 + yOffset);

                QGraphicsPathItem* arrow = m_pdaScene->addPath(path, transitionPen);
                addArrowhead(path.pointAtPercent(1.0), arrowSize, arrowBrush);

                // Transition label
                QString label = QString("%1, %2 → %3")
                                    .arg(inputStackPair.first)
                                    .arg(inputStackPair.second)
                                    .arg(target.second);

                QGraphicsTextItem* transText = m_pdaScene->addText(label);
                transText->setDefaultTextColor(QColor("#1b5e20"));
                transText->setFont(labelFont);
                transText->setPos(q1CenterX + 60, 180 + yOffset);

                yOffset += 40;
                transitionCount++;

                if (transitionCount >= maxTransitions &&
                    transIt != stateTransitions.end())
                {
                    QGraphicsTextItem* moreText = m_pdaScene->addText("...");
                    moreText->setPos(q1CenterX + 60, 180 + yOffset);
                    break;
                }
            }
        }
    }

    // q1 → q2 transition
    QGraphicsLineItem* l2 = m_pdaScene->addLine(320, 135, 450, 135, transitionPen);
    // addArrowhead(l2->line().p2(), arrowSize, arrowBrush);
    addArrowhead(l2->line().p1(), l2->line().p2(), arrowSize, arrowBrush);






    QGraphicsTextItem* t2 = m_pdaScene->addText("ε, Z → ε");

    t2->setDefaultTextColor(QColor("#1b5e20"));
    t2->setFont(labelFont);
    t2->setPos(360, 110);
}





// Helper function to draw arrowheads
void MainWindow::addArrowhead(QPointF point, double size, const QBrush& brush)
{
    QPolygonF arrowHead;
    arrowHead << QPointF(0, 0)
              << QPointF(-size, -size/2)
              << QPointF(-size, size/2);

    QGraphicsPolygonItem* arrow = m_pdaScene->addPolygon(arrowHead, QPen(), brush);
    arrow->setPos(point);
    arrow->setRotation(-180 + qRadiansToDegrees(atan2(
                           point.y() - point.y(), point.x() - point.x())));
}

void MainWindow::addArrowhead(QPointF start, QPointF end, double size, const QBrush& brush)
{
    QPolygonF arrowHead;
    arrowHead << QPointF(0, 0)
              << QPointF(-size, -size / 2)
              << QPointF(-size, size / 2);

    QGraphicsPolygonItem* arrow = m_pdaScene->addPolygon(arrowHead, QPen(), brush);
    arrow->setPos(end);

    double angle = std::atan2(end.y() - start.y(), end.x() - start.x());
    arrow->setRotation(qRadiansToDegrees(angle));
}




QString MainWindow::generatePDAText(PDA* pda)
{
    QString text;
    text += "=== PDA Definition ===\n\n";
    text += "States: " + QStringList(pda->getStates().begin(), pda->getStates().end()).join(", ") + "\n";
    text += "Input Alphabet: " + QStringList(pda->getInputAlphabet().begin(), pda->getInputAlphabet().end()).join(", ") + "\n";
    text += "Stack Alphabet: " + QStringList(pda->getStackAlphabet().begin(), pda->getStackAlphabet().end()).join(", ") + "\n";
    text += "Start State: " + pda->getStartState() + "\n";
    text += "Start Stack Symbol: " + pda->getStartStackSymbol() + "\n";
    text += "Accept States: " + QStringList(pda->getAcceptStates().begin(), pda->getAcceptStates().end()).join(", ") + "\n\n";
    text += "=== Transition Function ===\n";

    // Initial transition
    text += "δ(q0, ε, ε) → (q1, SZ)\n";

    // Get all transitions
    auto transitions = pda->getTransitions();

    // Process q1 transitions
    if (transitions.contains("q1")) {
        auto q1Transitions = transitions["q1"];

        // Filter and display valid transitions
        for (auto it = q1Transitions.begin(); it != q1Transitions.end(); ++it) {
            auto input = it.key();

            // Skip any transition containing metadata strings like "Terminals:"
            if (input.first.contains(":") || input.second.contains(":")) {
                continue;
            }

            // Skip the final Z transition (handled separately)
            if (input.first == "ε" && input.second == "Z") {
                continue;
            }

            for (const auto& transition : it.value()) {
                text += QString("δ(q1, %1, %2) → (%3, %4)\n")
                            .arg(input.first)
                            .arg(input.second)
                            .arg(transition.first)
                            .arg(transition.second);
            }
        }
    }

    // Final transition
    text += "δ(q1, ε, Z) → (q2, ε)\n";

    return text;
}
