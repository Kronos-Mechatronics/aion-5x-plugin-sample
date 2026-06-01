#pragma once
#include "stdafx.h"
#include "CamBase.h"

class CamDemo : public Aion::CamBase
{
public:
    // initialize with default name
    CamDemo(const ON_UUID& plugin_uuid);
    ~CamDemo() = default;
    // override class information
    // name of this cam block type
    virtual std::string getTypeName() const override { return "Demo"; };
    // short description of this cam block type
    virtual std::string getTypeShortDescription() const override { return "CamDemo example"; }
    // Optional url to documentation for this cam block type.
    virtual std::string getDocumentationUrl() const override { return "https://example.com"; }
    // factory method to create a new instance of this cam block type
    virtual Aion::CamBase* createNewInstance() const override;

    // generate Toolpath of the contour CAM block
    bool generateToolpath(const Aion::DrawingConfig* config, Aion::Errorlist& errors) override;
};