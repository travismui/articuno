###############################################################################
################### MOOSE Application Standard Makefile #######################
###############################################################################
#
# Optional Environment variables
# MOOSE_DIR        - Root directory of the MOOSE project
#
###############################################################################
# Use the MOOSE submodule if it exists and MOOSE_DIR is not set
MOOSE_SUBMODULE    := $(CURDIR)/moose
ifneq ($(wildcard $(MOOSE_SUBMODULE)/framework/Makefile),)
  MOOSE_DIR        ?= $(MOOSE_SUBMODULE)
else
  MOOSE_DIR        ?= $(shell dirname `pwd`)/moose
endif

MOLTRES_DIR ?= $(CURDIR)/moltres
MOLTRES_CONTENT := $(shell ls $(MOLTRES_DIR) 2> /dev/null)

SAM_DIR ?= $(CURDIR)/SAM
SAM_CONTENT := $(shell ls $(SAM_DIR) 2> /dev/null)

# framework
FRAMEWORK_DIR      := $(MOOSE_DIR)/framework
include $(FRAMEWORK_DIR)/build.mk
include $(FRAMEWORK_DIR)/moose.mk

################################## MODULES ####################################
# To use certain physics included with MOOSE, set variables below to
# yes as needed.  Or set ALL_MODULES to yes to turn on everything (overrides
# other set variables).

ALL_MODULES                 := no

CHEMICAL_REACTIONS          := no
CONTACT                     := no
ELECTROMAGNETICS            := no
EXTERNAL_PETSC_SOLVER       := no
FLUID_PROPERTIES            := yes
FSI                         := no
FUNCTIONAL_EXPANSION_TOOLS  := no
GEOCHEMISTRY                := no
HEAT_TRANSFER               := yes
LEVEL_SET                   := no
MISC                        := no
NAVIER_STOKES               := yes
OPTIMIZATION                := no
PERIDYNAMICS                := no
PHASE_FIELD                 := yes
POROUS_FLOW                 := no
RAY_TRACING                 := no
REACTOR                     := yes
RDG                         := no
RICHARDS                    := no
SOLID_MECHANICS             := yes
STOCHASTIC_TOOLS            := no
THERMAL_HYDRAULICS          := yes
XFEM                        := no
libmesh_CXXFLAGS            += -DTHM_ENABLED

include $(MOOSE_DIR)/modules/modules.mk
###############################################################################

MOOSE_JOBS ?= 8

# dep apps

# SAM
ifneq ($(SAM_CONTENT),)
  SAM_DIR            ?= $(CURDIR)/SAM
  libmesh_CXXFLAGS   += -DSAM_ENABLED
  APPLICATION_DIR    := $(SAM_DIR)
  APPLICATION_NAME   := sam
  include            $(FRAMEWORK_DIR)/app.mk
endif

# MOLTRES
ifneq ($(MOLTRES_CONTENT),)
  libmesh_CXXFLAGS   += -DMOLTRES_ENABLED

  MOLTRES_DIR        ?= $(CURDIR)/MOLTRES
  APPLICATION_DIR    := $(MOLTRES_DIR)
  APPLICATION_NAME   := moltres
  include            $(FRAMEWORK_DIR)/app.mk

  APPLICATION_DIR    := $(MOLTRES_DIR)/squirrel
  APPLICATION_NAME   := squirrel
  include            $(FRAMEWORK_DIR)/app.mk
endif

APPLICATION_DIR    := $(CURDIR)
APPLICATION_NAME   := articuno
BUILD_EXEC         := yes
DEP_APPS           := $(shell $(FRAMEWORK_DIR)/scripts/find_dep_apps.py $(APPLICATION_NAME))
include            $(FRAMEWORK_DIR)/app.mk

###############################################################################
# Additional special case targets should be added here
