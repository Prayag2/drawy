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

#include "line.hpp"

#include "../common/utils/math.hpp"

LineItem::LineItem() {
}

void LineItem::m_draw(QPainter &painter, const QPointF &offset) const {
    painter.drawLine(start() - offset, end() - offset);
}

bool LineItem::intersects(const QRectF &rect) {
    return Common::Utils::Math::intersects(rect, QLineF{start(), end()});
};

bool LineItem::intersects(const QLineF &line) {
    return Common::Utils::Math::intersects(QLineF{start(), end()}, line);
}

Item::Type LineItem::type() const {
    return Item::Line;
}
