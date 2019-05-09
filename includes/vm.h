/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vm.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/06 15:24:23 by awoimbee          #+#    #+#             */
/*   Updated: 2019/05/09 20:09:10 by awoimbee         ###   ########.fr       */
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
	int					live;
	int					op_cycles;
	t_bool				carry;
}				t_proc;

typedef struct	s_vecproc
{
	struct s_proc		*d;
	int					len;
	int					mem;
}				t_vecproc;

typedef struct	s_play
{
	int					id;
	char				*cor; // useless ?
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
typedef int(*t_opfun)(struct s_vm*, t_play*, t_proc*);

typedef struct	s_vm
{
	t_opfun				opfuns[16];
	int					op_ocps[16];
	int					cycle_dump;
	int					cycle_die;
	int					cycle_curr;
	int					die_cycle_checks;
	struct s_vecplay	players;
	struct s_garbage	gb;
	char				arena[MEM_SIZE];
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
t_vecproc		*vecproc_del_at(t_vecproc *v, int at);

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

void			loop(t_vm *env);

/*
**
*/
void			read_argv_init(t_vm *env, int argc, char **argv);

/*
**	OPs
*/
int			op_live(t_vm *vm, t_play *p, t_proc *proc);
int			op_ld(t_vm *vm, t_play *p, t_proc *proc);
int			op_st(t_vm *vm, t_play *p, t_proc *proc);
int			op_add(t_vm *vm, t_play *p, t_proc *proc);
int			op_sub(t_vm *vm, t_play *p, t_proc *proc);
int			op_zjmp(t_vm *vm, t_play *p, t_proc *proc);
int			op_fork(t_vm *vm, t_play *p, t_proc *proc);
int			op_lfork(t_vm *vm, t_play *p, t_proc *proc);
int			op_aff(t_vm *vm, t_play *p, t_proc *proc);

#endif
