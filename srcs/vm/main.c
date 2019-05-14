/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/02 14:51:50 by awoimbee          #+#    #+#             */
/*   Updated: 2019/05/14 15:15:09 by awoimbee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// each process has :
// 	REG_NUMBER registers, each of size REG_SIZE bytes, All registers but the PC are set to 0 by default
// 	A Programm Counter: a special register containing the address of the next instruction
// 	A 'carry' flag

// each player has:
// 	An id number, written in the r1 register of their first process at startup
// 	They are space equivalently in ram

// the vm should create a memory space dedicated to combat, will load the champions inside and their processes and execute everything until death

// every CYCLE_TO_DIE (...)

// ./corewar [-dump nbr_cycles] [[-n number] champion1.cor] ...

#include "corewar.h"
#include "vm.h"

const t_vm_op g_op[16] = {
	//  name - nb_args -- args_types --                id -- nb_cycles -- uses_coding_byte -- modcarry -- dir2 -- ldx_mod --  fun
	{"live", 1, {T_DIR},                                                1,   10, 0, 0, 0, 0, op_live},
	{"ld",   2, {T_DIR | T_IND, T_REG},                                 2,    5, 1, 1, 0, 0, op_ld},
	{"st",   2, {T_REG, T_IND | T_REG},                                 3,    5, 1, 0, 0, 0, op_st},
	{"add",  3, {T_REG, T_REG, T_REG},                                  4,   10, 1, 1, 0, 0, op_add},
	{"sub",  3, {T_REG, T_REG, T_REG},                                  5,   10, 1, 1, 0, 0, op_sub},
	{"and",  3, {T_REG | T_DIR | T_IND, T_REG | T_IND | T_DIR, T_REG},  6,    6, 1, 1, 0, 0, op_live},
	{"or",   3, {T_REG | T_IND | T_DIR, T_REG | T_IND | T_DIR, T_REG},  7,    6, 1, 1, 0, 0, op_live},
	{"xor",  3, {T_REG | T_IND | T_DIR, T_REG | T_IND | T_DIR, T_REG},  8,    6, 1, 1, 0, 0, op_live},
	{"zjmp", 1, {T_DIR},                                                9,   20, 0, 0, 1, 0, op_zjmp},
	{"ldi",  3, {T_REG | T_DIR | T_IND, T_DIR | T_REG, T_REG},         10,   25, 1, 0, 1, 0, op_live},
	{"sti",  3, {T_REG, T_REG | T_DIR | T_IND, T_DIR | T_REG},         11,   25, 1, 0, 1, 0, op_live},
	{"fork", 1, {T_DIR},                                               12,  800, 0, 0, 1, 0, op_fork},
	{"lld",  2, {T_DIR | T_IND, T_REG},                                13,   10, 1, 1, 0, 1, op_live},
	{"lldi", 3, {T_REG | T_DIR | T_IND, T_DIR | T_REG, T_REG},         14,   50, 1, 1, 1, 1, op_live},
	{"lfork",1, {T_DIR},                                               15, 1000, 0, 0, 1, 1, op_lfork},
	{"aff",  1, {T_REG},                                               16,    2, 1, 0, 0, 0, op_aff},

};

void	exit_vm(t_vm *env, char *err_msg)
{
	size_t		msg_len;

	msg_len = ft_strlen(err_msg);
	err_msg[msg_len] = '\n';
	write(2, err_msg, msg_len + 1);
	gb_freeall(&env->gb);
	exit(EXIT_FAILURE);
}

int		usage(void)
{
	ft_printf("<bold>{inv}va bien niquer ta grosse mère fdp<rst>\n");
	return (0);
}

int		main(int argc, char **argv)
{
	t_vm		env;

	if (argc == 1)// || argc > MAX_ARGS_NUMBER)
		return(usage());
	ft_bzero(&env, sizeof(env));
	ft_printf("gb len addr %p status: %lu\n", &env.gb.arr_len, env.gb.arr_len);
	gb_init_existing(&env.gb);
	ft_printf("gb len addr %p status: %lu\n", &env.gb.arr_len, env.gb.arr_len);
	read_argv_init(&env, argc, argv);
	env.verbosity = 4;

	{
		ft_printf("Our contestants are:\n");
		for (int i = 0; i < env.players.len; ++i)
			ft_printf("\tJean michel %s #%d avec un programme d'une taille de %ld octets\n", env.players.d[i].head.prog_name, env.players.d[i].id, env.players.d[i].head.prog_size);
	}
	ft_printf("Arena:\n");
	print_memory(env.arena, MEM_SIZE);

	loop(&env);
	gb_freeall(&env.gb);
	return (0);
}