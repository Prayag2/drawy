/*
 * Drawy - A simple brainstorming tool with an infinite canvas
 * Copyright (C) 2025 - Prayag Jain <prayagjain2@gmail.com>
 *
 * Authors:
 * 1. quarterstar <quarterstar@proton.me>
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

#include <QKeyEvent>
#include <QMouseEvent>
#include <QtGlobal>
#include <cstdint>
#include <variant>

struct KeyPress {
    std::variant<Qt::Key, Qt::MouseButton> m_key;
    qint64 m_timeMs{};

    KeyPress() noexcept = default;
    KeyPress(std::variant<Qt::Key, Qt::MouseButton> k, qint64 t) noexcept
        : m_key(std::move(k)),
          m_timeMs(t) {}

    KeyPress(const KeyPress &) = default;
    KeyPress(KeyPress &&) noexcept = default;
    KeyPress &operator=(const KeyPress &) = default;
    KeyPress &operator=(KeyPress &&) noexcept = default;
    ~KeyPress() = default;

    // equality is used by UniqueDeque so we only
    // compare the key
    bool operator==(const KeyPress &o) const noexcept { return m_key == o.m_key; }
};

namespace std {
template <>
struct hash<KeyPress> {
    size_t operator()(KeyPress const &kp) const noexcept {
        uint64_t type = static_cast<uint64_t>(kp.m_key.index());  // 0 = variant index
        uint64_t val = 0;
        if (std::holds_alternative<Qt::Key>(kp.m_key)) {
            val = static_cast<uint64_t>(std::get<Qt::Key>(kp.m_key));
        } else {
            val = static_cast<uint64_t>(std::get<Qt::MouseButton>(kp.m_key));
        }
        uint64_t combined = (type << 32) | (val & 0xffffffffULL);
        return std::hash<uint64_t>{}(combined);
    }
};
}  // namespace std
