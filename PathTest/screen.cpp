#include "screen.h"
#include <QPainter>
#include <QMouseEvent>
#include <QPainterPath>
#include <QPoint>
#include <QDebug>
#include <QScreen>
#include <QGuiApplication>
#include <QRectF>
#include "controlwidget.h"
#include <QHBoxLayout>
#include <QPushButton>
#include <QTime>

Screen::Screen(QWidget *parent)
    : QWidget(parent),
      widthInfoRect(QRectF(0, 0, 0, 0)),
      control(NULL)
{
    showFullScreen();
    //截取全屏
    QScreen *scrPix = QGuiApplication::primaryScreen();
    pixmap = scrPix->grabWindow(0);
    //全局路径的初始化，一个全屏闭合回路
    globalPath.lineTo(pixmap.width(), 0);
    globalPath.lineTo(pixmap.width(), pixmap.height());
    globalPath.lineTo(0, pixmap.height());
    globalPath.lineTo(0, 0);

    oncePress = true;

    setMouseTracking(true);
    type = NO;

}

void Screen::paintEvent(QPaintEvent *e)
{

    QPainter paint(this);
    paint.drawPixmap(0, 0, pixmap);
    //初始化画笔操作
    paint.setPen(Qt::blue);
    paint.setBrush(QColor(0, 0, 0, 100));
    //设置路径
    QPainterPath path = getPath();
    //addRect(path);
    paint.drawPath(path);
    //绘制选取左上角显示宽高的区域
    widthInfoRect.drawMe(paint);
    //绘制四个小正方形
    drawControlArea(paint);

}
//绘制正方形
void Screen::drawControlArea(QPainter &painter)
{
    //计算四个小正方形
    rect1.setX(movePoint.x() - 3);
    rect1.setY(pressedPoint.y() - 3);
    rect1.setWidth(6);
    rect1.setHeight(6);
    rect2.setX(pressedPoint.x() - 3);
    rect2.setY(pressedPoint.y() - 3);
    rect2.setWidth(6);
    rect2.setHeight(6);
    rect3.setX(pressedPoint.x() - 3);
    rect3.setY(movePoint.y() - 3);
    rect3.setWidth(6);
    rect3.setHeight(6);
    rect4.setX(movePoint.x() - 3);
    rect4.setY(movePoint.y() - 3);
    rect4.setWidth(6);
    rect4.setHeight(6);
    painter.save();
    painter.setBrush(Qt::blue);
    painter.drawRect(rect1);
    painter.drawRect(rect2);
    painter.drawRect(rect3);
    painter.drawRect(rect4);
    painter.restore();
}



//得到选区之外的路径
QPainterPath Screen::getPath()
{
    //选取路径
    QPainterPath path;
    path.moveTo(pressedPoint.x(), pressedPoint.y());
    path.lineTo(movePoint.x(), pressedPoint.y());
    path.lineTo(movePoint.x(), movePoint.y());
    path.lineTo(pressedPoint.x(), movePoint.y());
    path.lineTo(pressedPoint.x(), pressedPoint.y());
    return globalPath.subtracted(path);
}


void Screen::mousePressEvent(QMouseEvent *e)
{
    if( e->button() & Qt::LeftButton && oncePress)
    {
       pressedPoint = e->pos();
    } else
    {
        oldPoint = e->pos();
    }
}

void Screen::mouseReleaseEvent(QMouseEvent *e)
{
    oncePress = false;
    if( !control )
    {
qDebug() << "new ControlWidget";
        control = new QWidget(this);
        controlUi = new ControlWidget(control);
        QHBoxLayout *layout = new QHBoxLayout(control);
        layout->addWidget(controlUi);
        layout->setContentsMargins(0, 0, 0, 0);
        control->setObjectName("control");
        control->setStyleSheet("QWidget#control{\
                               background-color: #eaecf0;}");
        controlUi->setScreenQuote(this);
    }
    //设置控制面板的位置
    control->setGeometry(movePoint.x() - 543, movePoint.y() + 6, 543, 25);
    control->show();
}

