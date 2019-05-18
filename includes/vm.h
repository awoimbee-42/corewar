/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vm.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/06 15:24:23 by awoimbee          #+#    #+#             */
/*   Updated: 2019/05/18 16:29:51 by awoimbee         ###   ########.fr       */
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

# define RESERVED_ID	-2147483648

/*
**	Verbosity levels are defined so new ones can be added in between easily
*/

# define VE_VI_NOTINIT	-2
# define VE_VISU		-1
# define VE_WINNER		0
# define VE_AFF			1
# define VE_LIVE		2
# define VE_GREET		3
# define VE_PLAYDEATH	4
# define VE_PROCDEATH	5
# define VE_OPS			6
# define VE_CYCLE		7

/*
**	Ncurses sucks and doesnt define all the colors
*/

# define COLOR_BRIGHT_BLACK		8
# define COLOR_BRIGHT_RED		9
# define COLOR_BRIGHT_GREEN		10
# define COLOR_BRIGHT_YELLOW	11
# define COLOR_BRIGHT_BLUE		12
# define COLOR_BRIGHT_MAGENTA	13
# define COLOR_BRIGHT_CYAN		14
# define COLOR_BRIGHT_WHITE		15

/*
**	Define the COLOR_PAIR we'll use for players, cursors & newly written mem,
**		1 PLAY0_COLOR-1 & CURS0_COLOR-1 are reserved for unset memory.
**	/!\ code assumes CONTOUR_COLOR < FRESH0_COLOR
*/

# define CONTOUR_COLOR		1
# define PLAY0_COLOR		3
# define DELT_CURS_COLOR	10
# define CURS0_COLOR		13
# define DELT_FRESH_COLOR	20
# define FRESH0_COLOR		23

/*
**	#################
**	   Declarations
**	#################
*/

typedef int32_t	t_register;
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
	t_register			reg[REG_NUMBER + 4];
	t_register			pc;
	t_register			new_pc;
	int					live;
	int					op_id;    // The op code inside the arena can be updated while the process is waiting fo op_cycles !!
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
	// int					index;
	int					id;
	int					last_live;
	int					period_lives;
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
**	+-------------------------+------------------------------+
**	|                         |           sidepw             |
**	|                         |                              |
**	|                         |  +------------------------+  |
**	|                         |  |      spstatusw         |  |
**	|         arenaw          |  |                        |  |
**	|                         |  +------------------------+  |
**	|                         |                              |
**	|                         |                              |
**	|                         |                              |
**	|                         |                              |
**	|                         |                              |
**	|                         |                              |
**	|                         |                              |
**	+-------------------------+------------------------------+
*/

typedef struct	s_visu
{
	float				op_per_sec;
	t_bool				paused;
	WINDOW				*rootw;
	WINDOW				*arenaw;
	struct
	{
		WINDOW				*rootw;
		WINDOW				*statusw;
	}					sidep;
}				t_visu;


/*
**	OPs
*/

typedef void(*t_opfun)(struct s_vm*, t_play*, t_proc*, int[3]);

typedef struct	s_vm_op
{
	char				name[5];
	int					nb_args;
	t_arg_type			args_type[3]; // 3 is the max number of args -> the compiler will round to 4 anyways
	int					id; //op_code
	int					cycles;
	t_bool				coding_byte;
	t_bool				modcarry;
	t_bool				dir2;
	t_bool				ldx_rel;
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
	int					dump_width;
	int					cycle_die;
	int					cycle_curr;
	int					die_cycle_checks;
	struct s_vecplay	players;
	struct s_garbage	gb;
	uint8_t				arena[MEM_SIZE];
	uint8_t				mem_owner[MEM_SIZE];
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
void			print_memory(t_vm *env, const void *addr);

/*
**	arena_mem_*.c
**		The write functions need the procs PC
**		to be located on the instructions id byte.
*/
int32_t			load32(t_vm *vm, t_register pc);
int16_t			load16(t_vm *vm, t_register pc);
int8_t			load8(t_vm *vm, t_register pc);
uint8_t			*write32(t_vm *vm, t_proc *proc, int aptr, uint32_t data);
uint8_t			*write16(t_vm *vm, t_proc *proc, int aptr, uint16_t data);
uint8_t			*write8(t_vm *vm, t_proc *proc, int aptr, uint8_t data);
int				circumem(int ptr);

/*
**
*/
int				usage(const char *pname);
void			exit_vm(t_vm *env, char *err_msg);

int				run_vm_cycle(t_vm *vm);
void			loop(t_vm *env);
void			visu_loop(t_vm *vm);

/* ops.c */
void			launch_instruction(t_vm *vm, t_play *play, t_proc *proc);

/* visu */
void			clean_visu(t_vm *vm);
void			visu_update(t_vm *vm);
void			visu_khandler(t_vm *vm);
void			visu_init_memview(t_vm *vm);
void			visu_memview(t_vm *vm);
void			visu_sidepview(t_vm *vm);

/*
**
*/
void			read_argv_init(t_vm *env, int argc, char **argv);

/*
**	OPs
*/
void		read_instruction(t_proc *proc, t_vm *env);
int			read_one_arg(t_vm *vm, t_proc *proc, uint8_t cb, int cur_arg);
int			load_arg_into_regs(t_vm *vm, t_play *play, t_proc *proc, int reg_num[3]);

void		op_live(t_vm *vm, t_play *p, t_proc *proc, int reg_num[3]);
void		op_ld(t_vm *vm, t_play *p, t_proc *proc, int reg_num[3]);
void		op_st(t_vm *vm, t_play *p, t_proc *proc, int reg_num[3]);
void		op_add(t_vm *vm, t_play *p, t_proc *proc, int reg_num[3]);
void		op_sub(t_vm *vm, t_play *p, t_proc *proc, int reg_num[3]);
void		op_and(t_vm *vm, t_play *play, t_proc *proc, int reg_num[3]);
void		op_or(t_vm *vm, t_play *play, t_proc *proc, int reg_num[3]);
void		op_xor(t_vm *vm, t_play *play, t_proc *proc, int reg_num[3]);
void		op_zjmp(t_vm *vm, t_play *p, t_proc *proc, int reg_num[3]);
void		op_ldi(t_vm *vm, t_play *play, t_proc *proc, int reg_num[3]);
void		op_sti(t_vm *vm, t_play *play, t_proc *proc, int reg_num[3]);
void		op_fork(t_vm *vm, t_play *p, t_proc *proc, int reg_num[3]);
void		op_lld(t_vm *vm, t_play *play, t_proc *proc, int reg_num[3]);
void		op_lldi(t_vm *vm, t_play *play, t_proc *proc, int reg_num[3]);
void		op_lfork(t_vm *vm, t_play *p, t_proc *proc, int reg_num[3]);
void		op_aff(t_vm *vm, t_play *p, t_proc *proc, int reg_num[3]);
#endif
