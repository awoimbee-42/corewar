/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   asm.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/13 18:06:46 by awoimbee          #+#    #+#             */
/*   Updated: 2019/05/14 16:47:12 by cpoirier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ASM_H
# define ASM_H
# include "types.h"
# include "corewar.h"
# include "libft.h"
# include <fcntl.h>
# include <errno.h>

typedef struct	s_label
{
	char				*name;
	size_t				pos;
	size_t				offset;
}				t_label;

typedef struct	s_op
{
	char				*name;
	int					nb_args;
	t_arg_type			args_types[3]; // 3 is the max number of args -> the compiler will round to 4 anyways
	int					id; //op_code
	int					cycles;
	char				*desc;
	t_bool				coding_byte;
	t_bool				dir2;
}				t_op;

typedef struct	s_asm
{
	char				*file_name;
	int					curr_line;
	int					curr_char;
	//char				name[PROG_NAME_LENGTH + 1];
	//char				comment[COMMENT_LENGTH + 1];
	t_header			header;
	int					current_op;
	char				*output;
	size_t				cursor;
	t_label				*labels;
	size_t				label_pos;
	t_label				*labels_holder;
	size_t				label_holder_pos;
	size_t				op_begin;
}				t_asm;

extern const t_op	g_op_tab[17];

#endif
