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

#include "keybindmanager.hpp"

KeybindManager::KeybindManager(QObject *parent) : QObject{parent} {
}

void KeybindManager::addKeybinding(Action *action, const QString &sequence) {
    if (m_keyToAction.find(sequence) != m_keyToAction.end())
        return;

    if (m_keyToShortcut.find(sequence) == m_keyToShortcut.end()) {
        m_keyToShortcut[sequence] = new QShortcut{QKeySequence::fromString(sequence), parent()};
    }

    m_keyToAction[sequence] = action;

    QShortcut *shortcut{m_keyToShortcut[sequence]};
    QObject::connect(shortcut, &QShortcut::activated, action, &Action::run);
}

// TODO: Implement this
void removeKeybinding(QKeySequence sequence);

void KeybindManager::disable() {
    for (auto &keyShortcutPair : m_keyToShortcut) {
        keyShortcutPair.second->setEnabled(false);
    }
}

void KeybindManager::enable() {
    for (auto &keyShortcutPair : m_keyToShortcut) {
        keyShortcutPair.second->setEnabled(true);
    }
}
