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

#include "rectangle.hpp"

#include <QDebug>

#include "../common/utils/math.hpp"

RectangleItem::RectangleItem() {
}

void RectangleItem::m_draw(QPainter &painter, const QPointF &offset) const {
    painter.drawRect(QRectF(start() - offset, end() - offset));
}

bool RectangleItem::intersects(const QRectF &rect) {
    if (!boundingBox().intersects(rect))
        return false;

    QRectF box{start(), end()};
    QPointF p{box.topLeft()};
    QPointF q{box.topRight()};
    QPointF r{box.bottomRight()};
    QPointF s{box.bottomLeft()};

    QPointF a{rect.topLeft()};
    QPointF b{rect.topRight()};
    QPointF c{rect.bottomRight()};
    QPointF d{rect.bottomLeft()};

    return (Common::Utils::Math::intersects(QLineF{p, q}, QLineF{a, b}) ||
            Common::Utils::Math::intersects(QLineF{p, q}, QLineF{b, c}) ||
            Common::Utils::Math::intersects(QLineF{p, q}, QLineF{c, d}) ||
            Common::Utils::Math::intersects(QLineF{p, q}, QLineF{d, a}) ||
            Common::Utils::Math::intersects(QLineF{q, r}, QLineF{a, b}) ||
            Common::Utils::Math::intersects(QLineF{q, r}, QLineF{b, c}) ||
            Common::Utils::Math::intersects(QLineF{q, r}, QLineF{c, d}) ||
            Common::Utils::Math::intersects(QLineF{q, r}, QLineF{d, a}) ||
            Common::Utils::Math::intersects(QLineF{r, s}, QLineF{a, b}) ||
            Common::Utils::Math::intersects(QLineF{r, s}, QLineF{b, c}) ||
            Common::Utils::Math::intersects(QLineF{r, s}, QLineF{c, d}) ||
            Common::Utils::Math::intersects(QLineF{r, s}, QLineF{d, a}) ||
            Common::Utils::Math::intersects(QLineF{p, s}, QLineF{a, b}) ||
            Common::Utils::Math::intersects(QLineF{p, s}, QLineF{b, c}) ||
            Common::Utils::Math::intersects(QLineF{p, s}, QLineF{c, d}) ||
            Common::Utils::Math::intersects(QLineF{p, s}, QLineF{d, a}));
};

bool RectangleItem::intersects(const QLineF &line) {
    QRectF box{start(), end()};
    return Common::Utils::Math::intersects(box, line);
}

Item::Type RectangleItem::type() const {
    return Item::Rectangle;
}
