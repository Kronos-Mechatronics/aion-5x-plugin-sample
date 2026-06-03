#include "stdafx.h"
#include "CamDemo.h"
#include "InteractiveSelection.h"
#include "Geometry5X.h"

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
    // clear existing paths
    m_toolpaths.clear();

    // Check if a tool is specified
    // Note: we can return three error levels here:
    // Critical: something is wrong, toolpath can not be generated.
    // Warning: something is missing, toolpath can not be generated, but this is not a real error. E.g. no tool selected.
    // Info: information, e.g. no geometry selected. The resulting toolpath is valid, but may be empty.
    if (m_tool->getString().empty()) {
        errors.push_back(Aion::Error(Aion::ErrorClass::Warning, "No tool specified"));
        // No valid result, return false -> gcode export not possible
        return false;
    }

    // Check if input geometry is specified.
    if (m_curves->getComponents()->empty()) {
        errors.push_back(Aion::Error(Aion::ErrorClass::Info, "No geometry selected"));
        // Valid (but empty) result, return true -> gcode export possible
        return true;
    }

    // Get the tool config to to access tool specific parameters
    const Aion::ToolConfig* tool_config = config->getTool(m_tool->getString());
    if (!tool_config) {
        errors.push_back(Aion::Error(Aion::ErrorClass::Critical, "Tool not found in config"));
        // No valid result, return false -> gcode export not possible
        return false;
    }

    // Compute a set of toolpaths objects from input geometry.
    for (const Aion::Component& comp : *m_curves->getComponents()) {
        // Loop over each curve
        ON_PolylineCurve pline;
        ON_Polyline pl;

        // Try to extract a polyline (sequence of linear segments) from the curve.
        if (Aion::Geometry5X::getPolylineCurveFromComponent(comp, pline, config->machineConfig().path_deviation_tolerance())) {
            pl = pline.m_pline;
            // Return when polyline is invalid or has a length of 0
            if (!pl.IsValid() || pl.Count() == 0) {
                errors.push_back(Aion::Error(Aion::ErrorClass::Warning, "Polyline is invalid or has a length of 0"));
                return false;
            }
        } else {
            // Throw an error if the object is not a polyline
            errors.push_back(Aion::Error(Aion::ErrorClass::Critical, "Object can not be converted to polyline"));
            return false;
        }

        // Create a new toolpath path
        Aion::ToolpathPath* path = new Aion::ToolpathPath(tool_config);

        // Add all curve points to the toolpath
        for (int i = 0; i < pl.Count(); i++) {
            path->appendPoint(pl[i], ON_3dVector(0, 0, 1)); // tool vector is set to +Z for all points
        }

        // Set toolpath parameters
        path->height = 0.2;
        path->width = 0.5;
        path->speed = 40;

        // Add this path to the list of generated toolpaths.
        // CamBase takes ownership.
        m_toolpaths.emplace_back(path);
    }

    return true;
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

void CamDemo::onOptionChanged(const Aion::ConfigOptionBase* option, bool reset)
{
    // Set the recalculate flag
    setNeedsPathplanning();

    if (option == m_curves && !reset) {
        std::vector<Aion::Component>* components = m_curves->getComponents();
        // Select curves
        Aion::InteractiveSelection::interactiveCurveSelection(*components);
    }
}

void CamDemo::onObjectChanged(const ON_UUID& uuid, ON::object_type type, bool deleted)
{
    bool change = false;
    // Check if the modified object is part of m_curves.
    // If it is deleted we also delete it from the list.
    if (type == ON::curve_object || type == ON::instance_reference) {
        for (auto it = m_curves->getComponents()->begin(); it != m_curves->getComponents()->end();) {
            if (it->uuid == uuid) {
                change = true;
                if (deleted) {
                    it = m_curves->getComponents()->erase(it);
                    continue; // continue to the next iteration
                }
            }
            it++;
        }
    }

    // Set recalculate flag if we are affected by this object change
    if (change) {
        setNeedsPathplanning(true);
    }
}
