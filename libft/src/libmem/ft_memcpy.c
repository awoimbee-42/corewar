/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/23 01:06:53 by awoimbee          #+#    #+#             */
/*   Updated: 2019/05/24 01:15:00 by awoimbee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdint.h>
#include <x86intrin.h>
#include <avxintrin.h>

/*
**	In scalar memcpy we try to use the native cpu register widths
**	before falling back to using char to finish it off
*/

static void	scalar_memcpy(void *restrict d, const void *restrict s, size_t n)
{
	while (n >= sizeof(uintmax_t))
	{
		*(uintmax_t*)d = *(uintmax_t*)s;
		d += sizeof(uintmax_t);
		s += sizeof(uintmax_t);
		n -= sizeof(uintmax_t);
	}
	while (n >= sizeof(unsigned int))
	{
		*(unsigned int*)d = *(unsigned int*)s;
		d += sizeof(unsigned int);
		s += sizeof(unsigned int);
		n -= sizeof(unsigned int);
	}
	while (n > 0)
	{
		*(unsigned char*)d++ = *((unsigned char *)s++);
		--n;
	}
}

/*
**	Alligned sse/avx memcpy could not be more optimized that this.
**		(we use stream so the cpu knows the data should not be kept in cache)
*/

static void	align_avx_memcpy(void *restrict d, const void *restrict s, size_t n)
{
	while (n >= sizeof(__m256i))
	{
		_mm256_stream_si256((__m256i*)d, _mm256_stream_load_si256((__m256i*)s));
		d += sizeof(__m256i);
		s += sizeof(__m256i);
		n -= sizeof(__m256i);
	}
	if (__builtin_expect(n, 0))
		scalar_memcpy(d, s, n);
}

static void	align_sse_memcpy(void *restrict d, const void *restrict s, size_t n)
{
	while (n >= sizeof(__m128i))
	{
		_mm_stream_si128((__m128i*)d, _mm_stream_load_si128((__m128i*)s));
		d += sizeof(__m128i);
		s += sizeof(__m128i);
		n -= sizeof(__m128i);
	}
	if (__builtin_expect(n, 0))
		scalar_memcpy(d, s, n);
}

/*
**	This is the fallback (that will be used in most cases),
**	where we have a big amount of data to copy but no particular allignment
*/

static void	sse_memcpy(void *restrict d, const void *restrict s, size_t n)
{
	while (n >= sizeof(__m128i))
	{
		_mm_storeu_si128((__m128i*)d, _mm_loadu_si128(s));
		s += sizeof(__m128i);
		d += sizeof(__m128i);
		n -= sizeof(__m128i);
	}
	scalar_memcpy(d, s, n);
}

/*
**	The modulo operations in here are optimized by the compiler & cpu
**	as only a few bits need to be checked
*/

void		*ft_memcpy(void *restrict dst, const void *restrict src, size_t n)
{
	if (LFT_AVX
		&& !((uintptr_t)dst % sizeof(__m256i))
		&& !((uintptr_t)src % sizeof(__m256i)))
		align_avx_memcpy(dst, src, n);
	else if (LFT_SSE2
		&& !((uintptr_t)dst % sizeof(__m128i))
		&& !((uintptr_t)src % sizeof(__m128i)))
		align_sse_memcpy(dst, src, n);
	else if (LFT_SSE2)
		sse_memcpy(dst, src, n);
	else
		scalar_memcpy(dst, src, n);
	return (dst);
}
