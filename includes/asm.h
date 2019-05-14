/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   asm.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/13 18:06:46 by awoimbee          #+#    #+#             */
/*   Updated: 2019/05/14 20:33:56 by cpoirier         ###   ########.fr       */
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

void				fail_msg(t_asm *my_asm, char *s);
t_arg_type			get_arg_type(char *s);
void				write_to_output(char **output, size_t *pos, char *src);
void				write_n_to_output(char **o, size_t *p, char *src, size_t n);
void				write_nb_to_output(t_asm *my_asm, int nb, int pre);
void				write_output(t_asm *my_asm);
void				write_opcode(t_asm *my_asm, t_arg_type types[3]);
int					read_label(t_label *label, char *s);
void				init_labels(t_asm *my_asm);
void				add_label(t_label **l, size_t *pos, char *name, size_t p);
void				write_label_holders(t_asm *my_asm);
void				create_label_holder(t_asm *my_asm, char *s, size_t *i, 
		char *name);
int					get_name(char *s, char name[], size_t len);
int					get_op_id(char *s);
void				write_header(t_asm *my_asm);
char				*get_base_name(char *s);
void				init_op(t_asm *my_asm, char *s, size_t *i, t_arg_type t[3]);
void				handle_current_op(t_asm *a, char *s, size_t *i, size_t *c);
void				handle_op(t_asm *my_asm, char *s);
void				init_asm(t_asm *my_asm, int *fd, char *path);
void				handle_name(t_asm *my_asm, char *s, size_t *i);
void				handle_comment(t_asm *my_asm, char *s, size_t *i);
void				handle_labels(t_asm *my_asm, char *s, size_t *i);
void				clear_asm(t_asm *my_asm, char *s);

#endif
