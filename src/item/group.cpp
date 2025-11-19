#include "group.h"
#include <stdexcept>
#include <unordered_set>

void GroupItem::draw(QPainter &painter, const QPointF &offset) {
    for (auto item : m_items) {
        item->draw(painter, offset); }
}

void GroupItem::erase(QPainter &painter, const QPointF &offset) const {
    for (auto item : m_items) {
        item->erase(painter, offset);
    }
}

void GroupItem::translate(const QPointF &amount) {
    for (auto item : m_items) {
        item->translate(amount);
    }
}

void GroupItem::group(const QVector<std::shared_ptr<Item>>& items) {
    m_items = items;
}

bool GroupItem::intersects(const QRectF &rect) {
    for (auto item : m_items) {
        if (item->intersects(rect)) {
            return true;
        }
    }

    return false;
};

bool GroupItem::intersects(const QLineF &line) {
    for (auto item : m_items) {
        if (item->intersects(line)) {
            return true;
        }
    }

    return false;
};


QVector<std::shared_ptr<Item>> GroupItem::unGroup() {
    return m_items;
}

const QRectF GroupItem::boundingBox() const {
    QRectF result{};

    for (auto item : m_items) {
        result |= item->boundingBox();
    }

    return result;
};

Item::Type GroupItem::type() const {
    return Item::Group;
}

void GroupItem::setProperty(const Property::Type propertyType, Property newObj) {
    for (auto item : m_items) {
        item->setProperty(propertyType, newObj);
    }
};

const Property GroupItem::property(const Property::Type propertyType) const {
    if (m_items.empty()) {
        throw new std::logic_error("Group does not contain any item with this property");
    }

    Property property{};
    for (auto item : m_items) {
        try {
            if (property.type() != Property::Null) {
                if (property.variant() != item->property(propertyType).variant()) {
                    return Property{};
                }
            } else {
                property = item->property(propertyType);
            }
        } catch (const std::logic_error& e) {
            // ignore
        }
    }

    return property;
};

const QVector<Property> GroupItem::properties() const {
    QVector<Property> result{};

    for (auto item : m_items) {
        result += item->properties();
    }

    return result;
};

const QVector<Property::Type> GroupItem::propertyTypes() const {
    std::unordered_set<Property::Type> types;
    for (auto item : m_items) {
        auto &itemPropertyTypes{item->propertyTypes()};
        types.insert(itemPropertyTypes.begin(), itemPropertyTypes.end());
    }

    return QVector<Property::Type>(types.begin(), types.end());
};

void GroupItem::m_draw(QPainter &painter, const QPointF &offset) const {};
