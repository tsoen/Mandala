#include "paintwidget.h"

PaintWidget::PaintWidget(QWidget *parent) : QWidget(parent){
    setFixedSize(400, 400);
    toggleMulticolor(m_multicolor);

    QPixmap newPixmap{rect().size()};
    m_pixmap = newPixmap;
    clear();

}

void PaintWidget::paintEvent(QPaintEvent *) {
    QPainter painter{this};
    painter.drawPixmap(0, 0, m_grid);
    painter.drawPixmap(0, 0, m_pixmap);
    m_isSaved = false;

    if(m_saveFilePath.isEmpty()){
        emit modified(tr("Untitled"));
    } else {
        emit modified(m_saveFilePath);
    }
}

void PaintWidget::resizeEvent(QResizeEvent *) {

    QPixmap newPixmap{rect().size()};
    newPixmap.fill(Qt::transparent);
    QPainter painter{&newPixmap};
    painter.fillRect(newPixmap.rect(), Qt::transparent);
    painter.drawPixmap((newPixmap.width() - m_pixmap.width()) / 2, (newPixmap.height() - m_pixmap.height()) / 2, m_pixmap);
    m_pixmap = newPixmap;
    maps.push_back(m_pixmap);
    drawGrid();

    emit resized(m_pixmap.width());
}

void PaintWidget::mousePressEvent(QMouseEvent * ev) {
    m_lastPos = ev->pos();
    draw(ev->pos());
}

void PaintWidget::mouseMoveEvent(QMouseEvent * ev) {
    draw(ev->pos());
}

void PaintWidget::mouseReleaseEvent(QMouseEvent *) {
    maps.push_back(m_pixmap);
}

QPixmap PaintWidget::clearPixmap(QPixmap & pixmap){
    QPixmap newPixmap{rect().size()};
    QPainter painter{&newPixmap};
    painter.fillRect(newPixmap.rect(), Qt::white);
    painter.drawPixmap(0, 0, newPixmap);
    pixmap = newPixmap;

    return pixmap;
}

void PaintWidget::drawGrid(){
    clearPixmap(m_grid);

    QPainter painter{&m_grid};
    painter.setRenderHint(QPainter::Antialiasing);

    QPoint start(m_grid.width() / 2, -m_grid.height());
    QPoint end(m_grid.width() / 2, m_grid.height() / 2);
    drawWithPossibleRotations(start, end, &painter, false, m_nbParts);
}

void PaintWidget::draw(const QPoint & position) {
    QPoint pos(position);
    QPoint initPos(pos);
    QPainter painter{&m_pixmap};
    painter.setRenderHint(QPainter::Antialiasing);

    if(m_showGrid){
       drawGrid();
    }

    drawWithPossibleRotations(pos, m_lastPos, &painter, true, m_nbParts);

    m_lastPos = initPos;
    update();
}

void PaintWidget::drawWithPossibleRotations(QPoint & end, QPoint & start, QPainter *_painter, bool fromUser, int nbParts)
{
    // save starting positions
    QPoint initPos(end);
    QPoint initLastPos(start);

    if(m_mirror && !fromUser){
        nbParts *= 2;
    }

    for(int k = 0; k < nbParts; k++)
    {
        if(fromUser){
            _painter->setPen({brushScale[static_cast<unsigned int>(k)], m_penSize, Qt::SolidLine, Qt::RoundCap});
        } else {
            if(!m_mirror && nbParts == 1){
                continue;
            }

            if(m_mirror && (k % 2 == 1 || (m_mirror && nbParts == 2))){
                _painter->setPen({QColor(Qt::GlobalColor::gray), 3.0, Qt::SolidLine});
                _painter->setOpacity(m_gridOpacity);
            } else {
                _painter->setPen({QColor(Qt::GlobalColor::gray), 3.0, Qt::DotLine});
                _painter->setOpacity(m_gridOpacity);
            }
        }

        // save starting positions
        QPoint pos1(initPos);
        QPoint last1(initLastPos);

        // change reference
        start.setX(last1.x() - m_pixmap.width() / 2);
        start.setY(last1.y() - m_pixmap.height() / 2);

        end.setX(pos1.x() - m_pixmap.width() / 2);
        end.setY(pos1.y() - m_pixmap.height() / 2);

        // save starting positions
        QPoint pos2(end);
        QPoint last2(start);

        // rotate points
        QPoint mirrorStart(start);
        QPoint mirrorEnd(end);

        if(m_mirror && fromUser)
        {
            k++;

            mirrorStart.setX(static_cast<int>(-last2.x() * cos(2*k*M_PI/nbParts) + last2.y() * sin(2*k*M_PI/nbParts)));
            mirrorStart.setY(static_cast<int>(-last2.y() * cos(2*k*M_PI/nbParts) - last2.x() * sin(2*k*M_PI/nbParts)));

            mirrorEnd.setX(static_cast<int>(-pos2.x() * cos(2*k*M_PI/nbParts) + pos2.y() * sin(2*k*M_PI/nbParts)));
            mirrorEnd.setY(static_cast<int>(-pos2.y() * cos(2*k*M_PI/nbParts) - pos2.x() * sin(2*k*M_PI/nbParts)));

            k--;
        }

        start.setX(static_cast<int>(last2.x() * cos(2*k*M_PI/nbParts) - last2.y() * sin(2*k*M_PI/nbParts)));
        start.setY(static_cast<int>(last2.y() * cos(2*k*M_PI/nbParts) + last2.x() * sin(2*k*M_PI/nbParts)));

        end.setX(static_cast<int>(pos2.x() * cos(2*k*M_PI/nbParts) - pos2.y() * sin(2*k*M_PI/nbParts)));
        end.setY(static_cast<int>(pos2.y() * cos(2*k*M_PI/nbParts) + pos2.x() * sin(2*k*M_PI/nbParts)));

        if(m_mirror && fromUser)
        {
            mirrorStart.setX(mirrorStart.x() + m_pixmap.width() / 2);
            mirrorStart.setY(m_pixmap.height() / 2 - mirrorStart.y());

            mirrorEnd.setX(mirrorEnd.x() + m_pixmap.width() / 2);
            mirrorEnd.setY(m_pixmap.height() / 2 - mirrorEnd.y());

            _painter->drawLine(mirrorStart, mirrorEnd);
        }


        // reset reference to draw
        start.setX(start.x() + m_pixmap.width() / 2);
        start.setY(start.y() + m_pixmap.height() / 2);

        end.setX(end.x() + m_pixmap.width() / 2);
        end.setY(end.y() + m_pixmap.height() / 2);


        // draw
        _painter->drawLine(start, end);
    }
}

