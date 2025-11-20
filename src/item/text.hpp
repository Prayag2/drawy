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

#include <QPainter>
#include <QRect>

#include "item.hpp"

class TextItem : public Item {
public:
    TextItem();
    ~TextItem();

    bool intersects(const QRectF &rect) override;
    bool intersects(const QLineF &rect) override;

    void draw(QPainter &painter, const QPointF &offset) override;

    void translate(const QPointF &amount) override;

    void createTextBox(const QPointF position);

    enum Mode { EDIT, NORMAL };

    Mode mode() const;
    void setMode(Mode mode);

    int getLineFromY(double yPos) const;
    qsizetype getIndexFromX(double xPos, int lineNumber) const;

    qsizetype caret() const;
    void setCaret(qsizetype index, bool updatePosInLine = true);
    void setCaret(const QPointF &cursorPos);
    qsizetype caretPosInLine() const;

    qsizetype selectionStart() const;
    qsizetype selectionEnd() const;
    void setSelectionStart(qsizetype index);
    void setSelectionEnd(qsizetype index);
    const QString selectedText() const;

    const QString &text() const;
    void insertText(const QString &text);
    void deleteSubStr(int start, int end);
    void deleteSelection();

    bool hasSelection() const;

    std::pair<qsizetype, qsizetype> getLineRange(int lineNumber) const;
    std::pair<qsizetype, qsizetype> getLineRange(qsizetype position) const;

    qsizetype getPrevBreak(qsizetype pos) const;
    qsizetype getNextBreak(qsizetype pos) const;

    Item::Type type() const override;

    constexpr static int INVALID{-1};

    void updateAfterProperty() override;

protected:
    void m_draw(QPainter &painter, const QPointF &offset) const override;

private:
    QFont getFont() const;
    QPen getPen() const;

    static QTextOption getTextOptions();
    constexpr static int getTextFlags();

    QString m_text;

    void renderBoundingBox(QPainter &painter) const;
    void updateBoundingBox();

    qsizetype m_caretIndex{};
    qsizetype m_selectionStart{};
    qsizetype m_selectionEnd{};
    qsizetype m_caretPosInLine{};
    Mode m_mode;
};
