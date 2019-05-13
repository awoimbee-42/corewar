/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   asm.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/02 18:55:21 by cpoirier          #+#    #+#             */
/*   Updated: 2019/05/13 19:29:14 by awoimbee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asm.h"

const t_op	g_op_tab[17] =
{
	{"live", 1, {T_DIR},                                                1,   10, "alive",                                0, 0},
	{"ld",   2, {T_DIR | T_IND, T_REG},                                 2,    5, "load",                                 1, 0},
	{"st",   2, {T_REG, T_IND | T_REG},                                 3,    5, "store",                                1, 0},
	{"add",  3, {T_REG, T_REG, T_REG},                                  4,   10, "addition",                             1, 0},
	{"sub",  3, {T_REG, T_REG, T_REG},                                  5,   10, "soustraction",                         1, 0},
	{"and",  3, {T_REG | T_DIR | T_IND, T_REG | T_IND | T_DIR, T_REG},  6,    6, "et (and  r1, r2, r3   r1&r2 -> r3",    1, 0},
	{"or",   3, {T_REG | T_IND | T_DIR, T_REG | T_IND | T_DIR, T_REG},  7,    6, "ou  (or   r1, r2, r3   r1 | r2 -> r3", 1, 0},
	{"xor",  3, {T_REG | T_IND | T_DIR, T_REG | T_IND | T_DIR, T_REG},  8,    6, "ou (xor  r1, r2, r3   r1^r2 -> r3",    1, 0},
	{"zjmp", 1, {T_DIR},                                                9,   20, "jump if zero",                         0, 1},
	{"ldi",  3, {T_REG | T_DIR | T_IND, T_DIR | T_REG, T_REG},         10,   25, "load index",                           1, 1},
	{"sti",  3, {T_REG, T_REG | T_DIR | T_IND, T_DIR | T_REG},         11,   25, "store index",                          1, 1},
	{"fork", 1, {T_DIR},                                               12,  800, "fork",                                 0, 1},
	{"lld",  2, {T_DIR | T_IND, T_REG},                                13,   10, "long load",                            1, 0},
	{"lldi", 3, {T_REG | T_DIR | T_IND, T_DIR | T_REG, T_REG},         14,   50, "long load index",                      1, 1},
	{"lfork",1, {T_DIR},                                               15, 1000, "long fork",                            0, 1},
	{"aff",  1, {T_REG},                                               16,    2, "aff",                                  1, 0},
	{0, 0, {0}, 0, 0, 0, 0, 0}
};

int		get_name(char *s, char name[], size_t len)
{
	size_t	i;
	size_t	j;

	i = 0;
	skip_whitespace(s, &i);
	if (s[i] != '"')
		return (0);
	i++;
	j = 0;
	while (s[i + j] && s[i + j] != '"' && j < len)
	{
		name[j] = s[i + j];
		j++;
	}
	if (!s[i + j] || !j || (j >= len && s[i + j] != '"'))
		return (0);
	name[j] = '\0';
	return (1);
}

int		get_ocp(int op, t_arg_type args[3]) // ?
{
	char res;

	res = 0;
	return (res);
}

int		get_op_id(char *s)
{
	size_t	i;

	i = 0;
	while (i < 16)
	{
		if (!ft_strncmp(s, g_op_tab[i].name, ft_strlen(g_op_tab[i].name))
				&& (s[ft_strlen(g_op_tab[i].name)] == ' '
					|| s[ft_strlen(g_op_tab[i].name)] == '\t'))
			return (i + 1);
		i++;
	}
	return (0);
}


int		read_label(t_label *label, char *s)
{
	size_t	i;

	printf("Reading LABEL from %s\n", s);
	i = 0;
	while (s[i] && s[i] != LABEL_CHAR && ft_strchr(LABEL_CHARS, s[i]))
		i++;//add_to_name(&label->name, &i, s[i]);
	if (s[i] != LABEL_CHAR)
		return (0);
	free(label->name);
	if (!(label->name = (char *)ft_memalloc(i + 1)))
		fail_msg("Realloc failed");
	ft_strncpy(label->name, s, i);
	return (1);
}

void	init_labels(t_asm *my_asm)
{
	if (!(my_asm->labels = ft_memalloc((LABEL_COUNT + 1) * sizeof(t_label)))
		|| !(my_asm->labels[LABEL_COUNT].name = ft_strnew(0))
		|| !(my_asm->labels_holder = ft_memalloc((LABEL_COUNT + 1) * sizeof(t_label)))
		|| !(my_asm->labels_holder[LABEL_COUNT].name = ft_strnew(0)))
		fail_msg("Malloc failed");
}

void	add_label(t_label **l, size_t *pos, char *name, size_t p, size_t offset)
{
	if ((*l)[*pos].name)
	{
		if (!(*l = (t_label *)realloc(*l, sizeof(t_label) * (*pos * 2 + 1))))
			fail_msg("Realloc failed");
		(*l)[*pos * 2].name = ft_strnew(0);
	}
	(*l)[*pos].name = name;
	(*l)[*pos].pos = p;
	(*l)[*pos].offset = offset;
	(*pos)++;
}

void		write_to_output(char **output, size_t *pos, char *src)
{
	size_t	start;

	start = *pos;
	while (src[*pos - start])
	{
		if (*pos % OUTPUT_LENGTH == 0)
		{
			if (!(*output = (char *)realloc(*output, *pos + OUTPUT_LENGTH)))
				fail_msg("Realloc failed");
			ft_bzero(*output + *pos, OUTPUT_LENGTH - 1);
		}
		(*output)[*pos] = src[*pos - start];
		(*pos)++;
	}
}

void		write_n_to_output(char **output, size_t *pos, char *src, size_t n)
{
	size_t	start;

	start = *pos;
	while (*pos - start < n)
	{
		if (*pos % OUTPUT_LENGTH == 0)
		{
			if (!(*output = (char *)realloc(*output, *pos + OUTPUT_LENGTH)))
				fail_msg("Realloc failed");
			ft_bzero(*output + *pos, OUTPUT_LENGTH);
		}
		(*output)[*pos] = src[*pos - start];
		(*pos)++;
	}
}

t_arg_type	get_arg_type(char *s)
{
	size_t	i;

	i = 0;
	if (*s == 'r' || (*s == DIRECT_CHAR && s[1] != LABEL_CHAR))
	{
		if (*s != 'r' && s[1] == '-')
			i++;
		while (ft_isdigit(s[++i]))
			;
		skip_whitespace(s, &i);
		if (!s[i] || s[i] == SEPARATOR_CHAR)
			return (*s == 'r' ? T_REG : T_DIR);
		return (-1);
	}
	else if (*s == DIRECT_CHAR && s[1] == LABEL_CHAR)
	{
		++i;
		while (ft_strchr(LABEL_CHARS, s[++i]) && s[i])
			;
		skip_whitespace(s, &i);
		if (!s[i] || s[i] == SEPARATOR_CHAR)
			return (T_DIR | T_LAB);
		return (-1);
	}
	else if (*s == LABEL_CHAR)
	{
		while (ft_strchr(LABEL_CHARS, s[++i]) && s[i])
			;
		skip_whitespace(s, &i);
		if (!s[i] || s[i] == SEPARATOR_CHAR)
			return (T_IND | T_LAB);
		return (-1);
	}
	else if (ft_isdigit(*s) || *s == '-')
	{
		if (*s == '-')
			i++;
		while (ft_isdigit(s[i]))
			i++;
		skip_whitespace(s, &i);
		if (!s[i] || s[i] == SEPARATOR_CHAR)
			return (T_IND);
		return (-1);
	}
	return (-1);
}

void	write_nb_to_output(t_asm *my_asm, int nb, int pre)
{
	char *s = ft_strnew(pre);
	if (pre > 1)
		write_nb(s, nb, pre);
	else
		s[0] = nb;
	write_n_to_output(&my_asm->output, &my_asm->cursor, s, pre);
	free(s);
}

void	write_param(t_asm *my_asm, t_arg_type type, char *s)
{
	size_t	i;
	char 	*name;
	int		two;

	i = 0;
	two = g_op_tab[my_asm->current_op - 1].dir2 ? 2 : 4;
	if (type & T_DIR || type & T_REG)
		++s;
	if (type & T_LAB)
	{
		while (ft_strchr(LABEL_CHARS, s[i + 1]) && s[i + 1])
			i++;
		if (!(name = (char *)malloc(i + 1)))
			fail_msg("Malloc failed");
		ft_strncpy(name, s + 1, i);
		name[i] = '\0';
		printf("Label holder creation: %s\n", name);
		add_label(&my_asm->labels_holder, &my_asm->label_holder_pos,
				name, my_asm->op_begin, my_asm->cursor - my_asm->op_begin);
		write_nb_to_output(my_asm, two, 1);
		my_asm->cursor += two == 2 ? 1 : 3;
		//my_asm->cursor += g_op_tab[my_asm->current_op - 1].carry ? 2 : 4;
	}
	else if (!(type & T_REG))
	{
		write_nb_to_output(my_asm, ft_atoi(s + i), g_op_tab[my_asm->current_op - 1].dir2 ? 2 : 4);
	}
	else
		write_nb_to_output(my_asm, ft_atoi(s + i), 1);
}

void	write_label_holders(t_asm *my_asm)
{
	size_t	i;
	size_t	j;
	size_t	old_pos;

	i = -1;
	while (++i < my_asm->label_holder_pos)
	{
		j = -1;
		while (++j < my_asm->label_pos)
		{
			if (!ft_strcmp(my_asm->labels_holder[i].name,
						my_asm->labels[j].name))
			{



				old_pos = my_asm->cursor;
				my_asm->cursor = my_asm->labels_holder[i].pos
					+ my_asm->labels_holder[i].offset;
				write_nb_to_output(my_asm, (int)my_asm->labels[j].pos
						- (int)my_asm->labels_holder[i].pos,
						my_asm->output[my_asm->cursor]);
				printf("Diff is: %d\n", (int)my_asm->labels[j].pos - (int)my_asm->labels_holder[i].pos);

				printf("Size should be : %d\n", (int)my_asm->output[my_asm->cursor]);

				my_asm->cursor = old_pos;
				break ;
			}
		}
		if (j == my_asm->label_pos)
			fail_msg("Label not found");
	}
}

void	write_output(t_asm *my_asm)
{
	int		fd;

	fd = open("out.cor", O_CREAT | O_WRONLY, 0777);
	printf("Current cursor: %lu\n", my_asm->cursor);
	if (fd >= 0)
		write(fd, my_asm->output, my_asm->cursor);
	close(fd);
}

void	write_opcode(t_asm *my_asm, t_arg_type types[3])
{
	size_t	i;

	printf("Types: %d %d %d\n", types[0], types[1], types[2]);

	printf("At first, num is : %d\n", my_asm->output[my_asm->op_begin + 1]);

	i = -1;
	while (++i < 3)
	{
		if (types[i] & T_REG)
			my_asm->output[my_asm->op_begin + 1] += 1;
		else if (types[i] & T_IND)
			my_asm->output[my_asm->op_begin + 1] += 3;
		else if (types[i] & T_DIR)
			my_asm->output[my_asm->op_begin + 1] += 2;
		printf("So far, num is: %d\n", my_asm->output[my_asm->op_begin + 1]);
		my_asm->output[my_asm->op_begin + 1] <<= 2;
	}
}

void	handle_op(t_asm *my_asm, char *s)
{
	size_t		i;
	size_t		current_param;
	t_arg_type	current_type;
	t_arg_type	types[3];

	my_asm->op_begin = my_asm->cursor;

	printf("Before writing to op %d at pos %lu\n", my_asm->current_op, my_asm->cursor);

	ft_bzero(types, sizeof(t_arg_type) * 3);
	//for (size_t y = 0; y < my_asm->cursor; y++)
	//	printf("%d ", my_asm->output[y]);
	//printf("\n\n");
	write_nb_to_output(my_asm, my_asm->current_op, 1);
	current_param = 0;
	i = 0;
	while (s[i] != ' ' && s[i] != '\t')
		i++;
	skip_whitespace(s, &i);
	if (my_asm->current_op != 1 && my_asm->current_op != 12 && my_asm->current_op != 15 && my_asm->current_op != 9)
		write_nb_to_output(my_asm, 0, 1);//my_asm->cursor += 2;
	while (current_param < (size_t)g_op_tab[my_asm->current_op - 1].nb_args)
	{
		if ((types[current_param] = get_arg_type(s + i)) < 0)
		{
			fail_msg("Syntax error for parameter");
		}
		if (!(g_op_tab[my_asm->current_op - 1].args_types[current_param] & types[current_param]))
			fail_msg("Invalid type");
		write_param(my_asm, types[current_param], s + i);
		while (s[i] && s[i] != SEPARATOR_CHAR)
			i++;
		if (s[i] == SEPARATOR_CHAR && current_param + 1 == (size_t)g_op_tab[my_asm->current_op - 1].nb_args)
			fail_msg("Too much parameters");
		else if (!s[i] && current_param + 1 < (size_t)g_op_tab[my_asm->current_op - 1].nb_args)
			fail_msg("Missing parameters");
		if (s[i])
			++i;
		skip_whitespace(s, &i);
		current_param++;
	}
	if (my_asm->current_op != 1 && my_asm->current_op != 12 && my_asm->current_op != 15 && my_asm->current_op != 9)
		write_opcode(my_asm, types);
}

void	write_header(t_asm *my_asm)
{
	unsigned int	n;
	int				k;
	size_t			pos;

	my_asm->header.magic = COREWAR_EXEC_MAGIC;
	ft_memcpy(my_asm->output, &my_asm->header, sizeof(t_header));


	n = COREWAR_EXEC_MAGIC;
	k = -1;
	while (++k < 4)
	{
		my_asm->output[3 - k] = n % 256;
		n >>= 8;
	}
	n = my_asm->header.prog_size;
	k = -1;
	pos = ((4 + PROG_NAME_LENGTH) / 8 + 1) * 8;
	while (++k < 4)
	{
		my_asm->output[pos + 3 - k] = n % 256;
		n >>= 8;
	}

	//char *magic;// = ft_strnew(8);

	//write_nb_to_output(my_asm, COREWAR_EXEC_MAGIC, 4);


	/*write_nb(magic, COREWAR_EXEC_MAGIC, 8);
	  write_to_output(&my_asm->output, &my_asm->cursor, magic);
	  free(magic);*/

	/*while (++i < PROG_NAME_LENGTH)
	  my_asm->output[my_asm->cursor + i] = my_asm->name[i];
	  my_asm->cursor += PROG_NAME_LENGTH;
	  i = -1;
	  while (++i < COMMENT_LENGTH)
	  my_asm->output[my_asm->cursor + i] = my_asm->comment[i];
	  my_asm->cursor += COMMENT_LENGTH;
	  */


	//write_n_to_output(&my_asm->output, &my_asm->cursor, my_asm->header.prog_name, PROG_NAME_LENGTH);
	//char *s = ft_strnew(4);
	//s[3] = '$';
	//write_n_to_output(&my_asm->output, &my_asm->cursor, s, 4);
	//free(s);
	//write_n_to_output(&my_asm->output, &my_asm->cursor, my_asm->comment, COMMENT_LENGTH);

	//magic = ft_strnew(PROG_NAME_LENGTH);
	//ft_strcpy(magic, my_asm->name);
	//write_to_output(&my_asm->output, &my_asm->cursor, magic);
	//free(magic);
	//magic = ft_strnew(COMMENT_LENGTH);
	//ft_strcpy(magic, my_asm->comment);
	//write_to_output(&my_asm->output, &my_asm->cursor, magic);
	//free(magic);
	printf("Writing header at: %lu\n", my_asm->cursor);
}

