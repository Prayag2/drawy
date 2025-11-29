/*
 * Drawy - A simple brainstorming tool with an infinite canvas
 * Copyright (C) 2025 - Prayag Jain <prayagjain2@gmail.com>
 *
 * Authors:
 * 1. Prayag Jain <prayagjain2@gmail.com>
 * 2. quarterstar <quarterstar@proton.me>
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

#include <QDateTime>
#include <QMouseEvent>
#include <QObject>
#include <memory>
#include <optional>

#include "../data-structures/uniquedeque.hpp"
#include "../tools/tool.hpp"
#include "action.hpp"
#include "keypress.hpp"

class ApplicationContext;

namespace {
static constexpr qint64 DEFAULT_KEY_INPUT_THRESHOLD_MS = 100;
}

class Controller : public QObject {
    Q_OBJECT

public:
    Controller(QObject *parent = nullptr);
    ~Controller();

public slots:
    void initialize();
    void mousePressed(QMouseEvent *event);
    void mouseMoved(QMouseEvent *event);
    void mouseReleased(QMouseEvent *event);
    void mouseDoubleClick(QMouseEvent *event);
    void mouseTripleClick(QMouseEvent *event);
    void keyPressed(QKeyEvent *event);
    void keyReleased(QKeyEvent *event);
    void anyPressed(std::variant<QKeyEvent *, QMouseEvent *> input);
    void anyReleased(std::variant<QKeyEvent *, QMouseEvent *> input);
    void inputMethodInvoked(QInputMethodEvent *event);
    void tablet(QTabletEvent *event);
    void wheel(QWheelEvent *event);
    void leave(QEvent *event);

private:
    ApplicationContext *m_context{};
    qint64 m_lastTime{};
    qint64 m_lastClickTime{};
    size_t m_clickCount{};  // for double/triple clicks

    bool m_mouseMoved{false};

    std::vector<ActionFacade> m_actions{};

    UniqueDeque<KeyPress> m_keyBuffer{};
    UniqueDeque<KeyPress> m_prevKeyBuffer{};
    UniqueDeque<KeyPress> m_occupationKeyBuffer{};

    Qt::Key m_lastKeyPress{};
    Qt::MouseButton m_lastMousePress{};

    // The key buffer system accepts a sequence of inputs if they are either
    // in the same insertion order as the one they were defined as or the
    // time difference between the key presses is negligible. For example,
    // the user may intrinsically try to move around with Space + Left Click
    // but press Left Click a few milliseconds before Space. In that case, the
    // user's input will still be accepted.
    bool satisfiesKeys(std::vector<std::variant<Qt::Key, Qt::MouseButton>> &required,
                       std::optional<qint64> maxGapMs = DEFAULT_KEY_INPUT_THRESHOLD_MS);

    TaskContext m_taskCtx;
};
