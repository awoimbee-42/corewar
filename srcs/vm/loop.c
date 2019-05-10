/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/08 17:59:57 by awoimbee          #+#    #+#             */
/*   Updated: 2019/05/10 16:43:37 by awoimbee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "corewar.h"
#include "vm.h"

//	si le cycle_to_die tombe en meme temps qu'une instruction live on fait quoi ?
//	quand die_cycle_checks == MAX_CHECKS on décrémente de 1 ou de CYCLE_DELTA ?

void			read_instruction(t_proc *proc, t_play *play, t_vm *env)
{
	int		op_id;

	op_id = env->arena[proc->pc] - 1;
	if (op_id < 0 || 15 < op_id)
	{
		ft_printf("\t\t\t{red}load fail op: %#llx -> %lld, PC: %d{eoc}\n", op_id, op_id, proc->pc);                             // ignore and continue reading until next valid instruction byte
		proc->pc++;
		proc->op_cycles = 0;
	}
	else
	{
		proc->op_cycles = g_op[op_id].nb_cycles;
		ft_printf("\t\t\t{blu}load OP: %s{eoc}\n", g_op[op_id].name);
	}
}

void			launch_instruction(t_vm *vm, t_play *play, t_proc *proc)
{
	int		op_id;

	op_id = vm->arena[proc->pc] - 1;
	g_op[op_id].fun(vm, play, proc);
}

static int		loop_player(t_vm *env, t_play *p)
{
	int			i;

	ft_printf("\tPlayer: %d\n", p->id);
	i = -1;
	while (++i < p->procs.len)
	{
		ft_printf("\t\tProcess: %d\n", i);
		if (!(env->cycle_curr % env->cycle_die) && !p->procs.d[i].live)
		{
			ft_printf("\t\tProcess %lu of player %d died because he thought that live was not an important instruction\n", i, p->id);
			vecproc_del_at(&p->procs, i);
		}
		--p->procs.d[i].op_cycles;
		if (p->procs.d[i].op_cycles == 0 && --p->procs.d[i].op_cycles) // launch instruction
			launch_instruction(env, p, &p->procs.d[i]);
		if (p->procs.d[i].op_cycles == -1) // read next instruction
			read_instruction(&p->procs.d[i], p, env);
	}
	if (p->procs.len == 0)
		return (0);
	return (1);
}

void			loop(t_vm *env)
{
	int			i;
	int			alive;

	alive = 1;
	while (alive)
	{
		++env->cycle_curr;
		ft_printf("{PNK}cycle: %lu{eoc}\n", env->cycle_curr);
		if (!(env->cycle_curr % env->cycle_die))
			env->die_cycle_checks++;
		if (env->die_cycle_checks == MAX_CHECKS)
		{
			env->die_cycle_checks = 0;
			if ((env->cycle_die -= 1) <= 0) // should I decrement by 1 on CYCLE DELTA ?
				env->cycle_die = 1;
		}
		alive = 0;
		i = -1;
		while (++i < env->players.len)
		{
			if (loop_player(env, &env->players.d[i]) == 1) // if (something); {} <- worst fucking bug
				alive = 1;
		}
	}
}
