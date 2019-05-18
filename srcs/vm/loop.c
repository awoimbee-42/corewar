/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/08 17:59:57 by awoimbee          #+#    #+#             */
/*   Updated: 2019/05/16 13:56:01 by awoimbee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <time.h>
#include "vm.h"

void			loop(t_vm *env)
{
	int		cycle;

	cycle = 0;
	while (run_vm_cycle(env))
	{
		cycle++;
		if (env->cycle_dump == cycle)
		{
			print_memory(env, env->arena);
			break;
		}
	}
}

void			visu_loop(t_vm *vm)
{
	clock_t		t;
	int			alive;

	alive = 1;
	while (alive)
	{
		t = clock();
		if (!vm->visu.paused)
			alive = run_vm_cycle(vm);
		visu_update(vm);

		if ((t = (clock() - t) / CLOCKS_PER_SEC) < (1. / vm->visu.op_per_sec))
			// timeout(((1. / vm->visu.op_per_sec) - t) * 1000);
			usleep(((1. / vm->visu.op_per_sec) - t) * 1000000);
	}
}
