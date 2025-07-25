#include "ArticunoApp.h"
#include "Moose.h"
#include "AppFactory.h"
#include "ModulesApp.h"
#include "MooseSyntax.h"
#include "SystemInfo.h"
#include "CommandLine.h"

#ifdef SAM_ENABLED
#include "SamApp.h"
#include "SamRevision.h"
#endif

#ifdef MOLTRES_ENABLED
#include "MoltresApp.h"
#include "MoltresRevision.h"
#include "SquirrelApp.h"
#endif

InputParameters
ArticunoApp::validParams()
{
  InputParameters params = MooseApp::validParams();
  params.set<bool>("use_legacy_material_output") = false;
  params.set<bool>("use_legacy_initial_residual_evaluation_behavior") = false;
  return params;
}

registerKnownLabel("ArticunoApp");

ArticunoApp::ArticunoApp(const InputParameters & parameters) : MooseApp(parameters)
{
  ArticunoApp::registerAll(_factory, _action_factory, _syntax);
}

ArticunoApp::~ArticunoApp() {}

void
ArticunoApp::registerAll(Factory & f, ActionFactory & af, Syntax & syntax)
{
  ModulesApp::registerAllObjects<ArticunoApp>(f, af, syntax);
  Registry::registerObjectsTo(f, {"ArticunoApp"});
  Registry::registerActionsTo(af, {"ArticunoApp"});

#ifdef SAM_ENABLED
  SamApp::registerAll(f, af, syntax);
#endif
#ifdef MOLTRES_ENABLED
  MoltresApp::registerAll(f, af, syntax);
  SquirrelApp::registerAll(f, af, syntax);

  syntax.registerActionSyntax("NtAction", "Nt");
  syntax.registerActionSyntax("PrecursorAction", "Precursors/*");
#endif
}

void
ArticunoApp::registerApps()
{
  registerApp(ArticunoApp);

#ifdef SAM_ENABLED
  SamApp::registerApps();
#endif
#ifdef MOLTRES_ENABLED
  MoltresApp::registerApps();
#endif
}

/***************************************************************************************************
 *********************** Dynamic Library Entry Points - DO NOT MODIFY ******************************
 **************************************************************************************************/
extern "C" void
ArticunoApp__registerAll(Factory & f, ActionFactory & af, Syntax & s)
{
  ArticunoApp::registerAll(f, af, s);
}
extern "C" void
ArticunoApp__registerApps()
{
  ArticunoApp::registerApps();
}

void
ArticunoApp::associateSyntax(Syntax & syntax, ActionFactory & /*action_factory*/)
{
#ifdef SAM_ENABLED
  // Fixup syntax registry to replace SAMAddPostprocessorAction and restore PostprocessorAsControlAction from THM
  syntax.replaceActionSyntax("AddPostprocessorAction", "Postprocessors/*", "add_sam_pps");
  syntax.registerActionSyntax("PostprocessorAsControlAction", "Postprocessors/*");
#endif
}

void
ArticunoApp::setupOptions()
{
#ifdef SAM_ENABLED
  // if we are not creating a SamApp, we do not need to be using SAM default executioner
  auto app_type = parameters().get<std::string>("app_to_run");
  if (app_type.compare("SamApp") != 0)
  {
    auto app_cli = this->commandLine();
    app_cli->addArgument("Executioner/set_sam_default_petsc_options=false");
  }
#endif

  // Syntax is parsed at this point
  MooseApp::setupOptions();
}

std::string
ArticunoApp::header() const
{
  if (multiAppLevel() != 0)
    return std::string("");

  std::stringstream os;
  os << '\n'
     << "************************************************************************************\n"
     << '\n'
     << "                                      Articuno\n"
     << '\n'
     << "************************************************************************************\n"

     << '\n'
     << '\n'
     << "                                SAM-MOLTRES Combined App\n"
     << "\n\n";

  os << std::left;
  os << std::setw(25) << "Articuno version: " << ARTICUNO_REVISION << "\n";
  os << std::setw(25) << "\nApplication Information:\n";
#ifdef MOLTRES_ENABLED
  os << std::setw(25) << "  MOLTRES version: " << MOLTRES_REVISION << "\n";
#endif
#ifdef SAM_ENABLED
  os << std::setw(25) << "  SAM version: " << SAM_REVISION << "\n";
#endif
  os << "\n";
  if (getSystemInfo() != NULL)
    os << getSystemInfo()->getInfo();

  os << '\n'
     << "Parallelism:\n"
     << std::setw(25) << "  Num Processors: " << static_cast<std::size_t>(n_processors()) << '\n'
     << std::setw(25) << "  Num Threads: " << static_cast<std::size_t>(libMesh::n_threads()) << '\n'
     << '\n';

  if (isParamValid("_argc") && isParamValid("_argv"))
  {
    int argc = getParam<int>("_argc");
    char ** argv = getParam<char **>("_argv");

    os << "Command line:";
    for (int i = 0; i < argc; ++i)
    {
      std::string s = argv[i];
      auto p = s.find('=');
      if (p != std::string::npos && s.substr(++p).find(' ') != std::string::npos)
        os << " " << s.substr(0, p) << '\'' << s.substr(p) << '\'';
      else
        os << " " << s;
    }
    os << '\n';
  }

  return os.str();
}
