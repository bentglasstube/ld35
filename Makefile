UNAME=$(shell uname)

SOURCES=$(wildcard src/*.cc)
CONTENT=$(wildcard content/*)
BUILDDIR=build
OBJECTS=$(patsubst %.cc,$(BUILDDIR)/%.o,$(SOURCES))
NAME=shifty-berries
APP_NAME="Shifty Berries"

CC=clang++
CFLAGS=-g --std=c++14
CFLAGS+=-Wall -Wextra -Werror -pedantic

ifeq ($(UNAME), Linux)
	PACKAGE=$(NAME)-linux.tgz
	LDFLAGS=-static-libstdc++ -static-libgcc
	LDLIBS=`sdl2-config --cflags --libs` -lSDL2_mixer
endif
ifeq ($(UNAME), Darwin)
	PACKAGE=$(NAME)-osx.tgz
	LDLIBS=-framework SDL2 -framework SDL2_mixer -rpath @executable_path/../Frameworks
	CFLAGS+=-mmacosx-version-min=10.9
endif

EXECUTABLE=$(BUILDDIR)/$(NAME)

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(OBJECTS) $(LDLIBS)

$(BUILDDIR)/%.o: %.cc
	@mkdir -p $(BUILDDIR)/src
	$(CC) -c $(CFLAGS) -o $@ $<

run: $(EXECUTABLE)
	./$(EXECUTABLE)

clean:
	rm -rf $(BUILDDIR) $(APP_NAME).app $(PACKAGE) $(NAME).{mkv,glc,wav}

video: $(NAME).mkv

$(NAME).mkv: $(NAME).glc $(NAME).wav
	glc-play $< -o - -y 1 |ffmpeg -i - -i $(NAME).wav -acodec flac -vcodec libx264 -y $@

$(NAME).wav: $(NAME).glc
	glc-play $< -a 1 -o $@

$(NAME).glc: $(EXECUTABLE)
	glc-capture -so $@ $(EXECUTABLE)

$(NAME)-linux.tgz: $(EXECUTABLE)
	mkdir $(NAME)
	cp $(EXECUTABLE) README.md $(NAME)
	cp -R content $(NAME)/content
	tar zcf $@ $(NAME)
	rm -rf $(NAME)

$(NAME)-osx.tgz: dotapp
	mkdir $(NAME)
	cp -r $(APP_NAME).app $(NAME)/.
	tar zcf $@ $(NAME)
	rm -rf $(NAME)

dotapp: $(APP_NAME).app

package: $(PACKAGE)

$(APP_NAME).app: $(EXECUTABLE) launcher $(CONTENT) Info.plist
	rm -rf $(APP_NAME).app
	mkdir -p $(APP_NAME).app/Contents/{MacOS,Frameworks}
	cp $(EXECUTABLE) $(APP_NAME).app/Contents/MacOS/game
	cp launcher $(APP_NAME).app/Contents/MacOS/launcher
	cp -R content $(APP_NAME).app/Contents/MacOS/content
	cp Info.plist $(APP_NAME).app/Contents/Info.plist
	cp -R /Library/Frameworks/SDL2.framework $(APP_NAME).app/Contents/Frameworks/SDL2.framework
	cp -R /Library/Frameworks/SDL2_mixer.framework $(APP_NAME).app/Contents/Frameworks/SDL2_mixer.framework

.PHONY: all clean run video dotapp
