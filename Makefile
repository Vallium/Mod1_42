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

SRC			= main.cpp

HEAD_DIR	= include
SRC_DIR		= src
DEBUG_DIR	= debug
STATIC_DIR	= static
DEP_DIR		= dep


INCLUDES	= -I glfw/include
LIBS		= -Lglfw/src -lglfw3 -framework Cocoa -framework OpenGL -framework IOKit -framework CoreVideo

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

clean:
	rm -f $(OBJ) $(OBJ_DEBUG) $(DEPS)

fclean: clean
	rm -f $(NAME) $(NAME_DEBUG)

re: fclean
	make
