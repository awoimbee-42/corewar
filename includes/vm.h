/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vm.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/06 15:24:23 by awoimbee          #+#    #+#             */
/*   Updated: 2019/05/13 04:03:42 by awoimbee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VM_H
# define VM_H
# include "corewar.h"
# include "libft.h"
# include <ncurses.h>
# include <pthread.h>

/*
**	################
**	    Defines
**	################
*/

# define RESERVED_ID -2147483648
# define COLOR_BRIGHT_BLACK 8
# define COLOR_BRIGHT_RED 9
# define COLOR_BRIGHT_GREEN 10
# define COLOR_BRIGHT_YELLOW 11
# define COLOR_BRIGHT_BLUE 12
# define COLOR_BRIGHT_MAGENTA 13
# define COLOR_BRIGHT_CYAN 14
# define COLOR_BRIGHT_WHITE 15

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
	uint8_t				*cor; // useless ?
	t_header			head;
	struct s_vecproc	procs;
}				t_play;

typedef struct	s_vecplay
{
	struct s_play		d[MAX_PLAYERS];
	int					len;
}				t_vecplay;

/*
**
*/

typedef struct	s_visu
{
	pthread_t			thread;
	WINDOW				*rootw;
	WINDOW				*arenaw;
	WINDOW				*sidepw;
}				t_visu;


/*
**	OPs
*/

typedef void(*t_opfun)(struct s_vm*, t_play*, t_proc*);

typedef struct	s_vm_op
{
	char				name[5];
	int					nb_cycles;
	t_opfun				fun;
}				t_vm_op;

extern const t_vm_op g_op[16];

/*
**	#################
**	  MAIN STRUCT
**	#################
*/


typedef struct	s_vm
{
	t_visu				visu;
	int					verbosity;
	int					cycle_dump;
	int					cycle_die;
	int					cycle_curr;
	int					die_cycle_checks;
	struct s_vecplay	players;
	struct s_garbage	gb;
	uint8_t				arena[MEM_SIZE];
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
uint32_t		swap32_endian(uint32_t val);
uint16_t		swap16_endian(uint16_t val);
void			print_memory(const void *addr, size_t size);
uint32_t		load32(t_vm *vm, t_register pc);
uint16_t		load16(t_vm *vm, t_register pc);
uint8_t			load8(t_vm *vm, t_register pc);

/*
**
*/
int				usage(void);
void			exit_vm(t_vm *env, char *err_msg);

void			loop(t_vm *env);

void			visu_loop(t_vm *vm);
void			visu_init_memview(t_vm *vm);

/*
**
*/
void			read_argv_init(t_vm *env, int argc, char **argv);

/*
**	OPs
*/
void		op_live(t_vm *vm, t_play *p, t_proc *proc);
void		op_ld(t_vm *vm, t_play *p, t_proc *proc);
void		op_st(t_vm *vm, t_play *p, t_proc *proc);
void		op_add(t_vm *vm, t_play *p, t_proc *proc);
void		op_sub(t_vm *vm, t_play *p, t_proc *proc);
void		op_zjmp(t_vm *vm, t_play *p, t_proc *proc);
void		op_fork(t_vm *vm, t_play *p, t_proc *proc);
void		op_lfork(t_vm *vm, t_play *p, t_proc *proc);
void		op_aff(t_vm *vm, t_play *p, t_proc *proc);

#endif
