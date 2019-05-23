/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_argv.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/07 19:24:05 by awoimbee          #+#    #+#             */
/*   Updated: 2019/05/23 13:17:13 by awoimbee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "corewar.h"
#include "vm.h"
#include "init.h"
#include <errno.h>
#include <time.h>

static void		read_dump_cycle(t_vm *env, char *input)
{
	if (env->dump_width)
		exit_vm(env, "Dump cycle set multiple times !");
	env->dump_width = 32;
	if (input == NULL
		|| ft_strlen(input) > 10
		|| (env->cycle_dump = ft_atoi(input)) <= 0)
		exit_vm(env, gb_add(&env->gb,
			ft_cprintf("Dump cycle badly formatted ('%s')", input)));
}

static void		read_ndump_cycle(t_vm *env, char **argv, int *agid)
{
	if (env->dump_width)
		exit_vm(env, "Dump cycle set multiple times !");
	++(*agid);
	if (argv[*agid] == NULL
		|| ft_strlen(argv[*agid]) > 10
		|| (env->dump_width = ft_atoi(argv[*agid])) <= 0)
		exit_vm(env, gb_add(&env->gb,
			ft_cprintf("Dump width badly formatted ('%s')", argv[*agid])));
	++(*agid);
	if (argv[*agid] == NULL
		|| ft_strlen(argv[*agid]) > 10
		|| (env->cycle_dump = ft_atoi(argv[*agid])) <= 0)
		exit_vm(env, gb_add(&env->gb,
			ft_cprintf("Dump cycle badly formatted ('%s')", argv[*agid])));
}

/*
**	iterate over all the players:
**		if play id not set:
**			champ_id = player index
**			while play id not set:
**				increment champ_id
**				iterate over players and break if champ_id already taken
**				if champ_id not taken:
**					set player id to champ_id
*/

static void		set_remaining_play_id(t_vm *vm)
{
	int			pl;
	int			j;
	int			champ_id;

	pl = -1;
	while (++pl < vm->players.len)
	{
		if (vm->players.d[pl].id == RESERVED_ID)
		{
			champ_id = pl;
			while (vm->players.d[pl].id == RESERVED_ID)
			{
				++champ_id;
				j = 0;
				while (j < vm->players.len && vm->players.d[j].id != champ_id)
					++j;
				if (j == vm->players.len || vm->players.d[j].id != champ_id)
					vm->players.d[pl].id = champ_id;
			}
		}
	}
}

/*
**	Verbosity st to -1 means visualizer & the exit function WILL CRASH if
**		verbosity is set to -1 but the visualizer wasn't initialized
*/

void			set_verbosity(t_vm *vm, char *input)
{
	if (vm->verbosity == VE_VISU || vm->verbosity == VE_VI_NOTINIT)
		exit_vm(vm, "Cannot set verbosity in visual mode!");
	if (!input)
		exit_vm(vm, "Need a verbosity level (wtf are u trying to achieve ?)");
	vm->verbosity = ft_atoi(input);
	if (vm->verbosity < 0)
		vm->verbosity = VE_BAD;
	if (input == NULL
		|| vm->verbosity < 0
		|| (*input < '0' || *input > '9')
		|| ft_strlen(input) > 10)
		exit_vm(vm, gb_add(&vm->gb,
			ft_cprintf("Verbosity level badly formatted ('%s')", input)));
}

void			arena_fill_rand(t_vm *vm)
{
	int				i;

	srandom(time(NULL));
	i = -1;
	while (++i < MEM_SIZE)
		vm->arena[i] = random();
}

void			read_argv_init(t_vm *env, int argc, char **argv)
{
	int			i;

	env->cycle_die = CYCLE_TO_DIE;
	i = 0;
	while (++i < argc)
	{
		if (!ft_strcmp(argv[i], "-dump"))
			read_dump_cycle(env, argv[++i]);
		else if (!ft_strcmp(argv[i], "-ndump"))
			read_ndump_cycle(env, argv, &i);
		else if (!ft_strcmp(argv[i], "-visu") || !ft_strcmp(argv[i], "-vi"))
			env->verbosity = VE_VI_NOTINIT;
		else if (!ft_strcmp(argv[i], "-verbose") || !ft_strcmp(argv[i], "-ve"))
			set_verbosity(env, argv[++i]);
		else if (!ft_strcmp(argv[i], "-rand"))
			arena_fill_rand(env);
		else
			i = read_champ(env, argv, i);
	}
	if (!env->players.len)
		exit_vm(env, "At least one player is required");
	set_remaining_play_id(env);
	champs_setup(env);
	if (env->verbosity == VE_VI_NOTINIT)
		init_ncurses(env);
}
