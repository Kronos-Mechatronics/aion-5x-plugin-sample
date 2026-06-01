#include "stdafx.h"
#include "CamDemo.h"

CamDemo::CamDemo(const ON_UUID& plugin_uuid)
    : CamBase("Unnamed CamDemo",                                    // Default name of this cam block, as displayed in the list.
        ON_UuidFromString(L"31108A39-8E67-46AC-8D24-23523FFEAE01"), // Replace with a unique UUID to identify this cam block type. This UUID can never be changed after release.
        plugin_uuid)
{
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
