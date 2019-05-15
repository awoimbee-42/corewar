/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_argv.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/07 19:24:05 by awoimbee          #+#    #+#             */
/*   Updated: 2019/05/15 19:48:03 by awoimbee         ###   ########.fr       */
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

static void		load_cor(t_vm *env, t_play *p, uint8_t *buffer)
{
	int			fd;
	t_garbage	*gb;
	char		*fname;
	uint		size;

	// ft_printf("{grn}Load .cor file{eoc} \"%s\"\n", p->cor);
	fname = (char*)p->cor;
	gb = &env->gb;
	fd = open(fname, O_RDONLY);
	if (fd == -1)
		exit_vm(env, gb_add(gb, ft_cprintf("%s '%s'", strerror(errno), fname)));
	if ((size = lseek(fd, 0, SEEK_END)) <= sizeof(t_header))
		exit_vm(env, gb_add(gb, ft_cprintf("Too small/empty file %s", fname)));
	size -= sizeof(t_header);
	if (size > CHAMP_MAX_SIZE)
		exit_vm(env, gb_add(gb, ft_cprintf("Champion %s too big", fname)));
	lseek(fd, 0, SEEK_SET);
	if (read(fd, &p->head, sizeof(t_header)) != sizeof(t_header)
		|| read(fd, buffer, size) != size)
		exit_vm(env, gb_add(gb, ft_cprintf("%s '%s'", strerror(errno), fname)));
	p->head.magic = swap32_endian(p->head.magic);
	p->head.prog_size = swap32_endian(p->head.prog_size);
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

	// ft_printf("{grn}read_champ{eoc}\n"); // REMOVE
	if (env->players.len == 4)
		exit_vm(env, gb_add(&env->gb, ft_cprintf("Too many champions !")));
	champ = &env->players.d[env->players.len++];
	// champ->index = env->players.len - 1;
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
	champ->cor = (uint8_t*)input[i];
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

void		init_ncurses(t_vm *vm)
{
	int		dim[2];

	vm->verbosity = -1;
	vm->visu.op_per_sec = 10;
	initscr();
	noecho();
	curs_set(FALSE);
	start_color();

	vm->visu.rootw = newwin(68, 270, 0, 0);
	vm->visu.arenaw = subwin(vm->visu.rootw, 64, 193, 2, 2);
	vm->visu.sidep.rootw = subwin(vm->visu.rootw, 68, 74, 0, 196);
	vm->visu.sidep.statusw = subwin(vm->visu.sidep.rootw, 4, 68, 2, 199);

box(vm->visu.sidep.statusw, '%', '%');

	getmaxyx(stdscr, dim[1], dim[0]);
	if (dim[0] < 270 || dim[1] < 68)
		exit_vm(vm, gb_add(&vm->gb,
				ft_cprintf("Windows too small (%dx%d), minimum is (270x68)",
					dim[0], dim[1])));

	ft_memset(vm->mem_owner, PLAY_COLOR_START, MEM_SIZE);
	init_pair(32, COLOR_WHITE, 0b00001000);  // contour
	init_pair(PLAY_COLOR_START, COLOR_WHITE, COLOR_BLACK);
	init_pair(PLAY0_COLOR + 0, COLOR_GREEN, COLOR_BLACK);  // p1
	init_pair(PLAY0_COLOR + 1, COLOR_BLUE, COLOR_BLACK);   // p2
	init_pair(PLAY0_COLOR + 2, COLOR_RED, COLOR_BLACK);    // p3
	init_pair(PLAY0_COLOR + 3, COLOR_YELLOW, COLOR_BLACK); // p4

	init_pair(CURS0_COLOR + 0, COLOR_BLACK, COLOR_WHITE);
	init_pair(CURS0_COLOR + 1, COLOR_BLACK, COLOR_GREEN);
	init_pair(CURS0_COLOR + 2, COLOR_BLACK, COLOR_BLUE);
	init_pair(CURS0_COLOR + 3, COLOR_BLACK, COLOR_RED);
	init_pair(CURS0_COLOR + 4, COLOR_BLACK, COLOR_YELLOW);


	// init_pair(DELT_CURS_COLOR + 5, 232, 255); //useless


	wattron(vm->visu.rootw, COLOR_PAIR(32));
	box(vm->visu.rootw, '*', '*');
	wattroff(vm->visu.rootw, COLOR_PAIR(32));
	wbkgd(vm->visu.arenaw, COLOR_PAIR(0));

	wattron(vm->visu.sidep.rootw, COLOR_PAIR(32));
	box(vm->visu.sidep.rootw, '*', '*');
	wattroff(vm->visu.sidep.rootw, COLOR_PAIR(32));

	wrefresh(vm->visu.rootw);
	wrefresh(vm->visu.sidep.rootw);
	wrefresh(vm->visu.sidep.statusw);
	visu_init_memview(vm);

	// pthread_create(&vm->visu.keys_handler, NULL, visu_khandler, (void*)vm);
}

void		read_argv_init(t_vm *env, int argc, char **argv)
{
	int		i;

	env->cycle_die = CYCLE_TO_DIE;
	i = 0;
	while (++i < argc)
	{
		// ft_printf("--reading argument #%d--\n", i); // REMOVE
		if (!ft_strcmp(argv[i], "-dump"))
			read_dump_cycle(env, argv[++i]);
		else if (!ft_strncmp(argv[i], "-visu", 5))
			env->verbosity = -1;
		else
			i = read_champ(env, argv, i);
	}
	set_remaining_play_id(env);
	load_cor_files(env); // also sets the PC & r0
	if (env->verbosity == -1)
		init_ncurses(env);
}
