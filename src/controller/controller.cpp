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

#include "controller.hpp"

#include <QDateTime>
#include <QWheelEvent>
#include <algorithm>
#include <array>
#include <ranges>
#include <variant>
#include <vector>

#include "../canvas/canvas.hpp"
#include "../common/constants.hpp"
#include "../common/renderitems.hpp"
#include "../common/utils/qt.hpp"
#include "../components/toolbar.hpp"
#include "../context/applicationcontext.hpp"
#include "../context/renderingcontext.hpp"
#include "../context/spatialcontext.hpp"
#include "../context/uicontext.hpp"
#include "../event/event.hpp"
#include "../tools/tool.hpp"

using namespace Common::Utils::QtUtil;

Controller::Controller(QObject *parent) : QObject{parent} {
    m_context = ApplicationContext::instance(dynamic_cast<QWidget *>(parent));
    m_context->setContexts();
}

Controller::~Controller() {
    qDebug() << "Object deleted: Controller";
}

void Controller::mousePressed(QMouseEvent *event) {
    std::variant<Qt::Key, Qt::MouseButton> keyVar{static_cast<Qt::MouseButton>(event->button())};
    KeyPress kp{keyVar, QDateTime::currentMSecsSinceEpoch()};
    m_keyBuffer.push_back(kp);

    this->anyPressed(event);

    // No on really clicks in this corner (0, 0) and this solves a
    // bug on Hyprland where it would register a mouse press in this corner
    if (event->pos() == QPoint{0, 0})
        return;

    qint64 lastTime{m_lastClickTime};
    m_lastClickTime = QDateTime::currentMSecsSinceEpoch();
    if (m_lastClickTime - lastTime <= Common::doubleClickInterval && !m_mouseMoved &&
        m_occupationKeyBuffer.empty()) {
        m_clickCount++;

        if (m_clickCount == 2) {
            mouseDoubleClick(event);
        } else if (m_clickCount == 3) {
            mouseTripleClick(event);
        }
        return;
    } else {
        m_clickCount = 1;
        m_mouseMoved = false;
    }

    Event &contextEvent{m_context->uiContext().event()};
    ToolBar &toolBar{m_context->uiContext().toolBar()};
    Canvas &canvas{m_context->renderingContext().canvas()};

    contextEvent.setPos(event->pos(), canvas.scale());
    contextEvent.setButton(event->button());
    contextEvent.setModifiers(event->modifiers());

    if (m_occupationKeyBuffer.empty()) {
        toolBar.curTool().mousePressed(m_context);
    }

    if (event->type() != QEvent::TabletPress) {
        contextEvent.setPressure(1.0);
    }

    m_prevKeyBuffer = m_keyBuffer;
    m_lastMousePress = event->button();
}

void Controller::mouseDoubleClick(QMouseEvent *event) {
    if (!m_occupationKeyBuffer.empty())
        return;

    Event &contextEvent{m_context->uiContext().event()};
    ToolBar &toolBar{m_context->uiContext().toolBar()};
    Canvas &canvas{m_context->renderingContext().canvas()};

    contextEvent.setPos(event->pos(), canvas.scale());
    contextEvent.setButton(event->button());
    contextEvent.setModifiers(event->modifiers());

    toolBar.curTool().mouseDoubleClick(m_context);
}

void Controller::mouseTripleClick(QMouseEvent *event) {
    if (!m_occupationKeyBuffer.empty())
        return;

    Event &contextEvent{m_context->uiContext().event()};
    ToolBar &toolBar{m_context->uiContext().toolBar()};
    Canvas &canvas{m_context->renderingContext().canvas()};

    contextEvent.setPos(event->pos(), canvas.scale());
    contextEvent.setButton(event->button());
    contextEvent.setModifiers(event->modifiers());

    toolBar.curTool().mouseTripleClick(m_context);
}

void Controller::mouseMoved(QMouseEvent *event) {
    m_mouseMoved = true;

    Event &contextEvent{m_context->uiContext().event()};
    ToolBar &toolBar{m_context->uiContext().toolBar()};
    Canvas &canvas{m_context->renderingContext().canvas()};

    contextEvent.setPos(event->pos(), canvas.scale());
    contextEvent.setButton(event->button());
    contextEvent.setModifiers(event->modifiers());

    for (auto &action : m_actions) {
        action.execute(m_taskCtx);
    }

    if (m_occupationKeyBuffer.empty()) {
        toolBar.curTool().mouseMoved(m_context);
    }
}

