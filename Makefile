# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2018/11/26 22:06:19 by marvin            #+#    #+#              #
#    Updated: 2019/05/14 20:32:55 by cpoirier         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

################################################################################
###############                  VARIABLES                       ###############
################################################################################

NAME =	corewar
ASM  =	asm

CFLAGS	=	-g3 -Wall -Wextra #-Ofast -march=native#-fsanitize=address #-Werror -Ofast -march=native

SRC_NAME_CORE =	main.c								\
				read_argv.c							\
				print_memory.c						\
				swap_endianess.c					\
				loop.c								\
				ops.c								\
				arena_mem_load.c					\
				arena_mem_write.c					\
				visu_loop.c							\
				operators/vecproc/init.c			\
				operators/vecproc/new.c				\
				operators/vecproc/point_last.c		\
				operators/vecproc/push.c			\
				operators/vecproc/push_empty.c		\
				operators/vecproc/realloc.c			\
				operators/vecproc/vector_del_at.c	\

SRC_NAME_ASM =	asm.c					\
				get_arg_type.c			\
				write.c					\
				labels.c				\
				helper.c				\
				handle_op.c				\
				get_asm.c				\
				utils.c

ASM_FD = asm/
ASM_SRC_SUBFOLDERS =
CORE_FD = vm/
CORE_SRC_SUBFOLDERS =	operators/vecplay		\
						operators/vecproc
BUILD_FOLDER =	build

################################################################################
###############                  CONSTANTS                       ###############
################################################################################
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
	NUMPROC := $(shell grep -c ^processor /proc/cpuinfo)
	EXEC_RPATH := \$$ORIGIN
else ifeq ($(UNAME_S),Darwin)
	NUMPROC := $(shell sysctl hw.ncpu | awk '{print $$2}')
	EXEC_RPATH := @loader_path #https://blogs.oracle.com/dipol/dynamic-libraries,-rpath,-and-mac-os
endif

CC = gcc
LDLIBS = -lft -lncurses -lpthread
LDFLAGS = -L./libft
CFLAGS += -MMD -I./includes -I./libft

SRC_FOLDERS = $(addprefix $(ASM_FD), $(ASM_SRC_SUBFOLDERS)) $(addprefix $(CORE_FD), $(CORE_SRC_SUBFOLDERS)) $(ASM_FD) $(CORE_FD)
REPO_PATH = $(patsubst %/,%,$(dir $(realpath $(firstword $(MAKEFILE_LIST)))))
BUILD_PATH = $(REPO_PATH)/$(BUILD_FOLDER)
SRC_PATH =	srcs
OBJ_FOLDER = $(BUILD_FOLDER)/obj
LFT = libft/libft.a

OBJ_CORE = $(addprefix $(OBJ_FOLDER)/$(CORE_FD), $(SRC_NAME_CORE:.c=.o))
OBJ_ASM  = $(addprefix $(OBJ_FOLDER)/$(ASM_FD),  $(SRC_NAME_ASM:.c=.o))

################################################################################
#################                  RULES                       #################
################################################################################
all :
	@$(MAKE) -j$(NUMPROC) $(NAME)   --no-print-directory
	@$(MAKE) -j$(NUMPROC) $(ASM) --no-print-directory

############## LIBS ############
$(LFT) :
	@printf "$(YLW)Making libft...$(EOC)\n"
	@$(MAKE) -s -C libft/
################################

$(NAME) : $(LFT) $(OBJ_CORE)
	@printf "$(GRN)Linking $@...$(EOC)\n"
	$(CC) $(CFLAGS) $(OBJ_CORE) -o $@ $(LDFLAGS) $(LDLIBS)

$(ASM)	: $(LFT) $(OBJ_ASM)
	@printf "$(GRN)Linking $@...$(EOC)\n"
	$(CC) $(CFLAGS) $(OBJ_ASM) -o $@ $(LDFLAGS) $(LDLIBS)

$(BUILD_FOLDER) :
	@mkdir -p $(BUILD_PATH) 2> /dev/null
	@mkdir -p $(OBJ_FOLDER) 2> /dev/null
	@mkdir -p $(addprefix $(OBJ_FOLDER)/,$(SRC_FOLDERS)) 2> /dev/null

$(BUILD_FOLDER)/obj :
	mkdir -p $(BUILD_FOLDER) $(BUILD_FOLDER)/obj 2> /dev/null
	mkdir -p $(addprefix $(OBJ_FOLDER)/,$(SRC_FOLDERS)) 2> /dev/null

#objects
$(OBJ_FOLDER)/%.o : $(SRC_PATH)/%.c $(LSDL2) $(LSDL2_TTF) | $(BUILD_FOLDER)/obj
	@printf "\t$(CC) (...) $@\n"
	@$(CC) $(CFLAGS) -o $@ -c $<

# Add rules written in .d files (by gcc -MMD)
# The '-' makes it doesn't care if the file exists or not
-include $(OBJ_DOOM:.o=.d) $(OBJ_EDITOR:.o=.d)

obj_clean :
	@printf "$(RED)Cleaning $(OBJ_FOLDER)$(EOC)\n"
	@rm -rf $(OBJ_FOLDER)

clean :
	@printf "$(RED)Cleaning $(BUILD_FOLDER)$(EOC)\n"
	@rm -rf $(BUILD_FOLDER)
	@printf "$(YLW)Cleaning libft...$(EOC)\n"
	@make -s fclean -C libft
	# @printf "$(RED)Cleaning $(DEPS_FOLDER)$(EOC)\n"
	# @rm -f ./$(DEPS_FOLDER)/lib/*.la ./$(DEPS_FOLDER)/lib/*.a
	# @rm -rf ./$(DEPS_FOLDER)/lib/cmake ./$(DEPS_FOLDER)/lib/pkgconfig

fclean : clean
	# @printf "$(YLW)Cleaning libs dependencies...$(EOC)\n"
	# @rm -rf $(DEPS_FOLDER)/lib
	@printf "$(RED)Cleaning $(NAME) & $(ASM)$(EOC)\n"
	@rm -f $(NAME)
	@rm -f $(ASM)

sclean	:	obj_clean
re		:	fclean		all
sre		:	obj_clean		all

.PHONY: all obj_clean clean fclean re sre

RED = \033[1;31m
GRN = \033[1;32m
YLW = \033[1;33m
INV = \033[7m
EOC = \033[0m
