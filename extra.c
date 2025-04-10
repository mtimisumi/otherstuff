/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extra.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmisumi <mmisumi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 17:06:26 by mmisumi           #+#    #+#             */
/*   Updated: 2025/04/10 20:14:21 by mmisumi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

// void perror_exit(const char *msg, int exit_code)
// {
// 	perror(msg);
// 	exit(exit_code);
// }

char	*get_env(char **envp)
{
	char	*env = NULL;
	int		i;

	i = 0;
	while(envp[i])
	{
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
		{
			env = envp[i] + 5;
			if (! env)
				perror("There is no env\n");
			break ;
		}
		i++;
	}
	return (env);
}

char	**get_path(char *path, char *cmd)
{
	char	**paths;
	char	*temp;
	int		i;

	i = 0;
	paths = ft_split(path, ':');
	while (paths[i])
	{
		temp = ft_strjoin(paths[i], "/");
		free(paths[i]);
		paths[i] = ft_strjoin(temp, cmd);
		free(temp);
		i++;
	}
	return (paths);
}

char	*get_cmd(char *cmd, char **envp)
{
	char	*path;
	char	**paths;
	int		i;
	char	*full_cmd = NULL;

	if (cmd[0] == '/' || cmd[0] == '.')
		return (cmd);
	i = 0;
	path = get_env(envp);
	paths = get_path(path, cmd);
	if (!paths)
		return (perror("Error splitting PATH\n"), NULL);
	while (paths[i])
	{
		full_cmd = paths[i];
		if (access(full_cmd, F_OK) == 0)
		{
			free_array(paths, i);
			return (full_cmd);
		}
		free(full_cmd);
		i++;
	}
	free_array(paths, i);
	return (NULL);
}

// char	*get_cmd(char *cmd, char **envp)
// {
// 	char	*path;
// 	char	**paths;
// 	// char	*temp;
// 	char	*full_cmd;
// 	int		i;
	
// 	i = 0;
// 	path = get_env(envp);
// 	paths = ft_split(path, ':');
// 	if (!paths)
// 		return (perror("Error splitting PATH\n"), NULL);
// 	while (paths[i])
// 	{
// 		full_cmd = ft_strjoin(paths[i], "/", cmd);
// 		if (!full_cmd)
// 		{
// 			free_array(paths, i);
// 			return (perror("Error joining path\n"), NULL);
// 		}
// 		if (access(full_cmd, F_OK) == 0)
// 		{
// 			free_array(paths, i);
// 			return (full_cmd);
// 		}
// 		else
// 			i++;
// 		free(full_cmd);
// 		i++;
// 	}
// 	free_array(paths, i);
// 	return (NULL);
// }
