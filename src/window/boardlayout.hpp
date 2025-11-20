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

#include <QLayout>

class BoardLayout : public QLayout {
    Q_OBJECT
public:
    BoardLayout(QWidget *parent = nullptr);
    ~BoardLayout();

    void addItem(QLayoutItem *item) override;
    QLayoutItem *itemAt(int index) const override;
    QLayoutItem *takeAt(int index) override;
    void setGeometry(const QRect &rect) override;
    int count() const override;
    QSize sizeHint() const override;
    QSize minimumSize() const override;

    void setLeftWidget(QWidget *const item, bool pinned = false);
    void setTopWidget(QWidget *const item, bool pinned = false);
    void setRightWidget(QWidget *const item, bool pinned = false);
    void setBottomWidget(QWidget *const item, bool pinned = false);
    void setCentralWidget(QWidget *const item);
    void setMargins(int margins);

private:
    QLayoutItem *m_centralWidget{nullptr};
    QLayoutItem *m_leftWidget{nullptr};
    QLayoutItem *m_topWidget{nullptr};
    QLayoutItem *m_rightWidget{nullptr};
    QLayoutItem *m_bottomWidget{nullptr};

    std::array<QLayoutItem **, 5> m_widgets{};

    bool m_isLeftPinned{false};
    bool m_isTopPinned{false};
    bool m_isRightPinned{false};
    bool m_isBottomPinned{false};

    int m_margins{0};
};
