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

#include "event.hpp"

Event::Event() {
}

void Event::setPos(const QPoint &point, qreal const scale) {
    m_pos = point * scale;
}

void Event::setButton(Qt::MouseButton btn) {
    m_button = btn;
}

void Event::setPressure(qreal pressure) {
    m_pressure = pressure;
}

qreal Event::pressure() const {
    return m_pressure;
}

const QPoint &Event::pos() const {
    return m_pos;
};

Qt::MouseButton Event::button() const {
    return m_button;
}

QString Event::text() const {
    return m_text;
}

int Event::key() const {
    return m_key;
}

Qt::KeyboardModifiers Event::modifiers() const {
    return m_modifiers;
}

void Event::setKey(int key) {
    m_key = key;
}

void Event::setText(const QString &text) {
    m_text = text;
}

void Event::setModifiers(Qt::KeyboardModifiers modifiers) {
    m_modifiers = modifiers;
}
