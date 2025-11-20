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

#include <QDebug>
#include <QObject>

class Action : public QObject {
    Q_OBJECT
private:
    QString m_name;
    QString m_description;
    std::function<void()> m_callable;

public slots:
    void run();

public:
    Action(QString name, QString description, std::function<void()> callable, QObject *parent);
    QString name() const;
};

namespace std {
template <>
struct hash<Action> {
    size_t operator()(Action &action) const noexcept { return std::hash<QString>{}(action.name()); }
};
};  // namespace std
