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
#include <qglobal.h>

#include <QPoint>
#include <QString>

class Event {
public:
    Event();
    const QPoint &pos() const;
    Qt::MouseButton button() const;
    qreal pressure() const;
    QString text() const;
    int key() const;
    Qt::KeyboardModifiers modifiers() const;

    void setPos(const QPoint &point, qreal const scale = 1.0);
    void setButton(Qt::MouseButton btn);
    void setPressure(qreal pressure);
    void setKey(int key);
    void setText(const QString &text);
    void setModifiers(Qt::KeyboardModifiers modifiers);

private:
    Qt::MouseButton m_button;
    QPoint m_pos;
    qreal m_pressure{1.0};
    QString m_text;
    int m_key;
    Qt::KeyboardModifiers m_modifiers;
};
