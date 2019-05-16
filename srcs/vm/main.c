/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/02 14:51:50 by awoimbee          #+#    #+#             */
/*   Updated: 2019/05/16 14:23:50 by awoimbee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// ./corewar [-dump nbr_cycles] [[-n number] champion1.cor] ...

#include "corewar.h"
#include "vm.h"

const t_vm_op g_op[16] = {
	//  name - nb_args -- args_types --                          id -- nb_cycles -- uses_coding_byte -- modcarry -- dir2 -- ldx_mod --  fun
	{"live", 1, {T_DIR},                                                1,   10, 0, 0, 0, 0, op_live},
	{"ld",   2, {T_DIR | T_IND, T_REG},                                 2,    5, 1, 1, 0, 0, op_ld},
	{"st",   2, {T_REG, T_IND | T_REG},                                 3,    5, 1, 0, 0, 0, op_st},
	{"add",  3, {T_REG, T_REG, T_REG},                                  4,   10, 1, 1, 0, 0, op_add},
	{"sub",  3, {T_REG, T_REG, T_REG},                                  5,   10, 1, 1, 0, 0, op_sub},
	{"and",  3, {T_REG | T_DIR | T_IND, T_REG | T_IND | T_DIR, T_REG},  6,    6, 1, 1, 0, 0, op_and},
	{"or",   3, {T_REG | T_IND | T_DIR, T_REG | T_IND | T_DIR, T_REG},  7,    6, 1, 1, 0, 0, op_or},
	{"xor",  3, {T_REG | T_IND | T_DIR, T_REG | T_IND | T_DIR, T_REG},  8,    6, 1, 1, 0, 0, op_xor},
	{"zjmp", 1, {T_DIR},                                                9,   20, 0, 0, 1, 0, op_zjmp},
	{"ldi",  3, {T_REG | T_DIR | T_IND, T_DIR | T_REG, T_REG},         10,   25, 1, 0, 1, 0, op_ldi},
	{"sti",  3, {T_REG, T_REG | T_DIR | T_IND, T_DIR | T_REG},         11,   25, 1, 0, 1, 0, op_sti},
	{"fork", 1, {T_DIR},                                               12,  800, 0, 0, 1, 0, op_fork},
	{"lld",  2, {T_DIR | T_IND, T_REG},                                13,   10, 1, 1, 0, 1, op_lld},
	{"lldi", 3, {T_REG | T_DIR | T_IND, T_DIR | T_REG, T_REG},         14,   50, 1, 1, 1, 1, op_lldi},
	{"lfork",1, {T_DIR},                                               15, 1000, 0, 0, 1, 1, op_lfork},
	{"aff",  1, {T_REG},                                               16,    2, 1, 0, 0, 0, op_aff},

};

/*
**	Lots of times we pass strerror() to exit_vm,
**	this function should return a const char* (according to man),
**		but macOS is a special little snowflake and returns a malloced string.
*/

void	exit_vm(t_vm *env, char *err_msg)
{
	if (env->verbosity == -1)
	{
		delwin(env->visu.arenaw);
		delwin(env->visu.sidep.rootw);
		delwin(env->visu.sidep.statusw);
		echo();
		nocbreak();
		curs_set(TRUE);
		reset_shell_mode();
		endwin();
	}
	write(2, "Error:\n", 7);
	write(2, err_msg, ft_strlen(err_msg));
	write(2, "\n", 1);
	gb_freeall(&env->gb);
	exit(EXIT_FAILURE);
}

int		usage(const char *pname)
{
	ft_printf("<bold>Usage: %s"
		" [-visu]"
		" [-dump nbr_cycles (not implemented yet)]"
		" [[-n number] champion.cor]<rst>\n", pname);
	return (0);
}

int		main(int argc, char **argv)
{
	t_vm		vm;

	if (argc == 1)// || argc > MAX_ARGS_NUMBER)
		return(usage(argv[0]));
	ft_bzero(&vm, sizeof(vm));
	gb_init_existing(&vm.gb);
	read_argv_init(&vm, argc, argv);
	if (vm.verbosity > 0)
	{
		ft_printf("Our contestants are:\n");
		for (int i = 0; i < vm.players.len; ++i)
			ft_printf("\tJean michel %s #%d avec un programme d'une taille de %ld octets\n", vm.players.d[i].head.prog_name, vm.players.d[i].id, vm.players.d[i].head.prog_size);
		ft_printf("Arena:\n");
		print_memory(vm.arena, MEM_SIZE);
	}
	if (vm.verbosity == -1)
		visu_loop(&vm);
	else
		loop(&vm);
	gb_freeall(&vm.gb);
	return (0);
}