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

#include "renderingcontext.hpp"

#include <QScreen>

#include "../canvas/canvas.hpp"
#include "../common/renderitems.hpp"
#include "../data-structures/cachegrid.hpp"
#include "applicationcontext.hpp"
#include "spatialcontext.hpp"

RenderingContext::RenderingContext(ApplicationContext *context)
    : QObject{context},
      m_applicationContext(context) {
}

RenderingContext::~RenderingContext() {
    qDebug() << "Object deleted: RenderingContext";
    delete m_canvasPainter;
}

void RenderingContext::setRenderingContext() {
    m_canvas = new Canvas(m_applicationContext->parentWidget());

    m_canvasPainter = new QPainter(m_canvas->canvas());
    m_overlayPainter = new QPainter(m_canvas->overlay());

    QObject::connect(m_canvas, &Canvas::destroyed, this, &RenderingContext::endPainters);
    QObject::connect(m_canvas, &Canvas::resizeStart, this, &RenderingContext::endPainters);
    QObject::connect(m_canvas, &Canvas::resizeEnd, this, &RenderingContext::beginPainters);
    QObject::connect(m_canvas, &Canvas::resizeEventCalled, this, &RenderingContext::canvasResized);

    QObject::connect(&m_frameTimer, &QTimer::timeout, m_canvas, [&]() {
        if (m_needsReRender) {
            Common::renderCanvas(m_applicationContext);
            m_needsReRender = false;
        }

        if (m_needsUpdate) {
            if (m_updateRegion.width() == 0 || m_updateRegion.height() == 0) {
                m_canvas->update();
            } else {
                m_canvas->update(m_updateRegion);
            }

            m_updateRegion.setWidth(0);
            m_updateRegion.setHeight(0);
            m_needsUpdate = false;
        }
    });

    m_frameTimer.start(1000 / fps());
}

Canvas &RenderingContext::canvas() const {
    return *m_canvas;
}

QPainter &RenderingContext::canvasPainter() const {
    return *m_canvasPainter;
}

QPainter &RenderingContext::overlayPainter() const {
    return *m_overlayPainter;
}

// PRIVATE SLOTS
void RenderingContext::endPainters() {
    if (m_canvasPainter->isActive())
        m_canvasPainter->end();
    if (m_overlayPainter->isActive())
        m_overlayPainter->end();
}

void RenderingContext::beginPainters() {
    QPainter::RenderHints renderHints{QPainter::Antialiasing | QPainter::SmoothPixmapTransform};
    if (!m_canvasPainter->isActive()) {
        m_canvasPainter->begin(m_canvas->canvas());
        m_canvasPainter->setRenderHints(renderHints);
    }
    if (!m_overlayPainter->isActive()) {
        m_overlayPainter->begin(m_canvas->overlay());
        m_overlayPainter->setRenderHints(renderHints);
    }
}

qreal RenderingContext::zoomFactor() const {
    return m_zoomFactor;
}

void RenderingContext::updateZoomFactor(qreal diff, QPoint center) {
    // zoom out limit is 0.1
    if (diff < 0 && m_zoomFactor - 0.1 <= 1e-9)
        return;

    qreal oldZoomFactor = m_zoomFactor;
    m_zoomFactor += diff * 0.1;

    qDebug() << "Zoom: " << m_zoomFactor;

    QPointF offsetPos{m_applicationContext->spatialContext().offsetPos()};

    QSize viewport{canvas().dimensions() / oldZoomFactor};
    int width{viewport.width()};
    int height{viewport.height()};

    qreal centerX{center.x() == -1 ? width / 2.0 : center.x()};
    qreal centerY{center.y() == -1 ? height / 2.0 : center.y()};

    offsetPos.setX(offsetPos.x() + centerX * (1 - oldZoomFactor / m_zoomFactor));
    offsetPos.setY(offsetPos.y() + centerY * (1 - oldZoomFactor / m_zoomFactor));

    m_applicationContext->spatialContext().setOffsetPos(offsetPos);

    // changes scale
    endPainters();
    beginPainters();

    m_applicationContext->spatialContext().cacheGrid().markAllDirty();

    m_applicationContext->renderingContext().markForRender();
    m_applicationContext->renderingContext().markForUpdate();
}

void RenderingContext::setZoomFactor(qreal newValue) {
    m_zoomFactor = newValue;
}

const int RenderingContext::fps() const {
    QScreen *screen{m_canvas->screen()};
    if (screen) {
        return screen->refreshRate();
    }

    return 60;
}

void RenderingContext::canvasResized() {
    int width{m_canvas->dimensions().width()}, height{m_canvas->dimensions().height()};
    int cellW{CacheCell::cellSize().width()}, cellH{CacheCell::cellSize().height()};
    int rows{static_cast<int>(std::ceil(height / static_cast<double>(cellH)) + 1)};
    int cols{static_cast<int>(std::ceil(width / static_cast<double>(cellW)) + 1)};

    m_applicationContext->spatialContext().cacheGrid().setSize(9 * rows * cols);
}

void RenderingContext::markForRender() {
    m_needsReRender = true;
}

void RenderingContext::markForUpdate() {
    m_needsUpdate = true;
}

void RenderingContext::markForUpdate(const QRect &region) {
    m_needsUpdate = true;
    m_updateRegion = region;
}

void RenderingContext::reset() {
    setZoomFactor(1.0);
}
