/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   corewar.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/02 18:47:28 by cpoirier          #+#    #+#             */
/*   Updated: 2019/05/08 16:02:44 by awoimbee         ###   ########.fr       */
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

# define OUTPUT_LENGTH 1024

static t_op    op_tab[17] =
{
//				T_REG 01	1 byte
//				T_DIR 10	4 byte
//				T_IND 11	2 byte
//  name - nb_args -- args_types --                                 id -- nb_cycles -- description --      uses_coding_byte -- does_direct_values_on_2_bytes_instead_of_4
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



#endif
