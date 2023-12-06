/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_files.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emre <emre@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/05 14:20:50 by ealbayra          #+#    #+#             */
/*   Updated: 2023/12/06 19:53:05 by emre             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void	other_out_filesme(t_parse *parse)
{
	char	str[256];
	char	*pwd;
	char	*pwd1;
	t_parse	*nparse;

	getcwd(str, 256);
	nparse = parse->next;
	pwd = ft_strjoin(str, "/");
	pwd1 = ft_strjoin(pwd, nparse->text[0]);
	open(pwd1, O_CREAT | O_RDWR, 0777);
	free(pwd1);
	pwd1 = ft_strjoin(pwd, nparse->next->text[0]);
	if (nparse->type == 4)
		parse->fd = open(pwd1, O_CREAT | O_RDWR | O_APPEND, 0777);
	else if (parse->type == 3)
		parse->fd = open(pwd1, O_CREAT | O_RDWR | O_TRUNC, 0777);
	parse->outfile = parse->fd;
	if (pwd)
		free(pwd);
	if (pwd1)
		free(pwd1);
}

void	other_text_create_me(t_parse *m_parse)
{
	t_parse	*n_parse;
	int		i;
	int		j;

	n_parse = m_parse->next;
	i = 0;
	while (m_parse->text[i])
		i++;
	while (n_parse->next && n_parse->type != 2)
	{
		j = 1;
		while (n_parse->text[j])
		{
			m_parse->text[i] = ft_strdup(n_parse->text[j]);
			i++;
			j++;
		}
		n_parse = n_parse->next;
	}
	m_parse->text[i] = NULL;
	other_out_filesme(m_parse);
}

/**
 * Bu fonksiyon, çıkış dosyalarını oluşturur.
 *
 * @param parse   Geçerli ayrıştırma yapısı
 * @param prev_parse   Önceki ayrıştırma yapısı
 */
void	create_output_files(t_parse *parse, t_parse *prev_parse)
{
	char	str[256];
	char	*pwd;
	t_parse	*next_parse;
	char	*temp;

	getcwd(str, 256);
	next_parse = parse->next;
	if (next_parse->type == 3 || next_parse->type == 4)
		return (other_text_create_me(parse));
	pwd = ft_strjoin(str, "/");
	temp = ft_strjoin(pwd, next_parse->text[0]);
	free(pwd);
	pwd = temp;
	if (parse->type == 4)
		next_parse->fd = open(pwd, O_CREAT | O_RDWR | O_APPEND, 0777);
	else if (parse->type == 3)
		next_parse->fd = open(pwd, O_CREAT | O_RDWR | O_TRUNC, 0777);
	else if (parse->cmd)
		parse->outfile = next_parse->fd;
	else if (prev_parse->cmd)
		prev_parse->outfile = next_parse->fd;
	if (pwd)
		free(pwd);
}

/**
 * create_files_m fonksiyonu,
	verilen t_shell yapısı içindeki parse listesini dolaşarak
 * çıkış ve giriş dosyalarını oluşturur.
 *
 * @param m_shell t_shell yapısı
 * @return int işlem başarılıysa 1, aksi halde 0
 */
int	create_files(t_shell *m_shell)
{
	t_parse	*parse;
	int		i;

	i = 1;
	parse = m_shell->parse;
	while (parse)
	{
		if (parse->type == 3 || parse->type == 4)
			create_output_files(parse, m_shell->parse);
		else if (parse->type == 5)
			i = create_input_files(parse);
		parse = parse->next;
	}
	return (i);
}