void Controller::mouseReleased(QMouseEvent *event) {
    auto old_occupationKeyBuffer = m_occupationKeyBuffer;

    m_keyBuffer.erase_if(
        [&](auto const &v) { return keyToken(v.m_key) == keyToken(event->button()); });

    this->anyReleased(event);

    Event &contextEvent{m_context->uiContext().event()};
    ToolBar &toolBar{m_context->uiContext().toolBar()};
    Canvas &canvas{m_context->renderingContext().canvas()};

    contextEvent.setPos(event->pos(), canvas.scale());
    contextEvent.setButton(event->button());
    contextEvent.setModifiers(event->modifiers());

    if (old_occupationKeyBuffer.empty()) {
        toolBar.curTool().mouseReleased(m_context);
    }

    m_prevKeyBuffer = m_keyBuffer;
}

void Controller::tablet(QTabletEvent *event) {
    Event &ev{m_context->uiContext().event()};

    // TODO: Remove magic numbers
    ev.setPressure(event->pressure() / 1.60 + 0.375);
}

void Controller::keyPressed(QKeyEvent *event) {
    std::variant<Qt::Key, Qt::MouseButton> keyVar{static_cast<Qt::Key>(event->key())};
    KeyPress kp{keyVar, QDateTime::currentMSecsSinceEpoch()};
    m_keyBuffer.push_back(kp);

    this->anyPressed(event);

    Event &contextEvent{m_context->uiContext().event()};
    ToolBar &toolBar{m_context->uiContext().toolBar()};

    contextEvent.setKey(event->key());
    contextEvent.setModifiers(event->modifiers());
    contextEvent.setText(event->text());

    if (m_occupationKeyBuffer.empty()) {
        toolBar.curTool().keyPressed(m_context);
    }

    m_prevKeyBuffer = m_keyBuffer;
    m_lastKeyPress = static_cast<Qt::Key>(event->key());
}

void Controller::keyReleased(QKeyEvent *event) {
    auto old_occupationKeyBuffer = m_occupationKeyBuffer;

    if (event->isAutoRepeat()) {
        return;
    }

    m_prevKeyBuffer = m_keyBuffer;
    // Issue is caused here unless auto repeat events are ignored
    m_keyBuffer.erase_if([&](auto const &v) {
        return keyToken(v.m_key) == keyToken(static_cast<Qt::Key>(event->key()));
    });

    this->anyReleased(event);

    Event &contextEvent{m_context->uiContext().event()};
    ToolBar &toolBar{m_context->uiContext().toolBar()};

    contextEvent.setKey(event->key());
    contextEvent.setModifiers(event->modifiers());
    contextEvent.setText(event->text());

    if (old_occupationKeyBuffer.empty()) {
        toolBar.curTool().keyReleased(m_context);
    }
}

void Controller::anyPressed(std::variant<QKeyEvent *, QMouseEvent *> input) {
    for (auto &action : m_actions) {
        for (const auto &keyCombination : action.m_data.m_keys) {
            auto keysView =
                keyCombination | std::views::transform([](auto key) { return key.m_id; });
            std::vector<std::variant<Qt::Key, Qt::MouseButton>> keys{keysView.begin(),
                                                                     keysView.end()};

            bool hasAllMoveKeys = action.m_data.m_maxGapMs < 0
                                      ? this->satisfiesKeys(keys)
                                      : this->satisfiesKeys(keys, std::nullopt);

            if (m_occupationKeyBuffer.empty() && hasAllMoveKeys) {
                m_occupationKeyBuffer = m_keyBuffer;
                action.enable(m_taskCtx);
            }

            action.m_state.m_prevHeld = hasAllMoveKeys;
        }
    }
}

void Controller::anyReleased(std::variant<QKeyEvent *, QMouseEvent *> input) {
    for (auto &action : m_actions) {
        for (const auto &keyCombination : action.m_data.m_keys) {
            bool hasAllMoveKeys = std::ranges::all_of(keyCombination, [&](const auto &requiredKey) {
                return std::ranges::any_of(m_keyBuffer, [&](const auto &pressedKey) {
                    return pressedKey.m_key == requiredKey.m_id;
                });
            });

            if (!hasAllMoveKeys) {
                m_occupationKeyBuffer.clear();
                action.disable(m_taskCtx);
            }

            action.m_state.m_prevHeld = hasAllMoveKeys;
        }
    }

    if (m_keyBuffer.empty()) {
        m_occupationKeyBuffer.clear();
    }
}

