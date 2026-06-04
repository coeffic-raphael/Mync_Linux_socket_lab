# Nom du compilateur
CXX=g++

# Options de compilation
CXXFLAGS=-Wall -std=c++11

# Nom de l'exécutable
TARGET=ttt

# Sources
SOURCES=ttt.cpp

# Règle par défaut
all: $(TARGET)

# Règle de liaison
$(TARGET): $(SOURCES)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Règle pour nettoyer les fichiers compilés
clean:
	rm -f $(TARGET)

# Règle pour lancer le programme
run: $(TARGET)
	./$(TARGET)