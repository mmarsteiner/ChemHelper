# ----------------------------
# Makefile Options
# ----------------------------

NAME = CHEM
ICON = icon.png
DESCRIPTION = "AP Chem Programs"
COMPRESSED = NO
HAS_PRINTF = NO

CFLAGS = -Wall -Wextra -Oz
CXXFLAGS = -Wall -Wextra -Oz

# ----------------------------

include $(shell cedev-config --makefile)
