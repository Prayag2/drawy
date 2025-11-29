#pragma once

#include <QKeyEvent>
#include <QMouseEvent>
#include <cstdint>
#include <variant>

namespace Common::Utils::QtUtil {
uint64_t keyToken(const std::variant<Qt::Key, Qt::MouseButton> &v);
}
