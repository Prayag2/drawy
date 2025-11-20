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

#pragma once

#include <QPainter>
#include <QWidget>

class Canvas : public QWidget {
    Q_OBJECT

public:
    explicit Canvas(QWidget *parent = nullptr);
    ~Canvas();

    QPixmap *const canvas() const;
    QPixmap *const overlay() const;
    QPixmap *const widget() const;
    QSize sizeHint() const override;
    QSize dimensions() const;

    QColor bg() const;
    void setBg(const QColor &color, QPixmap *canvas = nullptr, QPixmap *overlay = nullptr);

    qreal scale() const;
    void setScale(const qreal scale);

signals:
    void mousePressed(QMouseEvent *event);
    void mouseMoved(QMouseEvent *event);
    void mouseReleased(QMouseEvent *event);
    void keyPressed(QKeyEvent *event);
    void keyReleased(QKeyEvent *event);
    void inputMethodInvoked(QInputMethodEvent *event);
    void tablet(QTabletEvent *event);
    void wheel(QWheelEvent *event);
    void resizeEventCalled();
    void resizeStart();
    void resizeEnd();
    void destroyed();
    void leave(QEvent *event);

public slots:
    void triggerUpdate();

protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void inputMethodEvent(QInputMethodEvent *event) override;
    void tabletEvent(QTabletEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    bool event(QEvent *event) override;
    void leaveEvent(QEvent *event) override;

private:
    qreal m_scale{1.0};  // default scale is 1
    QPixmap *m_canvas{};
    QPixmap *m_overlay{};
    QPixmap *m_widget{};
    QColor m_bg{};

    QSize m_sizeHint{500, 500};
    QSize m_maxSize{};
    // const QPixmap::Format m_imageFormat{QPixmap::Format_ARGB32_Premultiplied};

    static QByteArray imageData(QPixmap *const img);
    static void setImageData(QPixmap *const img, const QByteArray &arr);
    void resize();
};