void Screen::mouseMoveEvent(QMouseEvent *e)
{
    QPoint tempPoint = e->pos();
    if(e->buttons() & Qt::LeftButton)
    {
        //选取左上角的信息更改
        int x = qAbs(movePoint.x() - pressedPoint.x());
        int y = qAbs(movePoint.y() - pressedPoint.y());
        widthInfoRect.setText(tr("%1 * %2").arg(x).arg(y));
        if( comparePoint(pressedPoint, movePoint)) {
            widthInfoRect.setLocation(pressedPoint.x(), pressedPoint.y());
            //计算选区矩形
            rect.setX(pressedPoint.x());
            rect.setY(pressedPoint.y());
            rect.setWidth(movePoint.x() - pressedPoint.x());
            rect.setHeight(movePoint.y() - pressedPoint.y());
        } else {
            widthInfoRect.setLocation(movePoint.x(), movePoint.y());
            rect.setX(movePoint.x());
            rect.setY(movePoint.y());
            rect.setWidth(pressedPoint.x() - movePoint.x());
            rect.setHeight(pressedPoint.y() - movePoint.y());
        }
        if( oncePress )
        {
            movePoint = e->pos();
        //此时的移动代表选区的变化
        } else
        {
            if( control )
                control->hide();
            int moveX = tempPoint.x() - oldPoint.x();
            int moveY = tempPoint.y() - oldPoint.y();
            switch( type )
            {
            case RECT1:
                pressedPoint.setY(pressedPoint.y() + moveY);
                movePoint.setX(movePoint.x() + moveX);
                break;
            case RECT2:
                pressedPoint.setX(pressedPoint.x() + moveX);
                pressedPoint.setY(pressedPoint.y() + moveY);
                break;
            case RECT3:
                pressedPoint.setX(pressedPoint.x() + moveX);
                movePoint.setY(movePoint.y() + moveY);
                break;
            case RECT4:
                movePoint.setX(movePoint.x() + moveX);
                movePoint.setY(movePoint.y() + moveY);
                break;
            case RECT:
            {
                int tempPressX = pressedPoint.x() + moveX;
                int tempPressY = pressedPoint.y() + moveY;
                int tempMoveX = movePoint.x() + moveX;
                int tempMoveY = movePoint.y() + moveY;
                int deskWidth = pixmap.width();
                int deskHeight = pixmap.height();
                if( tempPressX < 0 )
                {
                    tempPressX = 0;
                    tempMoveX = movePoint.x();
                }
                if( tempPressX > deskWidth)
                {
                    tempPressX = deskHeight;
                }
                if(tempPressY < 0)
                {
                    tempPressY = 0;
                    tempMoveY = movePoint.y();
                }
                if( tempPressY > deskHeight)
                {

                    tempPressY = deskHeight;
                }
                if( tempMoveX < 0)
                {
                    tempMoveX = 0;
                }
                if( tempMoveX > deskWidth)
                {
                    tempMoveX = deskWidth;
                    tempPressX = pressedPoint.x();
                }
                if( tempMoveY < 0)
                {
                    tempMoveY = 0;
                }
                if( tempMoveY > deskHeight)
                {
                    tempMoveY = deskHeight;
                    tempPressY = pressedPoint.y();

                }
                pressedPoint.setX(tempPressX);
                pressedPoint.setY(tempPressY);
                movePoint.setX(tempMoveX);
                movePoint.setY(tempMoveY);
                break;
            }
            case NO:
                break;
            default:
                break;
            }
            oldPoint = tempPoint;
        }

    } else
    {
        Type r = pointInWhere(e->pos());
        if( r == RECT1)
        {
            type = RECT1;
            setCursor(Qt::SizeBDiagCursor);
        } else if( r == RECT2)
        {
            type = RECT2;
            setCursor(Qt::SizeFDiagCursor);
        } else if( r == RECT3)
        {
            type = RECT3;
            setCursor(Qt::SizeBDiagCursor);
        } else if( r == RECT4)
        {
            type = RECT4;
            setCursor(Qt::SizeFDiagCursor);
        } else if( r == RECT)
        {
            type = RECT;
            setCursor(Qt::SizeAllCursor);
        } else
        {
            type = NO;
            setCursor(Qt::ArrowCursor);
        }
    }
    repaint();
}

bool Screen::compareRect(QRectF &r1, QRectF &r2)
{
    return false;
}

//计算此时鼠标移动在哪个选区上面
Type Screen::pointInWhere(QPoint p)
{
    if( pointInRect(p, rect1))
        return RECT1;
    else if( pointInRect(p, rect2))
        return RECT2;
    else if( pointInRect(p, rect3))
        return RECT3;
    else if( pointInRect(p, rect4))
        return RECT4;
    else if( pointInRect(p, rect))
        return RECT;
    else
        return NO;

}

/**
 * 判断点是否在矩形内
 */
bool Screen::pointInRect(QPoint &p, QRectF &r)
{
    if( p.x() > r.x() && p.x() < r.x() + r.width() &&
            p.y() > r.y() && p.y() < r.y() + r.height())
        return true;
    return false;
}

void Screen::keyPressEvent(QKeyEvent *e)
{
    int key = e->key();
    switch( key )
    {
    case Qt::Key_Escape :
        close();
        break;
    case Qt::Key_Enter:
        if( controlUi )
        {
            controlUi->finishBtn_slot();
        }
        break;
    case Qt::Key_Return :
        if( controlUi )
        {
            controlUi->finishBtn_slot();
        }
        break;
    default:
        break;
    }
}

/**
 * 如果p1的x和y坐标都小于p2的x和y坐标返回true
 * @param p1
 * @param p2
 * @return
 */
bool Screen::comparePoint(QPoint &p1, QPoint &p2)
{
    if(p1.x() < p2.x() && p1.y() < p2.y())
        return true;
    return false;
}

/**
 * 保存截取出来的图片
 */
void Screen::savePixmap()
{
    //生成图片名称
    QString picName = "小万截图";
    QTime time;
    //获取当前系统时间，用做伪随机数的种子
    time = QTime::currentTime();
    qsrand(time.msec() + time.second() * 1000);
    //随机字符串
    QString randStr;
    randStr.setNum(qrand());
    picName.append(randStr);
    picName.append(".jpg");
qDebug() << "picName:" << picName << "qrand:" << qrand();
    QPixmap pix = pixmap.copy(pressedPoint.x(), pressedPoint.y(), movePoint.x() - pressedPoint.x(),
                              movePoint.y() - pressedPoint.y());
    pix.save(picName, "JPG");
}

QPixmap Screen::getGrabPixmap()
{
    return pixmap.copy(pressedPoint.x(), pressedPoint.y(), movePoint.x() - pressedPoint.x(),
                                        movePoint.y() - pressedPoint.y());
}

Screen::~Screen()
{
   // delete control;
}