void PaintWidget::clear(){
    QPixmap newPixmap = clearPixmap(m_pixmap);
    m_pixmap.fill(Qt::transparent);
    //maps.clear(); // should it do this
    maps.push_back(m_pixmap);
    update();
}

void PaintWidget::undo(){
    if(maps.size() > 1){
        m_pixmap = maps[static_cast<unsigned int>(std::max(0, static_cast<int>(maps.size()-2)))];
        undoed.push_back(maps[maps.size()-1]);
        maps.erase(maps.begin() + static_cast<int>(maps.size()-1));

        resizeMap(m_pixmap.width());
    }
    update();
}

void PaintWidget::redo(){
    if(undoed.size() > 0){
        m_pixmap = undoed[undoed.size()-1];
        maps.push_back(m_pixmap);
        undoed.erase(undoed.begin() + static_cast<int>(undoed.size()-1));

        resizeMap(m_pixmap.width());
    }
    update();
}

void PaintWidget::changeColor(){
    QColorDialog * colorDialog = new QColorDialog;
    QColor c = colorDialog->getColor();
    if(c.isValid()){
        m_color = c;
        toggleMulticolor(m_multicolor);
    }
}

void PaintWidget::setPenSize(int size){
    m_penSize = size;
}
void PaintWidget::changeSlices(int nb){
    m_nbParts = nb;
    toggleMulticolor(m_multicolor);
    if(m_showGrid){
       drawGrid();
    }
    update();
}

void PaintWidget::toggleMulticolor(bool toggled){
    m_multicolor = toggled;

    brushScale.clear();
    double h;
    int h1, s, v, a;
    double golden_ratio = 0.618033988749895;

    for (int i = 0; i < m_nbParts; i++)
    {
        h = golden_ratio * i * 360/m_nbParts;

        m_color.getHsv(&h1, &s, &v, &a);

        if(toggled){
            h1 = (h1 + static_cast<int>(h)) % 360;
        }

        brushScale.push_back(QColor::fromHsv(h1, s, v, a));
    }
}

void PaintWidget::setGridVisibility(int checkedEnum){
    m_showGrid = checkedEnum;

    if(m_showGrid){
       drawGrid();
    }else {
        clearPixmap(m_grid);
    }

    update();
}

void PaintWidget::setGridOpacity(int opacity){
    m_gridOpacity = opacity / 10.0;
    if(m_showGrid){
        drawGrid();
    }
    update();
}

void PaintWidget::resizeMap(int w){
    resize (w, w);

    m_pixmap = m_pixmap.scaled(w, w, Qt::KeepAspectRatio);

    setFixedSize(w, w);
}

void PaintWidget::toggleMirror(int checkedEnum){
    m_mirror = checkedEnum;
    drawGrid();
    update();
}

void PaintWidget::saveAs(){

    if(m_keepAlphaChannel){
        QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), QDir::home().absolutePath(), tr("Images (*.png)"));

        if(!fileName.isEmpty()){
            m_saveFilePath = fileName;
        }

        m_pixmap.save(m_saveFilePath);

    } else {
        QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), QDir::home().absolutePath(), tr("Images (*.png *.gif *.jpg)"));

        if(!fileName.isEmpty()){
            m_saveFilePath = fileName;
        }

        QPixmap newPixmap{rect().size()};
        QPainter painter{&newPixmap};
        painter.fillRect(newPixmap.rect(), Qt::white);
        painter.drawPixmap(0, 0, m_pixmap);
        painter.drawPixmap(0, 0, newPixmap);

        newPixmap.save(m_saveFilePath);
    }

    if(!m_saveFilePath.isEmpty()){
        m_isSaved = true;
        emit saved(m_saveFilePath);
    }
}

void PaintWidget::save(){

    if(m_saveFilePath.isEmpty()){
        saveAs();
    }
    else
    {
        m_pixmap.save(m_saveFilePath);

        m_isSaved = true;
        emit saved(m_saveFilePath);
    }  
}

void PaintWidget::setKeepAlphaChannel(bool checked){
    m_keepAlphaChannel = checked;
}

bool PaintWidget::getSaveState(){
    return m_isSaved;
}

void PaintWidget::openFile(){
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), QDir::home().absolutePath(), tr("Images (*.png *.gif *.jpg)"));

    if(!fileName.isEmpty()){
        QPixmap img(fileName);

        m_pixmap = img;

        m_saveFilePath = fileName;

        resizeMap(m_pixmap.width());

        emit saved(m_saveFilePath);
    }
}
