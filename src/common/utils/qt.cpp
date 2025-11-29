#include "qt.hpp"

namespace Common::Utils::QtUtil {
uint64_t keyToken(const std::variant<Qt::Key, Qt::MouseButton> &v) {
    uint64_t type = static_cast<uint64_t>(v.index());  // 0 = Qt::Key, 1 = Qt::MouseButton
    uint64_t val = 0;
    if (std::holds_alternative<Qt::Key>(v)) {
        val = static_cast<uint64_t>(std::get<Qt::Key>(v));
    } else {
        val = static_cast<uint64_t>(std::get<Qt::MouseButton>(v));
    }
    return (type << 32) | (val & 0xffffffffULL);
}
}  // namespace Common::Utils::QtUtil
