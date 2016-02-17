# Sources for core

# Content sources
include core/content/Makefile

# Content fetchers sources
include core/content/fetchers/Makefile

# CSS sources
include core/css/Makefile

# render sources
include core/render/Makefile

# Desktop sources
include core/desktop/Makefile

# Javascript source
include core/javascript/Makefile

# Image content handler sources
include core/image/Makefile

# S_CORE are sources common to all builds
S_CORE := $(addprefix core/,$(S_CONTENT) $(S_FETCHERS) $(S_CSS) $(S_RENDER) $(S_DESKTOP) $(S_JAVASCRIPT)) $(S_JAVASCRIPT_BINDING)

S_BROWSER := $(addprefix core/,$(S_BROWSER))

S_IMAGE := $(addprefix core/,$(S_IMAGE))
