/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_argv.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/07 19:24:05 by awoimbee          #+#    #+#             */
/*   Updated: 2019/05/07 22:57:57 by awoimbee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "corewar.h"
#include "vm.h"
#include <fcntl.h>
#include <errno.h>

// ./corewar [-dump nbr_cycles] [[-n number] champion1.cor] ...


void		load_cor(t_vm *env, char *fname, t_play *p)
{
	int			fd;
	t_garbage	*gb;

	gb = &env->gb;
	fd = open(fname, O_RDONLY);
	if (fd == -1)
		exit_vm(env, gb_add(gb, ft_cprintf("%s '%s'", gb_add(gb, strerror(errno)), fname)));
	p->cor_len = lseek(fd, 0, SEEK_END);
	if (p->cor_len <= 0)
		exit_vm(env, gb_add(gb, ft_cprintf("%s '%s'", gb_add(gb, strerror(errno)), fname)));
	if (p->cor_len > CHAMP_MAX_SIZE) 										// wtf, CHAMP_MAX_SIZE is very small ??
		exit_vm(env, gb_add(gb, ft_cprintf("Champion %s too big", fname)));
	lseek(fd, 0, SEEK_SET);
	p->cor = gb_malloc(&env->gb, p->cor_len);
	if (read(fd, p->cor, p->cor_len) != p->cor_len)
		exit_vm(env, gb_add(gb, ft_cprintf("%s '%s'", gb_add(gb, strerror(errno)), fname)));
}

int			read_champ(t_vm *env, char **input, int i)
{
	t_play		*champ;

	ft_printf("read_champ\n"); // REMOVE
	champ = vecplay_point_last(vecplay_push_empty(&env->gb, &env->players));
	if (!ft_strcmp(input[i], "-n") && ++i)
	{
		if (input[i] == NULL
			|| ft_strlen(input[i]) > 10
			|| ((champ->id = ft_atoi(input[i])) == RESERVED_ID))
			exit_vm(env, gb_add(&env->gb,
					ft_cprintf("Champ. id badly formatted ('%s')", input[i])));
		ft_printf("Read champ id set by user: (string)'%s' == (int)%d\n", input[i], champ->id); // REMOVE
		++i;
	}
	else
		champ->id = RESERVED_ID;
	if (input[i] == NULL || ft_strlen(input[i]) > CHAMP_MAX_SIZE)
		exit_vm(env, gb_add(&env->gb,
			ft_cprintf("Champion name ('%s') over the limit of %d chars",
				input[i], CHAMP_MAX_SIZE)));
	load_cor(env, input[i], champ);
	return (i);
}

void		read_dump_cycle(t_vm *env, char *input)
{
	if (input == NULL
		|| ft_strlen(input) > 10
		|| (env->cycle_dump = ft_atoi(input)) <= 0)
		exit_vm(env, gb_add(&env->gb,
			ft_cprintf("Dump cycle badly formatted ('%s')", input)));
}

void		set_remaining_play_id(t_vm *env)
{
	size_t		i;
	size_t		j;
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

void		read_argv(t_vm *env, int argc, char **argv)
{
	int		i;

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
}