# ----------------------------------------------------------------------------
# Amiga target setup
# ----------------------------------------------------------------------------

CFLAGS += -std=c99 -Dnsamiga

ifneq ($(SUBTARGET),os3)
  CFLAGS += -O2 -finline-functions -U__STRICT_ANSI__ -D__USE_INLINE__ -D__USE_BASETYPE__
else
  CFLAGS += -msoft-float -m68020-60 -O2 -DPATH_MAX=1024 -D__m68k__
endif

NETSURF_FEATURE_ROSPRITE_CFLAGS := -DWITH_NSSPRITE
NETSURF_FEATURE_BMP_CFLAGS := -DWITH_BMP
NETSURF_FEATURE_GIF_CFLAGS := -DWITH_GIF
NETSURF_FEATURE_PNG_CFLAGS := -DWITH_PNG
NETSURF_FEATURE_NSSVG_CFLAGS := -DWITH_NS_SVG
NETSURF_FEATURE_VIDEO_CFLAGS := -DWITH_VIDEO

ifeq ($(HOST),amiga)
  $(eval $(call feature_enabled,ROSPRITE,-DWITH_NSSPRITE,-lrosprite,Sprite (librosprite)))
  $(eval $(call feature_enabled,BMP,-DWITH_BMP,-lnsbmp,BMP (libnsbmp)))
  $(eval $(call feature_enabled,GIF,-DWITH_GIF,-lnsgif,GIF (libnsgif)))
  $(eval $(call feature_enabled,PNG,-DWITH_PNG,-lpng,PNG (libpng)  ))
  $(eval $(call feature_enabled,NSSVG,-DWITH_NS_SVG,-lsvgtiny,SVG (libsvgtiny)))
  $(eval $(call feature_enabled,VIDEO,-DWITH_VIDEO -I /SDK/local/newlib/include/glib-2.0,-lgstreamer-0.10 -lglib-2.0 -lgobject-2.0 -lgmodule-2.0 -lgthread-2.0 -lintl -lffi,Video (libgstreamer)))
  $(eval $(call feature_enabled,JS,-DXP_UNIX -DWITH_JS -DJS_VERSION=185,-lstdc++ -lmozjs185,JavaScript))
  $(eval $(call feature_enabled,MOZJS,-DXP_AMIGA -DWITH_MOZJS -DJS_VERSION=170 -DJSVERSION_LATEST=170 -DJSOPTION_JIT=0 -DJSCLASS_GLOBAL_FLAGS=0,-ljs -lfdlibm,JavaScript))
  $(eval $(call feature_enabled,AMIGA_ICON,-DWITH_AMIGA_ICON,,Amiga icon))
  $(eval $(call feature_enabled,AMIGA_DATATYPES,-DWITH_AMIGA_DATATYPES,,DataTypes))

  CFLAGS += -I /SDK/local/common/include/libpng12
  LDFLAGS += -lcurl -lrtmp -lpthread -ltre -lintl -lpbl
  LDFLAGS += -lssl -lcrypto -ldom -lhubbub -lcss -lparserutils -lwapcaplet
else
  $(eval $(call feature_enabled,AMIGA_ICON,-DWITH_AMIGA_ICON,,Amiga icon))
  $(eval $(call feature_enabled,AMIGA_DATATYPES,-DWITH_AMIGA_DATATYPES,,DataTypes))

  CFLAGS += -I$(GCCSDK_INSTALL_ENV)/include
  CFLAGS += $(shell $(PKG_CONFIG) --cflags tre)

  LDFLAGS += $(shell $(PKG_CONFIG) --static --libs libcurl openssl)
  LDFLAGS += $(shell $(PKG_CONFIG) --libs tre)
  LDFLAGS += -L$(GCCSDK_INSTALL_ENV)/lib

  LDFLAGS += -lpbl -liconv

  ifeq ($(SUBTARGET),os3)
    LDFLAGS += -lamiga -lm
  endif
endif

EXETARGET := NetSurf

# The filter and target for split messages
MESSAGES_FILTER=ami

# ----------------------------------------------------------------------------
# Source file setup
# ----------------------------------------------------------------------------

# sources purely for the Amiga build
S_FRONTEND := gui.c tree.c history.c hotlist.c schedule.c file.c	\
	misc.c bitmap.c font.c filetype.c utf8.c login.c		\
	plotters.c object.c menu.c save_pdf.c arexx.c version.c		\
	cookies.c ctxmenu.c clipboard.c help.c font_scan.c		\
	launch.c search.c history_local.c download.c iff_dr2d.c		\
	sslcert.c gui_options.c print.c	theme.c	drag.c icon.c libs.c	\
	datatypes.c dt_picture.c dt_anim.c dt_sound.c plugin_hack.c	\
	stringview/stringview.c stringview/urlhistory.c rtg.c		\
	agclass/amigaguide_class.c os3support.c font_diskfont.c		\
	selectmenu.c hash/xxhash.c font_cache.c font_bullet.c

