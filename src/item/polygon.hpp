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

#include "item.hpp"

class PolygonItem : public Item {
public:
    PolygonItem();

    virtual void setStart(QPointF start);
    virtual void setEnd(QPointF end);

    void draw(QPainter &painter, const QPointF &offset) override;
    void erase(QPainter &painter, const QPointF &offset) const override;

    void translate(const QPointF &amount) override;

    const QPointF &start() const;
    const QPointF &end() const;

private:
    QPointF m_start{};
    QPointF m_end{};

    void m_updateBoundingBox();
};
