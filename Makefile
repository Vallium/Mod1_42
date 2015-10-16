# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: aalliot <aalliot@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2015/09/21 15:39:41 by aalliot           #+#    #+#              #
#    Updated: 2015/09/21 15:39:43 by aalliot          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		= mod1
NAME_DEBUG	= mod1_debug

SRC			=	main.cpp		\
				parse.cpp		\
				Camera.cpp		\
				Renderer.cpp	\
				Mesh.cpp		\
				Shader.cpp		\
				Context.cpp		\
				InputManager.cpp	\
				Map.cpp			\
				MapRenderer.cpp			\
				Drop.cpp		\
				sphere.cpp		\
				Octree.cpp

HEAD_DIR	= includes
SRC_DIR		= src
DEBUG_DIR	= debug
STATIC_DIR	= static
DEP_DIR		= dep


UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S),Linux)
	INCLUDES	=
	LIBS		= -lGL -lGLU -lglfw3 -lX11 -lXxf86vm -lXrandr -lpthread -lXi -lGLEW -ldl -lm -lXinerama -lXcursor -lrt -lm
endif

ifeq ($(UNAME_S),Darwin)
	INCLUDES	= -I glfw/include -I glm/ -I glew/include
	LIBS		= -Lglfw/src -Lglew/lib -lglfw3 -lGLEW -framework Cocoa -framework OpenGL -framework IOKit -framework CoreVideo -framework OpenCL -ldl
endif

OBJ			= $(patsubst %.cpp,$(STATIC_DIR)/%.o,$(SRC))
OBJ_DEBUG	= $(patsubst %.cpp,$(DEBUG_DIR)/%.o,$(SRC))
DEPS		= $(patsubst %.cpp,$(DEP_DIR)/%.d,$(SRC))

OPTI		= -O3
CXXFLAGS	= -std=c++11 -Wall #-Wextra -Wno-unused-result
DEPENDS 	= -MT $@ -MD -MP -MF $(subst .o,.d,$@)


$(shell mkdir -p $(STATIC_DIR) $(DEBUG_DIR) $(DEP_DIR))

.PHONY: clean fclean re debug init

all: $(NAME)

$(NAME): $(OBJ)
	$(CXX) $(OPTI) $(CXXFLAGS) -I $(HEAD_DIR) $(INCLUDES) -o $@ $(OBJ) $(LIBS)
	@echo "Compilation terminee. (realease)"

debug: $(OBJ_DEBUG)
	$(CXX) $(OPTI) $(CXXFLAGS) -I $(HEAD_DIR) $(INCLUDES) -o $(NAME_DEBUG) $(OBJ_DEBUG) $(LIBS) -g
	@echo "Compilation terminee. (debug)"

-include $(OBJ:.o=.d)

$(STATIC_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(OPTI) $(CXXFLAGS) $(DEPENDS) -I $(HEAD_DIR) $(INCLUDES) -o $@ -c $<

$(DEBUG_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(OPTI) $(CXXFLAGS) $(DEPENDS) -I $(HEAD_DIR) $(INCLUDES) -o $@ -c $< -g

init:
	git submodule init
	git submodule update
	cd glfw ; cmake .
	make -C glfw
	cd glm ; cmake .
	make -C glm
	make -C glew

clean:
	rm -f $(OBJ) $(OBJ_DEBUG) $(DEPS)

fclean: clean
	rm -f $(NAME) $(NAME_DEBUG)

re: fclean
	make
