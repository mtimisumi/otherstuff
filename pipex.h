/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmisumi <mmisumi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/12 14:26:10 by mmisumi           #+#    #+#             */
/*   Updated: 2025/04/10 20:06:53 by mmisumi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include <unistd.h>
# include <stdio.h>
# include <stddef.h>
# include <fcntl.h>
# include <stdlib.h>
# include <sys/wait.h>

void perror_free_exit(const char *msg, char **cmd_flags, int exit_code);
// void	perror_exit(const char *msg, int exit_code);
char	*get_cmd(char *cmd, char **envp);
char	**get_path(char *path, char *cmd);
char	*get_env(char **envp);

size_t	ft_strlen(const char *s);
int 	ft_strlcpy(char *dst, const char *src, size_t size);
char	*ft_strdup(const char *s);
char	*ft_substr(char const *s, unsigned int start, size_t len);
char	**free_array(char **arr, int x);
char	**litsplit(char **arr, char const *s, char c);
char	**ft_split(char const *s, char c);
char	*ft_strjoin(char const *s1, char const *s2);
// char	*ft_strjoin(char const *s1, char const *s2, char const *s3);
int		ft_strncmp(const char *s1, const char *s2, size_t n);

#endif