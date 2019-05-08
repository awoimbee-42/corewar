/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vm.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/06 15:24:23 by awoimbee          #+#    #+#             */
/*   Updated: 2019/05/08 02:37:46 by awoimbee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VM_H
# define VM_H
# include "corewar.h"
# include "libft.h"

/*
**	################
**	    Defines
**	################
*/

# define RESERVED_ID -2147483648

/*
**	#################
**	   Declarations
**	#################
*/

typedef uint32_t	t_register;
struct s_proc;
struct s_play;
struct s_vecproc;
struct s_vecplay;
struct s_vm;

/*
**	################
**	   Definitions
**	################
*/

typedef struct	s_proc
{
	t_register			reg[REG_NUMBER];
	t_register			pc;
	t_bool				carry;
}				t_proc;

typedef struct	s_vecproc
{
	struct s_proc		*d;
	unsigned int		wait_cnt;
	size_t				len;
	size_t				mem; // std::vector<t_proc>
}				t_vecproc;

typedef struct	s_play
{
	int					id;
	t_bool				alive_flag;
	char				*cor;
	t_header			*head;
	ssize_t				cor_len;
	struct s_vecproc	procs;
}				t_play;

typedef struct	s_vecplay
{
	struct s_play		*d;
	size_t				len;
	size_t				mem;
}				t_vecplay;

/*
**	#################
**	  MAIN STRUCT
**	#################
*/
typedef struct	s_vm
{
	size_t				cycle_dump;
	size_t				cycle_die;
	size_t				cycle_curr;
	char				arena[MEM_SIZE];
	struct s_vecplay	players;
	struct s_garbage	gb;
}				t_vm;

/*
**	##################
**	    OPERATORS
**	##################
*/

/*
**	Check for MAX_PLAYERS is done inside these functions, dont worry about it
*/

t_vecplay		*vecplay_init(t_garbage *gb, t_vecplay *vec, size_t reserv_len);
t_vecplay		*vecplay_new(t_garbage *gb, size_t reserved_len);
t_vecplay		*vecplay_push_empty(t_garbage *gb, t_vecplay *vec);
t_vecplay		*vecplay_push(t_garbage *gb, t_vecplay *vec, t_play d);
t_vecplay		*vecplay_realloc(t_garbage *gb, t_vecplay *vec);
t_vecplay		*vecplay_del_at(t_vecplay *v, size_t at);
t_play			*vecplay_point_last(const t_vecplay *vec);

t_vecproc		*vecproc_init(t_garbage *gb, t_vecproc *vec, size_t reserv_len);
t_vecproc		*vecproc_new(t_garbage *gb, size_t reserved_len);
t_vecproc		*vecproc_push_empty(t_garbage *gb, t_vecproc *vec);
t_vecproc		*vecproc_push(t_garbage *gb, t_vecproc *vec, t_proc d);
t_vecproc		*vecproc_realloc(t_garbage *gb, t_vecproc *vec);
t_vecproc		*vecproc_del_at(t_vecproc *v, size_t at);

/*
**	Functions
*/

/*
**
*/
void			exit_vm(t_vm *env, char *err_msg);

/*
**
*/
void			read_argv(t_vm *env, int argc, char **argv);

#endif
