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

#include "freeform.hpp"

#include <QDateTime>
#include <memory>

#include "../common/constants.hpp"
#include "../common/utils/math.hpp"

FreeformItem::FreeformItem() {
    m_properties[Property::StrokeWidth] = Property{1, Property::StrokeWidth};
    m_properties[Property::StrokeColor] = Property{QColor(Qt::black), Property::StrokeColor};
    m_properties[Property::Opacity] = Property{Common::maxItemOpacity, Property::Opacity};
}

int FreeformItem::minPointDistance() {
    return 0;
}

void FreeformItem::addPoint(const QPointF &point, const qreal pressure, bool optimize) {
    QPointF newPoint{point};
    if (optimize) {
        newPoint = optimizePoint(point);
    }
    double x = newPoint.x(), y = newPoint.y();

    m_boundingBox = m_boundingBox.normalized();
    double topLeftX{m_boundingBox.topLeft().x()}, topLeftY{m_boundingBox.topLeft().y()};
    double bottomRightX{m_boundingBox.bottomRight().x()},
        bottomRightY{m_boundingBox.bottomRight().y()};
    int mg{property(Property::StrokeWidth).value<int>()};

    if (m_points.size() <= 1) {
        m_boundingBox.setTopLeft({x - mg, y - mg});
        m_boundingBox.setBottomRight({x + mg, y + mg});
    } else {
        m_boundingBox.setLeft(std::min(topLeftX, x - mg));
        m_boundingBox.setTop(std::min(topLeftY, y - mg));
        m_boundingBox.setRight(std::max(bottomRightX, x + mg));
        m_boundingBox.setBottom(std::max(bottomRightY, y + mg));
    }

    m_points.push_back(newPoint);
    m_pressures.push_back(pressure);
}

bool FreeformItem::intersects(const QRectF &rect) {
    if (!boundingBox().intersects(rect))
        return false;

    qsizetype pointsSize{m_points.size()};
    if (pointsSize == 1) {
        return rect.contains(m_points[0]);
    }

    QPointF p{rect.topLeft()};
    QPointF q{rect.topRight()};
    QPointF r{rect.bottomRight()};
    QPointF s{rect.bottomLeft()};

    for (qsizetype idx{0}; idx < pointsSize - 1; idx++) {
        QLine l{m_points[idx].toPoint(), m_points[idx + 1].toPoint()};

        if (Common::Utils::Math::intersects(l, QLineF{p, q}) ||
            Common::Utils::Math::intersects(l, QLineF{q, r}) ||
            Common::Utils::Math::intersects(l, QLineF{r, s}) ||
            Common::Utils::Math::intersects(l, QLineF{s, q}) ||
            rect.contains(m_points[idx].toPoint()) || rect.contains(m_points[idx + 1].toPoint()))
            return true;
    }

    return false;
}

bool FreeformItem::intersects(const QLineF &line) {
    qsizetype pointSize{m_points.size()};
    for (qsizetype index{1}; index < pointSize; index++) {
        if (Common::Utils::Math::intersects(QLineF{m_points[index - 1], m_points[index]}, line)) {
            return true;
        }
    }
    return false;
}

void FreeformItem::draw(QPainter &painter, const QPointF &offset) {
    QPen pen{};

    QColor color{property(Property::StrokeColor).value<QColor>()};
    int alpha{property(Property::Opacity).value<int>()};
    color.setAlpha(alpha);

    pen.setJoinStyle(Qt::RoundJoin);
    pen.setCapStyle(Qt::RoundCap);
    pen.setWidth(property(Property::StrokeWidth).value<int>());
    pen.setColor(color);

    painter.setPen(pen);
    painter.setRenderHints(QPainter::SmoothPixmapTransform | QPainter::Antialiasing);

    m_draw(painter, offset);
}

void FreeformItem::erase(QPainter &painter, const QPointF &offset, QColor color) const {
    painter.setCompositionMode(QPainter::CompositionMode_Source);
    painter.fillRect(boundingBox().translated(-offset), Qt::transparent);
    painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
}

