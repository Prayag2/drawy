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

#include <deque>
#include <memory>

#include "item.hpp"

class FreeformItem : public Item, public std::enable_shared_from_this<FreeformItem> {
public:
    FreeformItem();
    ~FreeformItem() = default;
    FreeformItem(const FreeformItem &freeform);

    static int minPointDistance();

    void draw(QPainter &painter, const QPointF &offset) override;
    void quickDraw(QPainter &painter, const QPointF &offset) const;

    bool intersects(const QRectF &rect) override;
    bool intersects(const QLineF &rect) override;

    void translate(const QPointF &amount) override;

    QVector<std::shared_ptr<Item>> split() const;
    int size() const;
    int maxSize() const;

    virtual void addPoint(const QPointF &point, const qreal pressure, bool optimize = true);

    Item::Type type() const override;

    const QVector<QPointF> &points() const;
    const QVector<qreal> &pressures() const;

protected:
    void m_draw(QPainter &painter, const QPointF &offset) const override;
    QVector<QPointF> m_points{};
    QVector<qreal> m_pressures{};

private:
    QPointF optimizePoint(const QPointF &newPoint);
    std::deque<QPointF> m_currentWindow;
    QPointF m_currentWindowSum{0, 0};
    int m_bufferSize{7};
};
