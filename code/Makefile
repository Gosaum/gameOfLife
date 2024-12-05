TARGET = main

# Compilateur et options
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17

# Liste des fichiers sources
SRCS = cell.cpp cellfactory.cpp obstaclecell.cpp standardcell.cpp grid.cpp standardgrid.cpp toroidalgrid.cpp lifealgorithm.cpp main.cpp

# Génération automatique des fichiers objets
OBJS = $(SRCS:.cpp=.o)

# Règle par défaut
all: $(TARGET) clean_objects

# Construction de l'exécutable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Compilation des fichiers objets
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Nettoyage des fichiers objets après compilation
clean_objects:
	rm -f $(OBJS)

# Nettoyage complet (fichiers objets et exécutable)
clean:
	rm -f $(OBJS) $(TARGET)

# Règle pour tout regénérer
rebuild: clean all