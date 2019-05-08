/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/02 14:51:50 by awoimbee          #+#    #+#             */
/*   Updated: 2019/05/08 18:52:08 by awoimbee         ###   ########.fr       */
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
	gb_init_existing(&env.gb);
	read_argv_init(&env, argc, argv);

	{
		ft_printf("Our contestants are:\n");
		for (size_t i = 0; i < env.players.len; ++i)
			ft_printf("\tJean michel %s #%d avec un programme d'une taille de %ld octets\n", env.players.d[i].head.prog_name, env.players.d[i].id, env.players.d[i].head.prog_size);
	}
	ft_printf("Arena:\n");
	print_memory(env.arena, MEM_SIZE);

	loop(&env);
	gb_freeall(&env.gb);
	return (0);
}