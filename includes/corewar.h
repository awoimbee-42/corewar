/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   corewar.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/02 18:47:28 by cpoirier          #+#    #+#             */
/*   Updated: 2019/05/22 18:20:09 by cpoirier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COREWAR_H
# define COREWAR_H

#include "op.h"
#include "types.h"
# include <stdlib.h>
# include <unistd.h>
# include <sys/types.h> // yoloooooooo

# define LABEL_COUNT 50
# define LABEL_END "-"

# define OUTPUT_LENGTH 1048576

/*
**	utils.c
*/
void	write_nb(char *s, int nb, int byte_nb);
void	skip_whitespace(char *s, size_t *i);


#endif
