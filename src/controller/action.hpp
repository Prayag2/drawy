#pragma once

#include <cstdint>
#include <memory>
#include <set>
#include <variant>
#include <vector>

#include "../canvas/canvas.hpp"
#include "../common/utils/qt.hpp"
#include "../components/toolbar.hpp"
#include "../tools/tool.hpp"

using namespace Common::Utils::QtUtil;

struct ActionKey {
    std::variant<Qt::Key, Qt::MouseButton> m_id;
    bool m_mouse{false};
    size_t m_count{1};

    // NOTE: required by std::set
    std::strong_ordering operator<=>(ActionKey const &o) const noexcept {
        if (m_id.index() != o.m_id.index())
            return m_id.index() < o.m_id.index() ? std::strong_ordering::less
                                                 : std::strong_ordering::greater;

        auto visit_cmp = std::visit(
            [](auto const &a, auto const &b) -> std::strong_ordering {
                using A = std::decay_t<decltype(a)>;
                using B = std::decay_t<decltype(b)>;

                if constexpr (!std::is_same_v<A, B>) {
                    return std::strong_ordering::equal;
                } else {
                    if (a < b)
                        return std::strong_ordering::less;
                    if (b < a)
                        return std::strong_ordering::greater;
                    return std::strong_ordering::equal;
                }
            },
            m_id,
            o.m_id);

        if (visit_cmp != std::strong_ordering::equal)
            return visit_cmp;

        if (m_mouse != o.m_mouse)
            return m_mouse < o.m_mouse ? std::strong_ordering::less : std::strong_ordering::greater;
        if (m_count < o.m_count)
            return std::strong_ordering::less;
        if (m_count > o.m_count)
            return std::strong_ordering::greater;
        return std::strong_ordering::equal;
    }
};

inline ActionKey key(Qt::MouseButton key, size_t count = 1) {
    return ActionKey{.m_id = std::variant<Qt::Key, Qt::MouseButton>{key},
                     .m_mouse = true,
                     .m_count = count};
}

inline ActionKey key(Qt::Key key, size_t count = 1) {
    return ActionKey{.m_id = key, .m_mouse = false, .m_count = count};
}

struct ActionData {
    std::set<std::vector<ActionKey>> m_keys;
    bool m_holdRequired{false};
    int64_t m_maxGapMs{0};
};

struct TaskContext {
    ApplicationContext *m_appContext;
};

struct ActionState {
    bool m_prevHeld{false};
};

class IActionTask {
public:
    virtual void start(const TaskContext &ctx, const ActionState &state) = 0;
    virtual void execute(const TaskContext &ctx, const ActionState &state) = 0;
    virtual void stop(const TaskContext &ctx, const ActionState &state) = 0;

    virtual ~IActionTask() = default;
};

class ToolTask : public IActionTask {
public:
    ToolTask(ToolBar &toolBar, Canvas &canvas, Tool::Type tool)
        : m_toolBar(toolBar),
          m_canvas(canvas),
          m_tool(tool) {}

    ToolBar &m_toolBar;
    Canvas &m_canvas;

    Tool::Type m_tool;

    void start(const TaskContext &ctx, const ActionState &state) override;
    void execute(const TaskContext &ctx, const ActionState &state) override;
    void stop(const TaskContext &ctx, const ActionState &state) override;
};

struct ActionFacade {
    ActionFacade(std::unique_ptr<IActionTask> task, ActionData data)
        : m_task(std::move(task)),
          m_data(std::move(data)) {}

    std::unique_ptr<IActionTask> m_task;
    ActionData m_data;
    ActionState m_state{};
    bool m_enabled{false};

    inline void enable(const TaskContext &ctx) {
        if (m_enabled)
            return;

        if (m_task)
            m_task->start(ctx, m_state);

        m_enabled = true;
    }

    inline void execute(const TaskContext &ctx) {
        if (!m_enabled || !m_task)
            return;

        m_task->execute(ctx, m_state);
    }

    inline void disable(const TaskContext &ctx) {
        if (!m_enabled)
            return;

        if (m_task)
            m_task->stop(ctx, m_state);

        m_enabled = false;
    }

    inline void toggle(TaskContext ctx) { m_enabled ? disable(ctx) : enable(ctx); }
};

inline std::vector<ActionFacade> getDefaultActions(ToolBar &toolbar, Canvas &canvas) {
    std::vector<ActionFacade> result;

    result.push_back(ActionFacade{
        std::make_unique<ToolTask>(toolbar, canvas, Tool::Move),
        ActionData{{{key(Qt::Key_Space), key(Qt::LeftButton)}, {key(Qt::MiddleButton)}}, true, -2},
    });

    result.push_back(ActionFacade{
        std::make_unique<ToolTask>(toolbar, canvas, Tool::Eraser),
        ActionData{{{key(Qt::RightButton)}}, true, -2},
    });

    return result;
}
