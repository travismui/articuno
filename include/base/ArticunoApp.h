#pragma once

#include "MooseApp.h"
#include "ArticunoRevision.h"

class ArticunoApp : public MooseApp
{
public:
  static InputParameters validParams();

  ArticunoApp(const InputParameters & parameters);
  virtual ~ArticunoApp();

  virtual std::string getVersion() const override { return ARTICUNO_REVISION; };
  virtual std::string getPrintableName() const override { return "ARTICUNO"; }

  static void registerApps();
  static void registerAll(Factory & f, ActionFactory & af, Syntax & s);
  static void associateSyntax(Syntax & syntax, ActionFactory & action_factory);

  virtual void setupOptions() override;
  virtual std::string header() const override;
};
