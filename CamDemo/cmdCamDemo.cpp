// cmdCamDemo.cpp : command file
//

#include "stdafx.h"
#include "CamDemoPlugIn.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN CamDemo command
//

#pragma region CamDemo command

// Do NOT put the definition of class CCommandCamDemo in a header
// file. There is only ONE instance of a CCommandCamDemo class
// and that instance is the static theCamDemoCommand that appears
// immediately below the class definition.

class CCommandCamDemo : public CRhinoCommand
{
public:
  // The one and only instance of CCommandCamDemo is created below.
  // No copy constructor or operator= is required.
  // Values of member variables persist for the duration of the application.

  // CCommandCamDemo::CCommandCamDemo()
  // is called exactly once when static theCamDemoCommand is created.
  CCommandCamDemo() = default;

  // CCommandCamDemo::~CCommandCamDemo()
  // is called exactly once when static theCamDemoCommand is destroyed.
  // The destructor should not make any calls to the Rhino SDK. 
  // If your command has persistent settings, then override 
  // CRhinoCommand::SaveProfile and CRhinoCommand::LoadProfile.
  ~CCommandCamDemo() = default;

  // Returns a unique UUID for this command.
  // If you try to use an id that is already being used, then
  // your command will not work. Use GUIDGEN.EXE to make unique UUID.
  UUID CommandUUID() override
  {
    // {1F4970C3-C582-4B2C-82F5-049CD3FE810C}
    static const GUID CamDemoCommand_UUID = 
    {0x1f4970c3,0xc582,0x4b2c,{0x82,0xf5,0x04,0x9c,0xd3,0xfe,0x81,0x0c}};
    return CamDemoCommand_UUID;
  }

  // Returns the English command name.
  // If you want to provide a localized command name, then override 
  // CRhinoCommand::LocalCommandName.
  const wchar_t* EnglishCommandName() override { return L"CamDemo"; }

  // Rhino calls RunCommand to run the command.
  CRhinoCommand::result RunCommand(const CRhinoCommandContext& context) override;
};

// The one and only CCommandCamDemo object
// Do NOT create any other instance of a CCommandCamDemo class.
static class CCommandCamDemo theCamDemoCommand;

CRhinoCommand::result CCommandCamDemo::RunCommand(const CRhinoCommandContext& context)
{
  // CCommandCamDemo::RunCommand() is called when the user
  // runs the "CamDemo".

  // TODO: Add command code here.

  // Rhino command that display a dialog box interface should also support
  // a command-line, or script-able interface.

  ON_wString str;
  str.Format(L"The \"%s\" command is under construction.\n", EnglishCommandName());
  const wchar_t* pszStr = static_cast<const wchar_t*>(str);
  if (context.IsInteractive())
    RhinoMessageBox(pszStr, CamDemoPlugIn().PlugInName(), MB_OK);
  else
    RhinoApp().Print(pszStr);

  // TODO: Return one of the following values:
  //   CRhinoCommand::success:  The command worked.
  //   CRhinoCommand::failure:  The command failed because of invalid input, inability
  //                            to compute the desired result, or some other reason
  //                            computation reason.
  //   CRhinoCommand::cancel:   The user interactively canceled the command 
  //                            (by pressing ESCAPE, clicking a CANCEL button, etc.)
  //                            in a Get operation, dialog, time consuming computation, etc.

  return CRhinoCommand::success;
}

#pragma endregion

//
// END CamDemo command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
