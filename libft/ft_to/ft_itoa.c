/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asgalean <asgalean@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 11:34:21 by cvon-hee          #+#    #+#             */
/*   Updated: 2025/09/17 19:11:46 by asgalean         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_intlen(long n)
{
	unsigned long	n_len;

	n_len = 0;
	if (n == 0)
		n_len++;
	if (n < 0)
	{
		n = n * -1;
		n_len++;
	}
	while (n > 0)
	{
		n = n / 10;
		n_len++;
	}
	return (n_len);
}

static char	*ft_write_number(char *ptr, unsigned long n_len, long n)
{
	if (n == 0)
		*ptr = '0';
	if (n < 0)
	{
		*ptr = '-';
		n = n * -1;
	}
	while (n > 0)
	{
		*(ptr + --n_len) = (n % 10) + '0';
		n = n / 10;
	}
	return (ptr);
}

char	*ft_itoa(int n)
{
	char	*ptr;
	int		n_len;

	n_len = ft_intlen(n);
	ptr = (char *)malloc(sizeof(char) * (n_len + 1));
	if (!ptr)
		return (NULL);
	ptr = ft_write_number(ptr, n_len, n);
	ptr[n_len] = '\0';
	return (ptr);
}
