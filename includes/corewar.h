/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   corewar.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpoirier <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/02 18:47:28 by cpoirier          #+#    #+#             */
/*   Updated: 2019/05/02 21:24:20 by cpoirier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COREWAR_H
# define COREWAR_H


# include "../ressources/op.h"
# include <stdlib.h>

# define LABEL_COUNT 50
# define LABEL_END '-'

typedef	struct	s_label
{
	char		name[PROG_NAME_LENGTH + 1];
	size_t		pos;
}				t_label;

typedef	struct	s_asm
{
	char		name[PROG_NAME_LENGTH + 1];
	char		comment[COMMENT_LENGTH + 1];
	int			current_op;
	char		*output;
	size_t		cursor;
	t_label		*labels;
	size_t		label_pos;
	t_label		*labels_holder;
	size_t		label_holder_pos;
}				t_asm;



#endif
