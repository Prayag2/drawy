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

#include <QJsonArray>
#include <QJsonObject>

class Item;
class Property;
class ApplicationContext;

class Serializer {
public:
    Serializer();

    void serialize(ApplicationContext *context);
    void saveToFile();

private:
    static QJsonObject toJson(const QRectF &rect);
    static QJsonObject toJson(const QPointF &point);
    static QJsonObject toJson(std::shared_ptr<Item> item);
    static QJsonObject toJson(const Property &property);

    template <typename T>
    static QJsonValue toJson(const T &val) {
        return QJsonValue(val);
    };

    template <typename T>
    static QJsonArray toJson(const QVector<T> &vector) {
        QJsonArray result{};

        for (const T &el : vector) {
            result.push_back(toJson(el));
        }

        return result;
    }

private:
    // properties
    QJsonObject m_object;
};
