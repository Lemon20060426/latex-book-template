# ------------------------------------------------------------
# Cross-platform LaTeX Makefile (Overleaf-style build, explicit output/)
# ------------------------------------------------------------

# Main project files
TEX_MAIN    = main.tex
CLASS_FILE  = backend/BookTemplate.cls
BIB_FILE    = frontend/sample.bib
INDEX_IST   = backend/indexstyle.ist

# Frontend/backend trees
FRONTEND_SRC  = frontend/src
FRONTEND_INC  = frontend/inc
BACKEND_SRC   = backend/src
BACKEND_INC   = backend/inc

# Build/output dirs
BUILDDIR  = backend/build
OUTDIR    = output
PDF_OUT   = $(OUTDIR)/main.pdf

# --- Dependencies sweep (both frontend & backend, recursive wildcards) ---
# Use '=' (recursive) so wildcards are expanded at use-time (not frozen).
FRONTEND_SETTING = frontend/setting.tex
BACKEND_SETTING  = backend/setting.tex

# Explicit image extensions (portable)
IMG_DEPS = \
  $(wildcard frontend/pic/*.png) \
  $(wildcard frontend/pic/*.jpg) \
  $(wildcard frontend/pic/*.jpeg) \
  $(wildcard frontend/pic/*.pdf) \
  $(wildcard frontend/pic/*.svg)

# Optional: extend depth for future-proofing (+2 levels)
FRONTEND_TEX = \
  $(wildcard $(FRONTEND_SRC)/*.tex) \
  $(wildcard $(FRONTEND_SRC)/*/*.tex) \
  $(wildcard $(FRONTEND_SRC)/*/*/*.tex) \
  $(wildcard $(FRONTEND_SRC)/*/*/*/*.tex) \
  $(wildcard $(FRONTEND_SRC)/*/*/*/*/*.tex)

BACKEND_TEX = \
  $(wildcard $(BACKEND_SRC)/*.tex) \
  $(wildcard $(BACKEND_SRC)/*/*.tex) \
  $(wildcard $(BACKEND_SRC)/*/*/*.tex)

