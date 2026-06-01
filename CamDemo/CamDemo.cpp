#include "stdafx.h"
#include "CamDemo.h"

CamDemo::CamDemo(const ON_UUID& plugin_uuid)
    : CamBase("Unnamed CamDemo",                                    // Default name of this cam block, as displayed in the list.
        ON_UuidFromString(L"31108A39-8E67-46AC-8D24-23523FFEAE01"), // Replace with a unique UUID to identify this cam block type. This UUID can never be changed after release.
        plugin_uuid)
{
    // Initialize config options for this cam block.
    // All options must be stored in m_options to be saved and loaded correctly.
    // Note: "uuid", "plugin_uuid", "name" and "active" are already defined in CamBase. Do not add them again.

    m_tool = new Aion::ConfigOptionTool();
    this->m_options.addOption("tool", m_tool);
    m_tool->label = "Tool";
    m_tool->tooltip = "Select a tool";
    m_tool->tooltype_filter.push_back(Aion::ToolType::ttVolumetric);
    m_tool->tooltype_filter.push_back(Aion::ToolType::ttTriggered);
    m_tool->tooltype_filter.push_back(Aion::ToolType::ttExtruder);

    m_curves = new Aion::ConfigOptionComponentVector();
    this->m_options.addOption("pathUUIDs", m_curves);
    m_curves->label = "Paths";
    m_curves->tooltip = "Curve objects (lines, splines) to be printed";
}

Aion::CamBase* CamDemo::createNewInstance() const
{
    CamDemo* result = new CamDemo(this->getPluginUuid());
    return result;
}

bool CamDemo::generateToolpath(const Aion::DrawingConfig* config, Aion::Errorlist& errors)
{
    errors.push_back(Aion::Error(Aion::ErrorClass::Info, "CamDemo is not implemented"));
    return false;
}

Aion::OptionsGroups CamDemo::getOptionsByGroup()
{
    Aion::OptionsGroups result;
    Aion::OptionsGroup group_tool;
    group_tool.name = "Tool";
    group_tool.options.push_back(m_tool);
    result.push_back(group_tool);

    // Only show other options if a tool is selected.
    if (!m_tool->getString().empty()) {
        Aion::OptionsGroup group_geometry;
        group_geometry.name = "Geometry";
        group_geometry.options.push_back(m_curves);
        result.push_back(group_geometry);
    }

    return result;
}
