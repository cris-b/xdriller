#Makefile simple para quien quiera intentar compilar el codigo sin el codeblocks

RESET_COLOR="\033[0m"
SET_GREEN="\033[32;01m"
SET_YELLOW="\033[33m"
SET_RED="\033[31m"

CC=g++
LIBS = OGRE OIS sdl
CFLAGS=-c -Wall $(shell pkg-config --cflags $(LIBS)) -I include
LDFLAGS= $(shell pkg-config --libs $(LIBS)) -lSDL_mixer
SOURCES = Arrows.cpp BackgroundSceneManager.cpp Board.cpp Brick.cpp Cam.cpp ColoredTextAreaOverlayElement.cpp ConfigManager.cpp \
CreditsState.cpp DotScene.cpp EditableText.cpp EntityMaterialInstance.cpp Fader.cpp GameManager.cpp \
GameState.cpp HighScoreManager.cpp HighScoreState.cpp HighScoreTable.cpp InputManager.cpp IntroState.cpp \
LevelLoader.cpp Main.cpp MaterialInstance.cpp MenuButton.cpp MenuState.cpp MovableText.cpp Music.cpp \
PauseState.cpp Player.cpp PlayerModelSelector.cpp PlayState.cpp RingSwitcher.cpp RingSwitcherObject.cpp \
RumbleManager.cpp Sound.cpp SoundManager.cpp SubEntityMaterialInstance.cpp SuperBrick.cpp \
TextEffectorBigMessage.cpp TextEffector.cpp tinystr.cpp tinyxml.cpp tinyxmlerror.cpp tinyxmlparser.cpp Tools.cpp

SOURCES_DIR = src
OBJS := $(patsubst %.cpp,$(SOURCES_DIR)/%.o,$(SOURCES))

EXECUTABLE = xdriller

LOCALES = es ca de eu fr
PO_FILES = $(patsubst %,locale/%/LC_MESSAGES/xdriller.po,$(LOCALES))
MO_FILES = $(PO_FILES:.po=.mo)


INSTALL_PREFIX=/usr
DATA_DIR=$(INSTALL_PREFIX)/share/xdriller


all: $(OBJS) $(MO_FILES)
	@echo -e $(SET_GREEN)Linking $(SET_YELLOW)$(EXECUTABLE)$(SET_GREEN) executable...$(RESET_COLOR)
	@$(CC) $(LDFLAGS) $(OBJS) -o $(EXECUTABLE)

%.o : %.cpp
	@echo -e $(SET_GREEN)Compiling $(SET_YELLOW)$<$(SET_GREEN)...$(RESET_COLOR)
	@$(CC) $(CFLAGS) -o $@ $<

clean:
	@echo -e $(SET_GREEN)Cleaning...$(RESET_COLOR)
	@rm -f $(EXECUTABLE) $(SOURCES_DIR)/*.o
	@rm -f $(MO_FILES)

locales: $(MO_FILES)


%.mo : %.po
	@echo -e $(SET_GREEN)Generating locale: $(SET_YELLOW)$@$(SET_GREEN)...$(RESET_COLOR)
	@msgfmt -cv -o $@ $<


install:


	mkdir -p $(DESTDIR)$(INSTALL_PREFIX)/bin
	mkdir -p $(DESTDIR)$(DATA_DIR)
	mkdir -p $(DESTDIR)$(INSTALL_PREFIX)/share/pixmaps
	mkdir -p $(DESTDIR)$(INSTALL_PREFIX)/share/applications

	cp  xdriller $(DESTDIR)$(INSTALL_PREFIX)/bin
	cp -r default_config locale sounds media.zip $(DESTDIR)$(DATA_DIR)
	cp  xdriller.png $(DESTDIR)$(INSTALL_PREFIX)/share/pixmaps
	cp  xdriller.desktop $(DESTDIR)$(INSTALL_PREFIX)/share/applications



