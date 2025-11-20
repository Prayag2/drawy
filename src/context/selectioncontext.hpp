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

#include <QWidget>
#include <unordered_set>
class Property;
class Tool;
class Item;
class ApplicationContext;

class SelectionContext : public QObject {
public:
    SelectionContext(ApplicationContext *context);
    ~SelectionContext();

    std::unordered_set<std::shared_ptr<Item>> &selectedItems();
    QRectF selectionBox() const;

    void reset();

public slots:
    void updatePropertyOfSelectedItems(Property property);

private:
    std::unordered_set<std::shared_ptr<Item>> m_selectedItems{};

    ApplicationContext *m_applicationContext;
};
