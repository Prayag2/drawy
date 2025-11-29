#include "action.hpp"

void ToolTask::start(const TaskContext &ctx, const ActionState &state) {
    m_toolBar.curTool().cleanup();
    m_canvas.setCursor(m_toolBar.tool(m_tool).cursor());
    m_toolBar.tool(m_tool).mousePressed(ctx.m_appContext);

    // This is needed so that the thing hovered
    // over the first time a press is detected is immediately
    // queued for deletion.
    if (!state.m_prevHeld && m_tool == Tool::Eraser) {
        m_toolBar.tool(m_tool).mouseMoved(ctx.m_appContext);
    }
}

void ToolTask::execute(const TaskContext &ctx, const ActionState &state) {
    m_toolBar.tool(m_tool).mouseMoved(ctx.m_appContext);
}

void ToolTask::stop(const TaskContext &ctx, const ActionState &state) {
    m_toolBar.tool(m_tool).mouseReleased(ctx.m_appContext);
    m_toolBar.tool(m_tool).cleanup();
    m_canvas.setCursor(m_toolBar.curTool().cursor());
}
