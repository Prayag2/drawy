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

#include <QTimer>
#include <QWidget>
class Canvas;
class ApplicationContext;
class PropertyManager;

class RenderingContext : public QObject {
    Q_OBJECT

public:
    RenderingContext(ApplicationContext *context);
    ~RenderingContext();

    void setRenderingContext();

    Canvas &canvas() const;
    QPainter &canvasPainter() const;
    QPainter &overlayPainter() const;

    void markForRender();
    void markForUpdate();
    void markForUpdate(const QRect &region);

    qreal zoomFactor() const;
    void setZoomFactor(qreal newValue);
    void updateZoomFactor(qreal diff, QPoint center = {-1, -1});

    const int fps() const;

    void reset();

private slots:
    void beginPainters();
    void endPainters();
    void canvasResized();

private:
    Canvas *m_canvas{nullptr};
    QPainter *m_canvasPainter{};
    QPainter *m_overlayPainter{};

    QTimer m_frameTimer;

    bool m_needsReRender{false};
    bool m_needsUpdate{false};
    QRect m_updateRegion{};

    qreal m_zoomFactor{1};

    ApplicationContext *m_applicationContext;
};
