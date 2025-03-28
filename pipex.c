// /* ************************************************************************** */
// /*                                                                            */
// /*                                                        :::      ::::::::   */
// /*   pipex.c                                            :+:      :+:    :+:   */
// /*                                                    +:+ +:+         +:+     */
// /*   By: mmisumi <mmisumi@student.42.fr>            +#+  +:+       +#+        */
// /*                                                +#+#+#+#+#+   +#+           */
// /*   Created: 2025/03/19 13:48:32 by mmisumi           #+#    #+#             */
// /*   Updated: 2025/03/20 19:00:08 by mmisumi          ###   ########.fr       */
// /*                                                                            */
// /* ************************************************************************** */

// #include "pipex.h"

// #define CHILD 0
// #define READ 0
// #define WRITE 1

// char	*get_env(char **envp)
// {
// 	char	*env = NULL;
// 	int		i;

// 	i = 0;
// 	while (envp[i])
// 	{
// 		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
// 			env = envp[i] + 5;
// 		else
// 			i++;
// 	}
// 	// if (!env)//so if PATH= was never found
// 	// 	return (write(STDERR_FILENO, "no env found\n", 13), );
// 	if (!env)//so if PATH= was never found
// 	{
// 		write(STDOUT_FILENO, "no env found\n", 13);
// 		exit(127);
// 	}
// 	return (env);
// }

// char	*get_cmd(char *cmd, char **envp)
// {
// 	char	*env;
// 	char	**paths;
// 	char	*path;
// 	int		i;

// 	i = 0;
// 	env = get_env(envp);
// 	paths = ft_split(path, ':');
// 	while (paths[i])
// 	{
// 		path = ft_strjoin(paths[i], "/", cmd);
// 		if (access(path, R_OK) == 0)
// 			return (path);//i should free other paths
// 		else
// 		{
// 			free(path);
// 			i++;
// 		}
// 	}
// 	return (NULL);//maybe free also if cmd was never found
// }

// void	child_process_one(int *pip, char **argv, char **envp)
// {
// 	int		infile;
// 	char	*cmd;
// 	char 	**cmd_flags;

// 	infile = open(argv[1], O_RDONLY);
// 	if (infile == -1)
// 	{
// 		perror("no valid infile\n");
// 		exit(0);
// 	}
// 	dup2(infile, STDIN_FILENO);
// 	close(infile);
// 	pipe(pip);
// 	close(pip[READ]);
// 	dup2(pip[WRITE], STDOUT_FILENO);
// 	close(pip[WRITE]);
// 	cmd_flags = ft_split(argv[2], ' ');
// 	cmd = get_cmd(cmd_flags[0], envp);
// 	if (execve(cmd, cmd_flags, envp) == -1)
// 	{
// 		perror("cmd1 fail");
// 		exit(1);
// 	}
// }

// void	child_process_two(int *pip, char **argv, char **envp)
// {
// 	int		outfile;
// 	char	*cmd;
// 	char	**cmd_flags;

// 	outfile = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0644);
// 	if (outfile == -1)
// 	{
// 		perror("error with outfile\n");
// 		exit(0);
// 	}
// 	dup2(outfile, STDOUT_FILENO);
// 	close(outfile);
// 	dup2(pip[READ], STDIN_FILENO);
// 	close(pip[READ]);
// 	cmd_flags = ft_split(argv[3], ' ');
// 	cmd = get_cmd(cmd_flags[0], envp);
// 	if (execve(cmd, cmd_flags, envp) == -1)
// 	{
// 		perror("cmd2 fail");
// 		exit(127);
// 	}
// }


// int	main(int argc, char *argv[], char *envp[])
// {
// 	int		pip[2];
// 	pid_t	pid1;
// 	pid_t	pid2;
// 	int		status;
// 	// int		exitcode;

// 	if (argc != 5)
// 		return (write(STDERR_FILENO, "not good amount of arguments\n", 29), -1);
// 	pid1 = fork();
// 	if (pid1 == -1)
// 		return (write(STDERR_FILENO, "forking pid1 failed\n", 20), -1);
// 	if (pid1 == CHILD)
// 		child_process_one(&pip[2], argv, envp);
// 	pid2 = fork();
// 	if (pid2 == -1)
// 		return (write(STDERR_FILENO, "forking pid2 failed\n", 20), -1);
// 	if (pid2 == CHILD)
// 		child_process_two(&pip[2], argv, envp);
// 	waitpid(pid2, &status, WNOHANG);//only one waitpid because stdout of child one is needed for stdin for child two, and WHOHANG that if something goes wrong to not wait for this
// 	// if (WIFEXITED(status))
// 	// 	exitcode = WEXITSTATUS(status);
// 	// return (exitcode);
// 	return (0);
// }
