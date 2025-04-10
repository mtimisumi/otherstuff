/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmisumi <mmisumi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 15:45:19 by mmisumi           #+#    #+#             */
/*   Updated: 2025/04/10 19:51:25 by mmisumi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

#define CHILD 0
#define READ 0
#define WRITE 1


void perror_free_exit(const char *msg, char **cmd_flags, int exit_code)
{
	int	i;

	i = 0;
	perror(msg);
	while (cmd_flags && cmd_flags[i])
	{
		free(cmd_flags[i]);
		i++;
	}
	exit(exit_code);
}
void	child_process_one(int *pip, char **argv, char **envp)
{
	int		infile;
	char	*cmd;
	char	**cmd_flags;
	
	close(pip[READ]);
	infile = open(argv[1], O_RDONLY);
	if (infile == -1)
		perror_free_exit("Error opening infile\n", NULL, 0);
	dup2(infile, STDIN_FILENO);
	close(infile);
	dup2(pip[WRITE], STDOUT_FILENO);
	close(pip[WRITE]);
	cmd_flags = ft_split(argv[2], ' ');
	if (cmd_flags == NULL)
		perror_free_exit("Error splitting cmd1\n", NULL, 1);
	cmd = get_cmd(cmd_flags[0], envp);
	if (cmd != NULL)
	{
		if (access(cmd, X_OK) == -1)
			perror_free_exit("Error executing cmd1\n", cmd_flags, 126);
		execve(cmd, cmd_flags, envp);
		perror_free_exit("Error executing cmd1\n", cmd_flags, 1);
	}
	perror_free_exit("Invalid cmd1\n", cmd_flags, 127);
}

void	child_process_two(int *pip, char **argv, char **envp)
{
	int		outfile;
	char	*cmd;
	char	**cmd_flags;
	
	close(pip[WRITE]);
	outfile = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (outfile == -1)
		perror_free_exit("Error opening outfile\n", NULL, 0);
	dup2(pip[READ], STDIN_FILENO);
	close(pip[READ]);
	dup2(outfile, STDOUT_FILENO);
	close(outfile);
	cmd_flags = ft_split(argv[3], ' ');
	if (cmd_flags == NULL)
		perror_free_exit("Error splitting cmd2\n", NULL, 1);
	cmd = get_cmd(cmd_flags[0], envp);
	if (cmd != NULL)
	{
		if (access(cmd, X_OK) == -1)
			perror_free_exit("Error executing command", cmd_flags, 126);
		execve(cmd, cmd_flags, envp);
		perror_free_exit("Error executing cmd2\n", cmd_flags, 1);
	}
	perror_free_exit("Invalid cmd2\n", cmd_flags, 127);
}

int	pipex(char **argv, char **envp)
{
	int		pip[2];
	pid_t	pid1;
	pid_t	pid2;
	int		status;
	
	if (pipe(pip) == -1)
		perror_free_exit("Error creating pipe\n", NULL, 1);
	pid1 = fork();
	if (pid1 == -1)
		perror_free_exit("Error forking pid1\n", NULL, -1);
	if (pid1 == CHILD)
		child_process_one(pip, argv, envp);
	close(pip[WRITE]);
	pid2 = fork();
	if (pid2 == -1)
		perror_free_exit("Error forking pid2\n", NULL, -1);
	if (pid2 == CHILD)
		child_process_two(pip, argv, envp);
	close(pip[READ]);
	status = 0;
	waitpid(pid1, NULL, 0);
	waitpid(pid2, &status, 0);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (0);
}

int	main(int argc, char *argv[], char *envp[])
{
	if (argc != 5)
	{
		write(STDERR_FILENO, "invalid amount of arguments\n", 28);
		return (0);
	}
	return (pipex(argv, envp));
}


// void	child_process_one(int *pip, char **argv, char **envp)
// {
// 	int		infile;
// 	char	*cmd;
// 	char	**cmd_flags;
	
// 	close(pip[READ]);
// 	infile = open(argv[1], O_RDONLY);
// 	if (infile == -1)
// 		perror_exit("Error opening infile\n", 0);
// 	dup2(infile, STDIN_FILENO);
// 	close(infile);
// 	dup2(pip[WRITE], STDOUT_FILENO);
// 	close(pip[WRITE]);
// 	if (cmd_flags == NULL)
// 	perror_exit("Error splitting cmd1\n", 1);
// 	cmd_flags = ft_split(argv[2], ' ');
// 	cmd = get_cmd(cmd_flags[0], envp);
// 	if (cmd != NULL)
// 	{
// 		if (access(cmd, X_OK) == -1)
// 			perror_exit("Error executing cmd1\n", 126);
// 		execve(cmd, cmd_flags, envp);
// 		perror_exit("Error executing cmd1\n", 1);
// 	}
// 	perror_exit("Invalid cmd1\n", 127);
// }

// void	child_process_two(int *pip, char **argv, char **envp)
// {
// 	int		outfile;
// 	char	*cmd;
// 	char	**cmd_flags;
	
// 	close(pip[WRITE]);
// 	outfile = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0644);
// 	if (outfile == -1)
// 		perror_exit("Error opening outfile\n", 0);
// 	dup2(pip[READ], STDIN_FILENO);
// 	close(pip[READ]);
// 	dup2(outfile, STDOUT_FILENO);
// 	close(outfile);
// 	cmd_flags = ft_split(argv[3], ' ');
// 	if (cmd_flags == NULL)
// 		perror_exit("Error splitting cmd2\n", 1);
// 	cmd = get_cmd(cmd_flags[0], envp);
// 	if (cmd != NULL)
// 	{
// 		if (access(cmd, X_OK) == -1)
// 			perror_exit("Error executing command", 126);
// 		execve(cmd, cmd_flags, envp);
// 		perror_exit("Error executing cmd2\n", 1);
// 	}
// 	perror_exit("Invalid cmd2\n", 127);
// }

// int	pipex(char **argv, char **envp)
// {
// 	int		pip[2];
// 	pid_t	pid1;
// 	pid_t	pid2;
// 	int		status;
	
// 	if (pipe(pip) == -1)
// 		perror_exit("Error creating pipe\n", 1);
// 	pid1 = fork();
// 	if (pid1 == -1)
// 		perror_exit("Error forking pid1\n", -1);
// 	if (pid1 == CHILD)
// 		child_process_one(pip, argv, envp);
// 	close(pip[WRITE]);
// 	pid2 = fork();
// 	if (pid2 == -1)
// 		perror_exit("Error forking pid2\n", -1);
// 	if (pid2 == CHILD)
// 		child_process_two(pip, argv, envp);
// 	close(pip[READ]);
// 	status = 0;
// 	waitpid(pid1, NULL, 0);
// 	waitpid(pid2, &status, 0);
// 	if (WIFEXITED(status))
// 		return (WEXITSTATUS(status));
// 	return (0);
// }

// int	main(int argc, char *argv[], char *envp[])
// {
// 	if (argc != 5)
// 	{
// 		write(STDERR_FILENO, "invalid amount of arguments\n", 28);
// 		return (0);
// 	}
// 	return (pipex(argv, envp));
// }

