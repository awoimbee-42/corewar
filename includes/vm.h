/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vm.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/06 15:24:23 by awoimbee          #+#    #+#             */
/*   Updated: 2019/05/08 17:49:02 by awoimbee         ###   ########.fr       */
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
	size_t				len;
	size_t				mem;
}				t_vecproc;

typedef struct	s_play
{
	int					id;
	char				*cor;
	t_header			head;
	struct s_vecproc	procs;
}				t_play;

typedef struct	s_vecplay
{
	struct s_play		d[MAX_PLAYERS];
	int					len;
}				t_vecplay;

/*
**	#################
**	  MAIN STRUCT
**	#################
*/
typedef struct	s_vm
{
	unsigned int		cycle_dump;
	unsigned int		cycle_die;
	unsigned int		cycle_curr;
	unsigned int		die_cycle_checks;
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

// t_vecplay		*vecplay_init(t_garbage *gb, t_vecplay *vec, size_t reserv_len);
// t_vecplay		*vecplay_new(t_garbage *gb, size_t reserved_len);
// t_vecplay		*vecplay_push_empty(t_garbage *gb, t_vecplay *vec);
// t_vecplay		*vecplay_push(t_garbage *gb, t_vecplay *vec, t_play d);
// t_vecplay		*vecplay_realloc(t_garbage *gb, t_vecplay *vec);
// t_vecplay		*vecplay_del_at(t_vecplay *v, size_t at);
// t_play			*vecplay_point_last(const t_vecplay *vec);

t_vecproc		*vecproc_init(t_garbage *gb, t_vecproc *vec, size_t reserv_len);
t_vecproc		*vecproc_new(t_garbage *gb, size_t reserved_len);
t_vecproc		*vecproc_push_empty(t_garbage *gb, t_vecproc *vec);
t_vecproc		*vecproc_push(t_garbage *gb, t_vecproc *vec, t_proc d);
t_vecproc		*vecproc_realloc(t_garbage *gb, t_vecproc *vec);
t_vecproc		*vecproc_del_at(t_vecproc *v, size_t at);

/*
**	Functions
*/
uint			swap32_endianess(uint val);
void			print_memory(const void *addr, size_t size);

/*
**
*/
int				usage(void);
void			exit_vm(t_vm *env, char *err_msg);

/*
**
*/
void			read_argv_init(t_vm *env, int argc, char **argv);

#endif
