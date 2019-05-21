/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/08 17:59:57 by awoimbee          #+#    #+#             */
/*   Updated: 2019/05/21 17:03:40 by awoimbee         ###   ########.fr       */
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
		(t = (clock() - t) / CLOCKS_PER_SEC);
		if (t < (1. / vm->visu.op_per_sec))
			usleep(((1. / vm->visu.op_per_sec) - t) * 1000000);
	}
}
