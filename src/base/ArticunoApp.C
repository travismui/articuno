#include "ArticunoApp.h"
#include "Moose.h"
#include "AppFactory.h"
#include "ModulesApp.h"
#include "MooseSyntax.h"

InputParameters
ArticunoApp::validParams()
{
  InputParameters params = MooseApp::validParams();
  params.set<bool>("use_legacy_material_output") = false;
  params.set<bool>("use_legacy_initial_residual_evaluation_behavior") = false;
  return params;
}

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

  /* register custom execute flags, action syntax, etc. here */
}

void
ArticunoApp::registerApps()
{
  registerApp(ArticunoApp);
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
