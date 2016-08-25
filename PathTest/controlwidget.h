#ifndef CONTROLWIDGET_H
#define CONTROLWIDGET_H

#include <QWidget>

namespace Ui {
class ControlWidget;
}

class Screen;

//模仿qq那种控制面板
class ControlWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ControlWidget(QWidget *parent = 0);

    void connectSignalAndSlot();

    void setScreenQuote(Screen *screen);

    ~ControlWidget();

public slots:
    void cancelBtn_slot();

    void saveBtn_slot();

    void finishBtn_slot();

private:
    Ui::ControlWidget *ui;
    Screen *screen;
};

#endif // CONTROLWIDGET_H
