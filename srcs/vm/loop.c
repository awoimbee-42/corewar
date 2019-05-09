/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/08 17:59:57 by awoimbee          #+#    #+#             */
/*   Updated: 2019/05/09 22:25:12 by awoimbee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "corewar.h"
#include "vm.h"

//	si le cycle_to_die tombe en meme temps qu'une instruction live on fait quoi ?
//	quand die_cycle_checks == MAX_CHECKS on décrémente de 1 ou de CYCLE_DELTA ?

void			read_instruction(t_proc *proc, t_play *play, t_vm *env)
{
	t_op	*op;
	int		op_id;

	op_id = env->arena[proc->pc] - 1;
	if (op_id < 1 || 16 < op_id)
		ft_printf("\t\t\t{red}invalid op: %#llx -> %lld, PC: %d{eoc}\n", op_id, op_id, proc->pc);                             // ignore and continue reading until next valid instruction byte
	op = &op_tab[op_id];
	ft_printf("\t\t\tOP: %s\n", op->name);
	env->opfuns[op_id](env, play, proc);
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
			ft_printf("Process %lu of player %d died because he thought that live was not an important instruction\n", i, p->id);
			vecproc_del_at(&p->procs, i);
		}
		--p->procs.d[i].op_cycles;
		if (p->procs.d[i].op_cycles == -1) // begining, read next instruction
			read_instruction(&p->procs.d[i], p, env);
		if (p->procs.d[i].op_cycles == 0) // launch instruction and read next one
			;
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