QPointF FreeformItem::optimizePoint(const QPointF &newPoint) {
    m_currentWindow.push_back(newPoint);
    m_currentWindowSum += newPoint;

    if (m_currentWindow.size() > m_bufferSize) {
        m_currentWindowSum -= m_currentWindow.front();
        m_currentWindow.pop_front();
    }

    return m_currentWindowSum / m_currentWindow.size();
}

void FreeformItem::quickDraw(QPainter &painter, const QPointF &offset) const {
    QPen pen{};

    QColor color{property(Property::StrokeColor).value<QColor>()};
    int alpha{property(Property::Opacity).value<int>()};
    color.setAlpha(alpha);

    qreal penWidth{property(Property::StrokeWidth).value<qreal>()};
    if (alpha == Common::maxItemOpacity) {
        penWidth *= m_pressures.back();
    }

    pen.setJoinStyle(Qt::RoundJoin);
    pen.setCapStyle(Qt::RoundCap);
    pen.setColor(color);
    pen.setWidthF(penWidth);
    painter.setPen(pen);

    if (m_points.size() > 1) {
        painter.drawLine(m_points[m_points.size() - 2] - offset, m_points.back() - offset);
    } else {
        painter.drawPoint(m_points.back());
    }
}

void FreeformItem::m_draw(QPainter &painter, const QPointF &offset) const {
    int strokeWidth{property(Property::StrokeWidth).value<int>()};
    int alpha{property(Property::Opacity).value<int>()};
    double currentWidth{strokeWidth * 1.0};

    // Intersection points are visible on translucent pressure sensitive strokes
    // So I've disabled the use of pressure senstivity when opacity is not max,
    // for now
    bool canUsePressureSenstivity{alpha == Common::maxItemOpacity};
    if (!canUsePressureSenstivity) {
        painter.save();
        painter.translate(-offset);
        painter.drawPolyline(m_points);
        painter.restore();
        return;
    }

    qsizetype pointSize{m_points.size()};
    for (qsizetype index = 0; index < pointSize; index++) {
        double newWidth{strokeWidth * m_pressures[index]};

        if (abs(newWidth - currentWidth) >= 1e-3) {
            QPen pen{painter.pen()};
            pen.setWidthF(newWidth);
            painter.setPen(pen);

            currentWidth = newWidth;
        }

        if (index == 0) {
            painter.drawPoint(m_points.front() - offset);
        } else {
            painter.drawLine(m_points[index - 1] - offset, m_points[index] - offset);
        }
    }
}

int FreeformItem::size() const {
    return m_points.size();
}

int FreeformItem::maxSize() const {
    // Max number of points per freeform
    return 500;
}

// If the number of points exceeds the limit, this method can be called
// to split this freeform into multiple smaller freeforms
QVector<std::shared_ptr<Item>> FreeformItem::split() const {
    QVector<std::shared_ptr<Item>> items;

    qsizetype pointSize{m_points.size()};
    for (qsizetype index = 0; index < pointSize; index++) {
        if (index % maxSize() == 0) {
            // add this point to the previous freeform too
            if (!items.empty()) {
                std::shared_ptr<FreeformItem> last{
                    std::static_pointer_cast<FreeformItem>(items.back())};
                last->addPoint(m_points[index], m_pressures[index]);
            }

            // create a copy
            std::shared_ptr<FreeformItem> newItem{std::make_shared<FreeformItem>()};
            newItem->m_properties = m_properties;
            newItem->m_boundingBoxPadding = m_boundingBoxPadding;

            items.push_back(newItem);
        }
        std::shared_ptr<FreeformItem> cur{std::static_pointer_cast<FreeformItem>(items.back())};
        cur->addPoint(m_points[index], m_pressures[index], false);
    }

    return items;
}

void FreeformItem::translate(const QPointF &amount) {
    for (QPointF &point : m_points) {
        point += amount;
    }

    m_boundingBox.translate(amount);
};

Item::Type FreeformItem::type() const {
    return Item::Freeform;
}

const QVector<QPointF> &FreeformItem::points() const {
    return m_points;
}

const QVector<qreal> &FreeformItem::pressures() const {
    return m_pressures;
}
