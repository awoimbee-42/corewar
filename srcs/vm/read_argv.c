/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_argv.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/07 19:24:05 by awoimbee          #+#    #+#             */
/*   Updated: 2019/05/09 14:24:32 by awoimbee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "corewar.h"
#include "vm.h"
#include <fcntl.h>
#include <errno.h>

/*
**	Filename is put in t_play.cor by read_champ()
**		it is then read by load_cor which replaces t_play.cor by the actual
**		.cor data address (inside t_vm.arena)
*/

static void		load_cor(t_vm *env, t_play *p, char *buffer)
{
	int			fd;
	t_garbage	*gb;
	char		*fname;
	uint		size;

	ft_printf("{grn}Load .cor file{eoc} \"%s\"\n", p->cor);
	fname = p->cor;
	gb = &env->gb;
	fd = open(fname, O_RDONLY);
	if (fd == -1)
		exit_vm(env, gb_add(gb, ft_cprintf("%s '%s'", gb_add(gb, strerror(errno)), fname)));
	if ((size = lseek(fd, 0, SEEK_END)) <= sizeof(t_header))
		exit_vm(env, gb_add(gb, ft_cprintf("Too small/empty file %s", fname)));
	size -= sizeof(t_header);
	if (size > CHAMP_MAX_SIZE)
		exit_vm(env, gb_add(gb, ft_cprintf("Champion %s too big", fname)));
	lseek(fd, 0, SEEK_SET);
	if (read(fd, &p->head, sizeof(t_header)) != sizeof(t_header)
		|| read(fd, buffer, size) != size)
		exit_vm(env, gb_add(gb, ft_cprintf("%s '%s'", gb_add(gb, strerror(errno)), fname)));
	p->head.magic = swap32_endianess(p->head.magic);
	p->head.prog_size = swap32_endianess(p->head.prog_size);
	if (p->head.magic != COREWAR_EXEC_MAGIC)
		exit_vm(env, gb_add(gb, ft_cprintf("Exec magic not recognised")));
	if (p->head.prog_size != size)
		exit_vm(env, gb_add(gb, ft_cprintf("Prog size does not match")));
}

/*
**	This function also sets the PCm
*/

void			load_cor_files(t_vm *env)
{
	int			i;
	int			spacing;
	int			offset;

	offset = 0;
	spacing = MEM_SIZE / env->players.len;
	i = -1;
	while (++i < env->players.len)
	{
		load_cor(env, &env->players.d[i], &env->arena[offset]);
		env->players.d[i].cor = &env->arena[offset];
		env->players.d[i].procs.d->pc = offset;
		env->players.d[i].procs.d->reg[0] = env->players.d[i].id;
		offset += spacing;
	}
}

static int		read_champ(t_vm *env, char **input, int i)
{
	t_play		*champ;

	ft_printf("{grn}read_champ{eoc}\n"); // REMOVE
	champ = &env->players.d[env->players.len++];
	champ->id = RESERVED_ID;
	if (!ft_strcmp(input[i], "-n") && ++i)
	{
		if (input[i] == NULL
			|| ft_strlen(input[i]) > 10
			|| ((champ->id = ft_atoi(input[i])) == RESERVED_ID))
			exit_vm(env, gb_add(&env->gb,
					ft_cprintf("Champ. id badly formatted ('%s')", input[i])));
		ft_printf("{grn}Read champ id set by user: (string)'%s' == (int)%d{eoc}\n", input[i], champ->id); // REMOVE
		++i;
	}
	if (input[i] == NULL || ft_strlen(input[i]) > CHAMP_MAX_SIZE)
		exit_vm(env, gb_add(&env->gb,
			ft_cprintf("Champion name ('%s') over the limit of %d chars",
				input[i], CHAMP_MAX_SIZE)));
	vecproc_push_empty(&env->gb, &champ->procs);
	champ->procs.d->reg[0] = 0;
	champ->cor = input[i];
	return (i);
}

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
	}
}

void		read_argv_init(t_vm *env, int argc, char **argv)
{
	int		i;

	env->cycle_die = CYCLE_TO_DIE;
	i = 0;
	while (++i < argc)
	{
		ft_printf("--reading argument #%d--\n", i); // REMOVE
		if (!ft_strcmp(argv[i], "-dump"))
			read_dump_cycle(env, argv[++i]);
		else
			i = read_champ(env, argv, i);
	}
	set_remaining_play_id(env);
	load_cor_files(env); // also sets the PC & r0
}
