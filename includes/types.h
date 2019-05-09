/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   types.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/05 21:12:00 by awoimbee          #+#    #+#             */
/*   Updated: 2019/05/09 22:19:31 by cpoirier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TYPES_H
# define TYPES_H
# include "corewar.h"
# include <string.h>

typedef enum	e_bool
{
	FALSE,
	TRUE
}				t_bool;

typedef struct	s_header
{
	unsigned int		magic;
	char				prog_name[PROG_NAME_LENGTH + 1];
	unsigned int		prog_size;
	char				comment[COMMENT_LENGTH + 1];
}				t_header;

typedef struct	s_op
{
	char				*name;
	int					nb_args;
	t_arg_type			args_types[3]; // 3 is the max number of args -> the compiler will round to 4 anyways
	int					id; //op_code
	int					cycles;
	char				*desc;
	t_bool				coding_byte;
	t_bool				carry;
}				t_op;

typedef	struct	s_label
{
	char				*name;
	size_t				pos;
	size_t				offset;
}				t_label;

typedef	struct	s_asm
{
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

#endif
