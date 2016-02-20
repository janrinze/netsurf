# Sources for desktop

S_DESKTOP := knockout.c mouse.c plot_style.c print.c search.c searchweb.c \
	scrollbar.c sslcert_viewer.c version.c system_colour.c

S_DESKTOP := $(addprefix browser/,$(S_DESKTOP))

# version.c needs the testament
core/browser/version.c: testament $(OBJROOT)/testament.h

# S_BROWSER are sources related to full browsers but are common
# between RISC OS, GTK, BeOS and AmigaOS builds
S_BROWSER := browser.c browser_history.c download.c frames.c netsurf.c \
	save_complete.c save_text.c selection.c textinput.c gui_factory.c \
	save_pdf.c font_haru.c

S_BROWSER := $(addprefix browser/,$(S_BROWSER))
