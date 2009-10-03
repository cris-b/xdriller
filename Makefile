#Makefile simple para quien quiera intentar compilar el codigo sin el codeblocks

CC=g++
LIBS = OGRE OIS sdl
CFLAGS=-c -Wall $(shell pkg-config --cflags $(LIBS)) -I include -I /usr/include/OGRE
LDFLAGS= $(shell pkg-config --libs $(LIBS)) -lSDL_mixer
SOURCES := Arrows.cpp Board.cpp Brick.cpp Cam.cpp ConfigManager.cpp CreditsState.cpp DotScene.cpp EntityMaterialInstance.cpp Fader.cpp GameManager.cpp GameState.cpp InputManager.cpp IntroState.cpp LevelLoader.cpp Main.cpp MaterialInstance.cpp MenuButton.cpp MenuState.cpp MovableText.cpp Music.cpp PauseState.cpp Player.cpp PlayState.cpp Sound.cpp SoundManager.cpp SubEntityMaterialInstance.cpp SuperBrick.cpp tinystr.cpp tinyxml.cpp tinyxmlerror.cpp tinyxmlparser.cpp Tools.cpp

SOURCES_DIR = src
OBJS := $(patsubst %.cpp,$(SOURCES_DIR)/%.o,$(SOURCES))

EXECUTABLE = xdriller

INSTALL_PREFIX=/usr

all: $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) -o $(EXECUTABLE)

%.o : %.cpp
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f $(EXECUTABLE) $(SOURCES_DIR)/*.o



install:
	mkdir -p $(INSTALL_PREFIX)/games/bin
	mkdir -p $(INSTALL_PREFIX)/share/games/xdriller
	mkdir -p $(INSTALL_PREFIX)/share/pixmaps
	mkdir -p $(INSTALL_PREFIX)/share/applications

	cp  xdriller $(INSTALL_PREFIX)/games/bin
	cp -rv default_config $(INSTALL_PREFIX)/share/games/xdriller
	cp -rv media $(INSTALL_PREFIX)/share/games/xdriller
	cp  xdriller.png $(INSTALL_PREFIX)/share/pixmaps
	cp  xdriller.desktop $(INSTALL_PREFIX)/share/applications

uninstall:
	rm   $(INSTALL_PREFIX)/games/bin/xdriller
	rm -rv $(INSTALL_PREFIX)/share/games/xdriller
	rm   $(INSTALL_PREFIX)/share/pixmaps/xdriller.png
	rm   $(INSTALL_PREFIX)/share/applications/xdriller.desktop


