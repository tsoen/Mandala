#ifndef PAINTWIDGET_H
#define PAINTWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QPixmap>
#include <QPoint>
#include <QWidget>
#include <QMouseEvent>
#include <QApplication>
#include <QtMath>
#include <QDebug>
#include <QBrush>
#include <QColorDialog>
#include <QFileDialog>

class PaintWidget : public QWidget {
    Q_OBJECT

private:
    std::vector<QPixmap> maps;
    std::vector<QPixmap> undoed;
    std::vector<QColor> brushScale;

    QPixmap m_pixmap;
    QPixmap m_grid;
    QPoint m_lastPos;
    QColor m_color = Qt::blue;
    double m_penSize = 2.0;
    int m_nbParts = 1;
    bool m_multicolor = false;
    bool m_showGrid = false;
    bool m_mirror = false;
    double m_gridOpacity = 0.5;
    bool m_keepAlphaChannel = false;
    QString m_saveFilePath;
    bool m_isSaved = false;

private:
    void paintEvent(QPaintEvent *) override;
    void resizeEvent(QResizeEvent *) override;
    void mousePressEvent(QMouseEvent * ev) override;
    void mouseMoveEvent(QMouseEvent * ev) override;
    void mouseReleaseEvent(QMouseEvent * /*ev*/) override;

private:
    QPixmap clearPixmap(QPixmap & pixmap);
    void drawGrid();
    void draw(const QPoint & position);
    void drawWithPossibleRotations(QPoint & end, QPoint & start, QPainter *_painter, bool fromUser, int nbParts);

public:  
    explicit PaintWidget(QWidget *parent = nullptr);
    bool getSaveState();
    using QWidget::QWidget;

signals:
    void resized(int);
    void modified(QString);
    void saved(QString);

public slots:
    void clear();
    void undo();
    void redo();
    void changeColor();
    void setPenSize(int size);
    void changeSlices(int nb);
    void toggleMulticolor(bool toggled);
    void setGridVisibility(int checkedEnum);
    void setGridOpacity(int opacity);
    void resizeMap(int w);
    void toggleMirror(int);
    void save();
    void saveAs();
    void setKeepAlphaChannel(bool checked);
    void openFile();
};

#endif // PAINTWIDGET_H