void	write_final_length(t_asm *my_asm)
{
	//int		len;
	//size_t	old_pos;

	//old_pos = my_asm->cursor;
	//len = (int)my_asm->cursor;
	//my_asm->cursor = 4 + PROG_NAME_LENGTH;
	my_asm->header.prog_size = my_asm->cursor - sizeof(t_header);
	//write_nb_to_output(my_asm, len, 4);
	//my_asm->cursor = old_pos;
}

int		get_asm(char *path, t_asm *my_asm)
{
	int		fd;
	char	*s;
	size_t	i;

	if ((fd = open(path, O_RDONLY)) == -1)
		fail_msg(ft_cprintf("%s '%s'", strerror(errno), path));
	init_labels(my_asm);
	char *dummy = ft_strnew(sizeof(t_header));
	write_n_to_output(&my_asm->output, &my_asm->cursor, dummy, sizeof(t_header));
	free(dummy);
	while (get_next_line(fd, &s) > 0)
	{
		i = 0;
		skip_whitespace(s, &i);
		if (!ft_strncmp(s + i, NAME_CMD_STRING, ft_strlen(NAME_CMD_STRING)))
		{
			if (my_asm->header.prog_name[0])
				fail_msg("Error: Name already declared");
			if (!get_name(s + i + ft_strlen(NAME_CMD_STRING),
						my_asm->header.prog_name, PROG_NAME_LENGTH))
				fail_msg("Syntax error: Name not well-formated");
		}
		else if (!ft_strncmp(s + i, COMMENT_CMD_STRING,
					ft_strlen(COMMENT_CMD_STRING)))
		{
			if (my_asm->header.comment[0])
				fail_msg("Error: Comment already declared");
			if (!get_name(s + i + ft_strlen(COMMENT_CMD_STRING),
						my_asm->header.comment, COMMENT_LENGTH))
				fail_msg("Syntax error: Comment not well-formated");
		}
		else if ((my_asm->current_op = get_op_id(s + i)))
		{
			//if (!my_asm->cursor)
			//	write_header(my_asm);
			handle_op(my_asm, s + i);
		}
		else if (s[i] && s[i] != COMMENT_CHAR)
		{
			if (!my_asm->header.prog_name[0] || !my_asm->header.comment[0])
				fail_msg("Error: Name and Comment must be declared before any instruction or label");
			//if (!my_asm->cursor)
			//	write_header(my_asm);
			add_label(&my_asm->labels, &my_asm->label_pos, ft_strnew(0), my_asm->cursor, 0);
			if (!read_label(my_asm->labels + my_asm->label_pos - 1, s + i))
				fail_msg("Syntax error on label");
			else
			{
				i += ft_strlen(my_asm->labels[my_asm->label_pos - 1].name) + 1;
				skip_whitespace(s, &i);
				if ((my_asm->current_op = get_op_id(s + i)))
					handle_op(my_asm, s + i);
			}
			// Here, handle potential label definitions.
			// (precision: label names must be strictly followed by the LABEL_CHAR)
			// Else, it's a mistake.
		}
		free(s);
	}

	write_label_holders(my_asm);

	write_final_length(my_asm);

	write_header(my_asm);

	printf("My name is %s\n", my_asm->header.prog_name);
	printf("My comment is %s\n", my_asm->header.comment);


	write_output(my_asm);
	//printf("%s\n", my_asm->output);

	for (int i = 0; i < my_asm->label_holder_pos; i++)
		printf("Label holder:\nName: %s\nPos: %lu\n\n", my_asm->labels_holder[i].name, my_asm->labels_holder[i].pos);
	for (int i = 0; i < my_asm->label_pos; i++)
		printf("Label:\nName: %s\nPos: %lu\n\n", my_asm->labels[i].name, my_asm->labels[i].pos);

	free(my_asm->output);

	/*
	// Freeing everything to prevent nazi leaks
	for (int i = 0; i < my_asm->label_pos; i++)
	free(my_asm->labels[i].name);
	for (int i = 0; i < my_asm->label_holder_pos; i++)
	free(my_asm->labels_holder[i].name);
	free(my_asm->labels[(my_asm->label_pos / LABEL_COUNT + 1) * LABEL_COUNT].name);
	free(my_asm->labels_holder[(my_asm->label_holder_pos / LABEL_COUNT + 1) * LABEL_COUNT].name);
	free(my_asm->labels);
	free(my_asm->labels_holder);
	// End of freeing
	*/

	return (1);
}

int main(int ac, char **av)
{
	t_asm	my_asm;

	if (ac < 2)
	{
		ft_printf("Usage: %s <sourcefile.s>\n", av[0]);
		return (0);
	}
	ft_bzero(&my_asm, sizeof(my_asm));
	return (get_asm(av[1], &my_asm));
}
