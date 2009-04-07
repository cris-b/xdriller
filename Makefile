#Makefile simple para quien quiera intentar compilar el codigo sin el codeblocks

CC=g++
LIBS = OGRE OIS sdl
CFLAGS=-c -Wall $(shell pkg-config --cflags $(LIBS)) -I include -I /usr/include/OGRE
LDFLAGS= $(shell pkg-config --libs $(LIBS)) -lSDL_mixer
SOURCES := Board.cpp Brick.cpp Cam.cpp ConfigManager.cpp \
    DotScene.cpp EntityMaterialInstance.cpp GameManager.cpp GameState.cpp \
    InputManager.cpp IntroState.cpp LevelLoader.cpp Main.cpp  \
    MaterialInstance.cpp MenuButton.cpp MenuState.cpp MovableText.cpp \
    Music.cpp PauseState.cpp Player.cpp PlayState.cpp Sound.cpp \
    SoundManager.cpp SubEntityMaterialInstance.cpp \
    SuperBrick.cpp tinystr.cpp tinyxml.cpp tinyxmlerror.cpp tinyxmlparser.cpp Tools.cpp

SOURCES_DIR = src
OBJS := $(patsubst %.cpp,$(SOURCES_DIR)/%.o,$(SOURCES))

EXECUTABLE = xdriller

all: $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) -o $(EXECUTABLE)

%.o : %.cpp
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f $(EXECUTABLE) $(SOURCES_DIR)/*.o
