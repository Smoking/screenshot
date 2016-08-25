#include "controlwidget.h"
#include "ui_controlwidget.h"
#include <QPoint>
#include "screen.h"
#include <QDebug>
#include <QApplication>
#include <QClipboard>

ControlWidget::ControlWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ControlWidget),
    screen(NULL)
{
    ui->setupUi(this);
    connectSignalAndSlot();
}

void ControlWidget::connectSignalAndSlot()
{
    connect(ui->cancelBtn, SIGNAL(clicked()), this, SLOT(cancelBtn_slot()));
    connect(ui->saveBtn, SIGNAL(clicked()), this, SLOT(saveBtn_slot()));
    connect(ui->finishBtn, SIGNAL(clicked()), this, SLOT(finishBtn_slot()));
}

void ControlWidget::cancelBtn_slot()
{
    if( screen )
    {
        screen->close();
    }
}

void ControlWidget::saveBtn_slot()
{
    screen->savePixmap();
    cancelBtn_slot();
}

//完成按钮保存到系统剪贴板上面
void ControlWidget::finishBtn_slot()
{
    QClipboard *board = QApplication::clipboard();
    QPixmap pix = screen->getGrabPixmap();
    board->setImage(pix.toImage());
    cancelBtn_slot();
}

/**
 * 保存Screen类的引用
 * @param screen
 */
void ControlWidget::setScreenQuote(Screen *screen)
{
    this->screen = screen;
}

ControlWidget::~ControlWidget()
{
    delete ui;
}
