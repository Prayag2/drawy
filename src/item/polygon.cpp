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

#include "polygon.hpp"

PolygonItem::PolygonItem() {
    m_properties[Property::StrokeWidth] = Property{1, Property::StrokeWidth};
    m_properties[Property::StrokeColor] = Property{QColor(Qt::black), Property::StrokeColor};
    m_properties[Property::Opacity] = Property{255, Property::Opacity};
}

void PolygonItem::setStart(QPointF start) {
    m_start = start;
    m_end = start;
    m_boundingBox = QRectF{start, start};
}

void PolygonItem::setEnd(QPointF end) {
    m_end = end;
    m_updateBoundingBox();
}

const QPointF &PolygonItem::start() const {
    return m_start;
}

const QPointF &PolygonItem::end() const {
    return m_end;
}

void PolygonItem::m_updateBoundingBox() {
    double minX{std::min(m_start.x(), m_end.x())};
    double maxX{std::max(m_start.x(), m_end.x())};
    double minY{std::min(m_start.y(), m_end.y())};
    double maxY{std::max(m_start.y(), m_end.y())};
    int w{property(Property::StrokeWidth).value<int>()};

    m_boundingBox = QRectF{QPointF{minX, maxY}, QPointF{maxX, minY}}.normalized();
    m_boundingBox.adjust(-w, -w, w, w);
}

void PolygonItem::draw(QPainter &painter, const QPointF &offset) {
    QPen pen{};

    QColor color{property(Property::StrokeColor).value<QColor>()};
    color.setAlpha(property(Property::Opacity).value<int>());

    pen.setCapStyle(Qt::RoundCap);
    pen.setJoinStyle(Qt::RoundJoin);
    pen.setWidth(property(Property::StrokeWidth).value<int>());
    pen.setColor(color);

    painter.setPen(pen);

    m_draw(painter, offset);
}

void PolygonItem::erase(QPainter &painter, const QPointF &offset) const {
    QPen pen{};

    pen.setWidth(property(Property::StrokeWidth).value<int>() * 10);
    pen.setColor(Qt::transparent);

    painter.save();
    painter.setPen(pen);
    painter.setCompositionMode(QPainter::CompositionMode_Source);

    m_draw(painter, offset);

    painter.restore();
}

void PolygonItem::translate(const QPointF &amount) {
    m_start += amount;
    m_end += amount;

    m_updateBoundingBox();
};
