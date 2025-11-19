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

#include <QLineF>
#include <QRectF>

namespace Common::Utils::Math {
inline int orientation(QPointF a, QPointF b, QPointF c) {
    QPointF ab{b.x() - a.x(), b.y() - a.y()};
    QPointF ac{c.x() - a.x(), c.y() - a.y()};

    int orient{static_cast<int>(ab.x() * ac.y() - ac.x() * ab.y())};
    return (orient == 0 ? 0 : (orient < 0 ? -1 : 1));
}

inline bool intersects(const QRectF &rectA, const QRectF &rectB) {
    return rectA.intersects(rectB);
}

inline bool intersects(const QLineF &a, const QLineF &b) {
    QPointF p{a.p1()}, q{a.p2()};
    QPointF r{b.p1()}, s{b.p2()};
    return orientation(p, q, r) != orientation(p, q, s) &&
           orientation(r, s, p) != orientation(r, s, q);
}

inline bool intersects(const QRectF &rect, const QLineF &line) {
    QLineF left{rect.topLeft(), rect.bottomLeft()};
    QLineF top{rect.topLeft(), rect.topRight()};
    QLineF right{rect.topRight(), rect.bottomRight()};
    QLineF bottom{rect.bottomRight(), rect.bottomLeft()};

    return (intersects(line, left) || intersects(line, top) || intersects(line, right) ||
            intersects(line, bottom));
}

inline bool intersects(const QRectF &rect, const QPointF &point) {
    return rect.contains(point);
}
};  // namespace Common::Utils::Math