INC_TEX = \
  $(wildcard $(FRONTEND_INC)/*.tex) \
  $(wildcard $(BACKEND_INC)/*.tex)

# Rebuild TEX_DEPS with the new pieces
TEX_DEPS = $(TEX_MAIN) $(CLASS_FILE) $(BIB_FILE) $(INDEX_IST) \
           $(FRONTEND_SETTING) $(BACKEND_SETTING) \
           $(FRONTEND_TEX) $(BACKEND_TEX) $(INC_TEX) \
           $(IMG_DEPS)

# Absolute path for index style (so makeindex can find it from outdir)
INDEX_IST_ABS := $(abspath $(INDEX_IST))

# Detect OS
ifeq ($(OS),Windows_NT)
  SHELL := cmd.exe
  .SHELLFLAGS := /Q /C
  IS_WIN := 1

  MKDIR_P = if not exist "$(1)" mkdir "$(1)"
  CP_F    = copy /Y "$(1)" "$(2)" >nul
  RM_RF   = rmdir /S /Q "$(1)"
  RM_F    = del /Q /F "$(1)" 2>nul
  OPENPDF = start "" "$(PDF_OUT)"

  # IMPORTANT: keep trailing ';;' to preserve default TeX paths
  LATEXMK_BUILD = set "TEXINPUTS=.;backend;backend/src;backend/inc;frontend;frontend/src;frontend/inc;frontend/pic;;" && \
    latexmk -pdf -interaction=nonstopmode -file-line-error -g -f \
      -outdir="$(BUILDDIR)" -cd \
      -e "$$makeindex='makeindex -s \"$(INDEX_IST_ABS)\" %%O -o %%D %%S';"

  LATEXMK_CLEAN = latexmk -C -outdir="$(BUILDDIR)" -cd

else
  IS_WIN := 0

  MKDIR_P = mkdir -p "$(1)"
  CP_F    = cp -f "$(1)" "$(2)"
  RM_RF   = rm -rf "$(1)"
  RM_F    = rm -f $(1)
  OPENPDF = xdg-open "$(PDF_OUT)" >/dev/null 2>&1 || open "$(PDF_OUT)" || true

  # IMPORTANT: keep trailing '::' to preserve default TeX paths
  LATEXMK_BUILD = TEXINPUTS=".:backend:backend/src:backend/inc:frontend:frontend/src:frontend/inc:frontend/pic::" \
    latexmk -pdf -interaction=nonstopmode -file-line-error -g -f \
      -outdir="$(BUILDDIR)" -cd \
      -e "$$makeindex='makeindex -s \"$(INDEX_IST_ABS)\" %O -o %D %S';"

  LATEXMK_CLEAN = latexmk -C -outdir="$(BUILDDIR)" -cd
endif

# Default target
.DEFAULT_GOAL := all

.PHONY: all clean open help rebuild publish

all: $(PDF_OUT)

# Ensure dirs exist
$(BUILDDIR):
	@$(call MKDIR_P,$(BUILDDIR))

$(OUTDIR):
	@$(call MKDIR_P,$(OUTDIR))

# Build rule (depends on TEX_DEPS)
$(PDF_OUT): $(TEX_DEPS) | $(BUILDDIR) $(OUTDIR)
	@echo === Building PDF ===
	-@$(LATEXMK_BUILD) "$(TEX_MAIN)"
ifeq ($(IS_WIN),1)
	@if exist "$(BUILDDIR)\main.pdf" ($(call CP_F,$(BUILDDIR)\main.pdf,$(PDF_OUT))) else (echo WARNING: PDF not generated!)
else
	@if [ -f "$(BUILDDIR)/main.pdf" ]; then $(call CP_F,$(BUILDDIR)/main.pdf,$(PDF_OUT)); else echo "WARNING: PDF not generated!"; fi
endif

# Force a from-scratch rebuild (clean cache + run)
rebuild:
	@$(LATEXMK_CLEAN)
	@$(LATEXMK_BUILD) "$(TEX_MAIN)"

# Clean = remove build/ and output/
clean:
	@echo === Removing backend/build and output/ ===
	-@$(call RM_RF,$(BUILDDIR))
	-@$(call RM_RF,$(OUTDIR))

# Open PDF
open: $(PDF_OUT)
	@$(OPENPDF)

# Publish: copy output/main.pdf to parent folder (..), user-named
# Usage:
#   make publish                 # interactive; asks for name (default: main)
#   make publish NAME=myreport   # non-interactive => ../myreport.pdf
publish: $(PDF_OUT)
ifeq ($(IS_WIN),1)
	@echo === Publish PDF (Windows) ===
	@if not exist "$(OUTDIR)\main.pdf" (echo ERROR: not found: $(OUTDIR)\main.pdf & exit /b 1)
	@if "$(NAME)"=="" set /p NAME=Enter export filename (without .pdf^): 
	@if "$(NAME)"=="" set NAME=main
	@copy /Y "$(OUTDIR)\main.pdf" "..\$(NAME).pdf" >nul
	@echo Exported to ..\$(NAME).pdf
else
	@echo "=== Publish PDF (Unix/macOS) ==="
	@NAME="$(NAME)"; \
	if [ -z "$$NAME" ]; then read -r -p "Enter export filename (without .pdf): " NAME; fi; \
	[ -z "$$NAME" ] && NAME=main; \
	if [ ! -f "$(PDF_OUT)" ]; then echo "ERROR: not found: $(PDF_OUT)"; exit 1; fi; \
	cp -f "$(PDF_OUT)" "../$$NAME.pdf"; \
	echo "Exported to ../$$NAME.pdf"
endif

# Help
help:
	@echo "make            - build $(PDF_OUT)"
	@echo "make rebuild    - clean cache and rebuild (latexmk -C; then build)"
	@echo "make clean      - remove $(BUILDDIR) and $(OUTDIR)"
	@echo "make open       - open $(PDF_OUT)"
	@echo "make publish    - copy $(PDF_OUT) to ../<name>.pdf (interactive or NAME=<name>)"
	@echo "make help       - show this help"

