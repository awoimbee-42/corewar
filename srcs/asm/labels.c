/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   labels.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpoirier <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/14 19:12:52 by cpoirier          #+#    #+#             */
/*   Updated: 2019/05/14 20:08:31 by cpoirier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asm.h"

int		read_label(t_label *label, char *s)
{
	size_t	i;

	i = 0;
	while (s[i] && s[i] != LABEL_CHAR && ft_strchr(LABEL_CHARS, s[i]))
		i++;
	if (s[i] != LABEL_CHAR)
		return (0);
	free(label->name);
	if (!(label->name = (char *)ft_memalloc(i + 1)))
		fail_msg(0, "Realloc failed");
	ft_strncpy(label->name, s, i);
	return (1);
}

void	init_labels(t_asm *my_asm)
{
	if (!(my_asm->labels = ft_memalloc((LABEL_COUNT + 1) * sizeof(t_label)))
			|| !(my_asm->labels[LABEL_COUNT].name = ft_strnew(0))
			|| !(my_asm->labels_holder = ft_memalloc((LABEL_COUNT + 1)
					* sizeof(t_label)))
			|| !(my_asm->labels_holder[LABEL_COUNT].name = ft_strnew(0)))
		fail_msg(0, "Malloc failed");
}

void	add_label(t_label **l, size_t *pos, char *name, size_t p)
{
	if ((*l)[*pos].name)
	{
		if (!(*l = (t_label *)realloc(*l, sizeof(t_label) * (*pos * 2 + 1))))
			fail_msg(0, "Realloc failed");
		(*l)[*pos * 2].name = ft_strnew(0);
	}
	(*l)[*pos].name = name;
	(*l)[*pos].pos = p;
	(*l)[*pos].offset = 0;
	(*pos)++;
}

void	create_label_holder(t_asm *my_asm, char *s, size_t *i, char *name)
{
	while (ft_strchr(LABEL_CHARS, s[*i + 1]) && s[*i + 1])
		(*i)++;
	if (!(name = (char *)malloc(*i + 1)))
		fail_msg(0, "Malloc failed");
	ft_strncpy(name, s + 1, *i);
	name[*i] = '\0';
	add_label(&my_asm->labels_holder, &my_asm->label_holder_pos,
			name, my_asm->op_begin);
	my_asm->labels_holder[my_asm->label_holder_pos - 1].offset = my_asm->cursor
		- my_asm->op_begin;
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
				my_asm->cursor = old_pos;
				break ;
			}
		}
		(j == my_asm->label_pos) ? fail_msg(my_asm, "Label not found") : 0;
	}
}
