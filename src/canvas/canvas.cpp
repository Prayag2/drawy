/*
 * Drawy - A simple brainstorming tool with an infinite canvas
 * Copyright (C) 2025 - Prayag Jain <prayagjain2@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "canvas.hpp"

#include <QBuffer>
#include <QResizeEvent>
#include <QScreen>

// PUBLIC
Canvas::Canvas(QWidget *parent) : QWidget{parent} {
    m_sizeHint = screen()->size() * m_scale;
    m_maxSize = m_sizeHint;

    m_canvas = new QPixmap(m_sizeHint);
    m_overlay = new QPixmap(m_sizeHint);

    setBg(QColor{18, 18, 18});

    setTabletTracking(true);
    setMouseTracking(true);
    setAttribute(Qt::WA_InputMethodEnabled);

    setFocusPolicy(Qt::ClickFocus);
}

Canvas::~Canvas() {
    emit destroyed();

    delete m_canvas;
    delete m_overlay;
}

QSize Canvas::sizeHint() const {
    return m_sizeHint;
}

QPixmap *const Canvas::canvas() const {
    return m_canvas;
}

QPixmap *const Canvas::overlay() const {
    return m_overlay;
}

QPixmap *const Canvas::widget() const {
    return m_widget;
}

QColor Canvas::bg() const {
    return m_bg;
};

void Canvas::setBg(const QColor &color, QPixmap *canvas, QPixmap *overlay) {
    m_bg = color;
    if (canvas)
        canvas->fill(color);
    else
        m_canvas->fill(color);

    if (overlay)
        overlay->fill(Qt::transparent);
    else
        m_overlay->fill(Qt::transparent);
}

qreal Canvas::scale() const {
    return m_scale;
}

void Canvas::setScale(const qreal scale) {
    if (scale == 0 || m_scale == 0)
        return;

    m_sizeHint = (m_sizeHint * scale) / m_scale;

    if (scale > m_scale)
        m_maxSize = m_sizeHint;
    m_scale = scale;
    resize();
}

QSize Canvas::dimensions() const {
    return size() * m_scale;
}

// PROTECTED
void Canvas::paintEvent(QPaintEvent *event) {
    QPainter painter{this};
    painter.scale(1.0 / m_scale, 1.0 / m_scale);
    painter.setClipRegion(m_canvas->rect());

    if (m_canvas)
        painter.drawPixmap(0, 0, *m_canvas);
    if (m_overlay)
        painter.drawPixmap(0, 0, *m_overlay);
}

// just a small overload
bool operator<=(const QSize &a, const QSize &b) {
    return a.height() <= b.height() && a.width() <= b.width();
}

void Canvas::resizeEvent(QResizeEvent *event) {
    emit resizeEventCalled();

    setScale(devicePixelRatioF());
    if (size() * m_scale <= m_maxSize) {
        return;
    }

    resize();

    QWidget::resizeEvent(event);
}

void Canvas::mousePressEvent(QMouseEvent *event) {
    emit mousePressed(event);
    QWidget::mousePressEvent(event);
}

void Canvas::mouseMoveEvent(QMouseEvent *event) {
    emit mouseMoved(event);
    QWidget::mouseMoveEvent(event);
};

void Canvas::mouseReleaseEvent(QMouseEvent *event) {
    emit mouseReleased(event);
    QWidget::mouseReleaseEvent(event);
};

void Canvas::keyPressEvent(QKeyEvent *event) {
    emit keyPressed(event);
    QWidget::keyPressEvent(event);
}

void Canvas::keyReleaseEvent(QKeyEvent *event) {
    emit keyReleased(event);
    QWidget::keyReleaseEvent(event);
}

void Canvas::inputMethodEvent(QInputMethodEvent *event) {
    emit inputMethodInvoked(event);
    QWidget::inputMethodEvent(event);
}

void Canvas::tabletEvent(QTabletEvent *event) {
    emit tablet(event);
    QWidget::tabletEvent(event);
}

void Canvas::wheelEvent(QWheelEvent *event) {
    emit wheel(event);
    QWidget::wheelEvent(event);
}

void Canvas::leaveEvent(QEvent *event) {
    emit leave(event);
    QWidget::leaveEvent(event);
}

bool Canvas::event(QEvent *event) {
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent *ev = dynamic_cast<QKeyEvent *>(event);
        if (ev && (ev->key() == Qt::Key_Tab) || (ev->key() == Qt::Key_Backtab)) {
            emit keyPressed(ev);
            return true;
        }
    }
    return QWidget::event(event);
}

// PRIVATE
QByteArray Canvas::imageData(QPixmap *const img) {
    QByteArray arr{};
    QBuffer buffer{&arr};
    buffer.open(QBuffer::WriteOnly);
    img->save(&buffer, "PNG");
    return arr;
}

void Canvas::setImageData(QPixmap *const img, const QByteArray &arr) {
    img->loadFromData(arr, "PNG");
}

void Canvas::resize() {
    emit resizeStart();

    if (m_canvas->paintingActive() || m_overlay->paintingActive()) {
        return;
    }

    QSize oldSize{m_canvas->size()};
    QSize newSize{size() * m_scale};
    m_maxSize.setWidth(std::max(oldSize.width(), newSize.width()));
    m_maxSize.setHeight(std::max(oldSize.height(), newSize.height()));

    QPixmap *canvas{new QPixmap(m_maxSize)};
    QPixmap *overlay{new QPixmap(m_maxSize)};
    setBg(bg(), canvas, overlay);

    QPainter canvasPainter{canvas}, overlayPainter{overlay};
    canvasPainter.drawPixmap(0, 0, *m_canvas);
    overlayPainter.drawPixmap(0, 0, *m_overlay);

    delete m_canvas;
    delete m_overlay;

    m_canvas = canvas;
    m_overlay = overlay;

    canvasPainter.end();
    overlayPainter.end();
    emit resizeEnd();
}

void Canvas::triggerUpdate() {
    this->update();
}
