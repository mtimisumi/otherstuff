#include "pipex.h"

#define CHILD 0
#define READ 0
#define WRITE 1

char	*get_env(char **envp)
{
	char	*env = NULL;
	int		i;

	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
			env = envp[i] + 5;
		else
			i++;
	}
	if (!env) // If PATH= was never found
	{
		write(STDERR_FILENO, "no env found\n", 13);
		exit(127);
	}
	return (env);
}

char	*get_cmd(char *cmd, char **envp)
{
	char	*env;
	char	**paths;
	char	*path;
	char	*temp;
	int		i;

	i = 0;
	env = get_env(envp);
	paths = ft_split(env, ':');
	while (paths[i])
	{
		temp = ft_strjoin(paths[i], "/");
		path = ft_strjoin(temp, cmd);
		if (access(path, X_OK) == 0)
			return (path); // Return the valid path
		free(path);
		i++;
	}
	return (NULL); // Command not found
}

void	child_process_one(int *pip, char **argv, char **envp)
{
	int		infile;
	char	*cmd;
	char	**cmd_flags;

	close(pip[READ]); // Close the read end of the pipe
	if (dup2(pip[WRITE], STDOUT_FILENO) == -1)
	{
		perror("dup2 child1 failed");
		exit(1);
	}
	close(pip[WRITE]); // Close the write end after duplicating
	infile = open(argv[1], O_RDONLY);
	if (infile == -1)
	{
		perror("no valid infile");
		exit(1);
	}
	if (dup2(infile, STDIN_FILENO) == -1)
	{
		perror("dup2 child1 fail");
		exit(1);
	}
	close (infile);
	cmd_flags = ft_split(argv[2], ' ');
	cmd = get_cmd(cmd_flags[0], envp);
	// if (cmd != NULL)
	// {
	// 	if (execve(cmd, cmd_flags, envp) == -1)
	// 	{
	// 		perror("execve child1 fail");
	// 		exit(1);
	// 	}
	// }
	// else
	// {
	// 	perror("cmd1 fail");
	// 	exit(1);
	// }
	if (!cmd)
	{
		perror("cmd1 not found");
		exit(127);
	}
	printf("Executing: %s\n", cmd);  // DEBUG
    execve(cmd, cmd_flags, envp);
    perror("execve1 failed");
    exit(1);
}

void	child_process_two(int *pip, char **argv, char **envp)
{
	int		outfile;
	char	*cmd;
	char	**cmd_flags;

	outfile = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (outfile == -1)
	{
		perror("error with outfile");
		exit(1);
	}
	close(pip[WRITE]); // Close the write end of the pipe
	if (dup2(outfile, STDOUT_FILENO) == -1)
	{
		perror("dup2 child2 failed");
		exit(1);
	}
	close(outfile);
	if (dup2(pip[READ], STDIN_FILENO) == -1)
	{
		perror("dup2 child 2 failed");
		exit(1);
	}
	close(pip[READ]); // Close the read end after duplicating
	cmd_flags = ft_split(argv[3], ' ');
	cmd = get_cmd(cmd_flags[0], envp);
	// if (cmd != NULL)
	// {
	// 	if (execve(cmd, cmd_flags, envp) == -1)
	// 	{
	// 		perror("execve1 fail");
	// 		exit(1);
	// 	}
	// }
	// else
	// {
	// 	perror("cmd2 fail");
	// 	exit(1);
	// }
	if (!cmd)
	{
		perror("cmd2 not found");
		exit(127);
	}
	printf("Executing: %s\n", cmd);  // DEBUG
    execve(cmd, cmd_flags, envp);
    perror("execve2 failed");
    exit(1);
}

int	pipex(char **argv, char **envp)
{
    int pip[2];
    pid_t pid1, pid2;

	// pipe(pip);
    if (pipe(pip) == -1)
        return (perror("pipe failed"), 1);

    pid1 = fork();
    if (pid1 == -1)
        return (perror("forking pid1 failed"), 1);
    if (pid1 == CHILD)
        child_process_one(pip, argv, envp);
	// free(cmds);
    pid2 = fork();
    if (pid2 == -1)
        return (perror("forking pid2 failed"), 1);
    if (pid2 == CHILD)
        child_process_two(pip, argv, envp);

	//free(cmds)e
    // Close both ends of the pipe in the parent
	wait(NULL);
    // int status;
    // waitpid(pid1, NULL, 0);
    // waitpid(pid2, &status, 0);

	// int	exitcode = 0;
	// if (WIFEXITED(status))
	// {
	// 	exitcode = WEXITSTATUS(status);
	// 	return (exitcode);
	// }
    close(pip[WRITE]);
    close(pip[READ]);
    return (0);
}

int	main(int argc, char *argv[], char *envp[])
{
	if (argc != 5)
		return (write(STDERR_FILENO, "not good amount of arguments\n", 29), 1);
	pipex(argv, envp);
	return (0);
}