# This is the final source build list
# Note this is deliberately *not* expanded here as common and image
#   are not yet available
SOURCES = $(S_COMMON) $(S_IMAGE) $(S_BROWSER) $(S_FRONTEND)

# ----------------------------------------------------------------------------
# Install target
# ----------------------------------------------------------------------------

install-amiga:

# ----------------------------------------------------------------------------
# Package target
# ----------------------------------------------------------------------------

package-amiga: netsurf.lha

AMIGA_LANGUAGES := de en it ja nl
AMIGA_PLATFORM_RESOURCES := Pointers Themes default.css default.css.info favicon.png LangNames mimetypes Resource.map SearchEngines splash.png
AMIGA_GENERIC_RESOURCES := $(AMIGA_LANGUAGES) ca-bundle Icons
AMIGA_RESOURCES := $(addprefix $(FRONTEND_SOURCE_DIR)/resources/,$(AMIGA_PLATFORM_RESOURCES)) $(addprefix \!NetSurf/Resources/,$(AMIGA_GENERIC_RESOURCES))
AMIGA_DISTRIBUTION_FILES := $(FRONTEND_SOURCE_DIR)/dist/*
AMIGA_PKG_DIR := $(FRONTEND_SOURCE_DIR)/pkg
AMIGA_INSTALL_TARGET_DIR := NetSurf_Amiga

netsurf.lha: $(EXETARGET)
	$(VQ)echo Creating netsurf.lha
	$(Q)rm -rf $(AMIGA_INSTALL_TARGET_DIR)
	$(Q)mkdir -p $(AMIGA_INSTALL_TARGET_DIR)/NetSurf
	$(Q)cp -p $(EXETARGET) $(AMIGA_INSTALL_TARGET_DIR)/NetSurf
	$(Q)mkdir $(AMIGA_INSTALL_TARGET_DIR)/NetSurf/Resources
	$(Q)cp -r $(AMIGA_RESOURCES) $(AMIGA_INSTALL_TARGET_DIR)/NetSurf/Resources
	$(Q)cp -r $(AMIGA_DISTRIBUTION_FILES) $(AMIGA_INSTALL_TARGET_DIR)/NetSurf
	$(Q)cp \!NetSurf/Resources/AdBlock,f79 $(AMIGA_INSTALL_TARGET_DIR)/NetSurf/Resources/adblock.css
	$(Q)cp \!NetSurf/Resources/CSS,f79 $(AMIGA_INSTALL_TARGET_DIR)/NetSurf/Resources/nsdefault.css
	$(Q)cp \!NetSurf/Resources/internal.css,f79 $(AMIGA_INSTALL_TARGET_DIR)/NetSurf/Resources/internal.css
	$(Q)cp \!NetSurf/Resources/Quirks,f79 $(AMIGA_INSTALL_TARGET_DIR)/NetSurf/Resources/quirks.css
	$(Q)cp \!NetSurf/Resources/netsurf.png,b60 $(AMIGA_INSTALL_TARGET_DIR)/NetSurf/Resources/netsurf.png
	$(Q)cp $(AMIGA_PKG_DIR)/drawer.info $(AMIGA_INSTALL_TARGET_DIR)/NetSurf.info
	$(Q)cp $(AMIGA_PKG_DIR)/AutoInstall $(AMIGA_INSTALL_TARGET_DIR)
  ifeq ($(SUBTARGET),os3)
	$(Q)cp $(AMIGA_PKG_DIR)/netsurf_os3.readme $(AMIGA_INSTALL_TARGET_DIR)/NetSurf
	$(Q)cp $(AMIGA_PKG_DIR)/netsurf_os3.readme.info $(AMIGA_INSTALL_TARGET_DIR)/NetSurf
  else
	$(Q)cp $(AMIGA_PKG_DIR)/netsurf.readme $(AMIGA_INSTALL_TARGET_DIR)/NetSurf
	$(Q)cp $(AMIGA_PKG_DIR)/netsurf.readme.info $(AMIGA_INSTALL_TARGET_DIR)/NetSurf
  endif
	$(Q)cd $(AMIGA_INSTALL_TARGET_DIR); \
	  lha a netsurf.lha NetSurf NetSurf.info AutoInstall
