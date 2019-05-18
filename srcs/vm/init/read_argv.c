/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_argv.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/07 19:24:05 by awoimbee          #+#    #+#             */
/*   Updated: 2019/05/17 23:22:47 by awoimbee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "corewar.h"
#include "vm.h"
#include "init.h"
#include <errno.h>

static void		read_dump_cycle(t_vm *env, char *input)
{
	if (input == NULL
		|| ft_strlen(input) > 10
		|| (env->cycle_dump = ft_atoi(input)) <= 0)
		exit_vm(env, gb_add(&env->gb,
			ft_cprintf("Dump cycle badly formatted ('%s')", input)));
}

static void		set_remaining_play_id(t_vm *env)
{
	int			i;
	int			j;
	int			champ_id;

	i = -1;
	while (++i < env->players.len)
	{
		if (env->players.d[i].id == RESERVED_ID)
		{
			champ_id = i;
			while (env->players.d[i].id == RESERVED_ID)
			{
				++champ_id;
				j = -1;
				while (++j < env->players.len)
					if (env->players.d[j].id == champ_id)
						break ;
				if (env->players.d[j].id != champ_id)
					env->players.d[i].id = champ_id;
			}
		}
		env->players.d[i].procs.d[0].reg[1] = env->players.d[i].id;
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
		exit_vm(vm, "Mbenjell fuck off");
	vm->verbosity = ft_atoi(input);
	if (vm->verbosity < 0)
		vm->verbosity = 0;
	if (input == NULL
		|| ft_strlen(input) > 10
		|| vm->verbosity < 0)
		exit_vm(vm, gb_add(&vm->gb,
			ft_cprintf("Verbosity level badly formatted ('%s')", input)));
}

void			read_argv_init(t_vm *env, int argc, char **argv)
{
	int			i;

	env->cycle_die = CYCLE_TO_DIE;
	i = 0;
	while (++i < argc)
	{
		// ft_printf("--reading argument #%d--\n", i); // REMOVE
		if (!ft_strcmp(argv[i], "-dump"))
			read_dump_cycle(env, argv[++i]);
		else if (!ft_strcmp(argv[i], "-visu") || !ft_strcmp(argv[i], "-vi"))
			env->verbosity = VE_VI_NOTINIT;
		else if (!ft_strcmp(argv[i], "-verbose") || !ft_strcmp(argv[i], "-ve"))
			set_verbosity(env, argv[++i]);
		else
			i = read_champ(env, argv, i);
	}
	if (!env->players.len)
		exit_vm(env, "At least one player is required");
	set_remaining_play_id(env);
	load_cor_files(env); // also sets the PC & r0
	if (env->verbosity == VE_VI_NOTINIT)
		init_ncurses(env);
}