void Controller::inputMethodInvoked(QInputMethodEvent *event) {
}

void Controller::leave(QEvent *event) {
    ToolBar &toolBar{m_context->uiContext().toolBar()};

    toolBar.curTool().leave(m_context);
}

void Controller::wheel(QWheelEvent *event) {
    const QPointF &offsetPos{m_context->spatialContext().offsetPos()};
    const qreal zoomFactor{m_context->renderingContext().zoomFactor()};
    Canvas &canvas{m_context->renderingContext().canvas()};

    if (event->modifiers() & Qt::ControlModifier) {
        int delta{event->angleDelta().y() > 0 ? 1 : -1};
        m_context->renderingContext().updateZoomFactor(delta);
        return;
    }

    m_context->spatialContext().setOffsetPos(offsetPos - event->angleDelta() / zoomFactor);

    m_context->renderingContext().markForRender();
    m_context->renderingContext().markForUpdate();
}

bool Controller::satisfiesKeys(std::vector<std::variant<Qt::Key, Qt::MouseButton>> &required,
                               std::optional<qint64> maxGapMs) {
    using std::begin;
    using std::end;

    // Fast path: ordered search in buffer (insertion order)
    auto bufIt = m_keyBuffer.begin();
    for (auto const &reqKey : required) {
        bufIt = std::find_if(bufIt, m_keyBuffer.end(), [&](const KeyPress &kp) {
            return keyToken(kp.m_key) == keyToken(reqKey);
        });

        if (bufIt == m_keyBuffer.end()) {
            goto try_timed;
        }

        bufIt++;
    }

    return true;  // ordered sequence found

try_timed:
    if (!maxGapMs.has_value()) {
        return false;
    }

    // build required counts (respect multiplicity)
    std::unordered_map<uint64_t, int> reqCounts;
    reqCounts.reserve(required.size() * 2);
    for (auto const &r : required)
        ++reqCounts[keyToken(r)];

    // keep only events whose key is in required
    struct Tiny {
        uint64_t m_token;
        qint64 m_timeMs;
    };
    std::vector<Tiny> events;
    events.reserve(m_keyBuffer.size());
    for (auto const &ev : m_keyBuffer) {
        uint64_t t = keyToken(ev.m_key);
        if (reqCounts.find(t) != reqCounts.end()) {
            events.push_back({t, ev.m_timeMs});
        }
    }
    if (events.empty())
        return false;

    std::sort(events.begin(), events.end(), [](auto const &a, auto const &b) {
        return a.m_timeMs < b.m_timeMs;
    });

    const auto n = static_cast<size_t>(events.size());

    for (size_t s = 0; s < n; s++) {
        // quick skip: start event must be a required token (it is, by construction)
        std::unordered_map<uint64_t, int> have;
        have.reserve(reqCounts.size());
        have[events[s].m_token] = 1;
        qint64 prevTime = events[s].m_timeMs;

        // quick full-match check
        bool full = true;

        for (auto const &p : reqCounts) {
            int got = (have.find(p.first) != have.end()) ? have[p.first] : 0;
            if (got < p.second) {
                full = false;
                break;
            }
        }

        if (full)
            return true;

        for (size_t j = s + 1; j < n; j++) {
            qint64 gap = events[j].m_timeMs - prevTime;
            if (gap > maxGapMs.value())
                break;

            uint64_t tok = events[j].m_token;
            int needed = reqCounts[tok];
            int haveNow = (have.find(tok) != have.end()) ? have[tok] : 0;

            if (haveNow < needed) {
                have[tok] = haveNow + 1;
                prevTime = events[j].m_timeMs;  // advance the matched chain time
            }

            // check full
            bool done = true;
            for (auto const &p : reqCounts) {
                int g = (have.find(p.first) != have.end()) ? have[p.first] : 0;
                if (g < p.second) {
                    done = false;
                    break;
                }
            }
            if (done)
                return true;
        }
    }

    return false;
}

void Controller::initialize() {
    ToolBar &toolBar{m_context->uiContext().toolBar()};
    Canvas &canvas{m_context->renderingContext().canvas()};
    m_actions = getDefaultActions(toolBar, canvas);
    m_taskCtx = TaskContext{.m_appContext = m_context};
}
