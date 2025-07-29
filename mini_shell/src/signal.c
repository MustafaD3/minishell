#include "../minishell.h"

void sigint_handler(int sig, siginfo_t *info, void *context)
{
    (void)sig;
    (void)info;
    (void)context;
    
    write(STDOUT_FILENO, "\n", 1);
    rl_on_new_line();
    rl_replace_line("", 0);
    rl_redisplay();
}

void setup_signals(void)
{
    struct sigaction sa;
    
    // SIGINT (Ctrl+C) handler
    sa.sa_sigaction = sigint_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_SIGINFO | SA_RESTART;
    sigaction(SIGINT, &sa, NULL);
    
    // SIGQUIT (Ctrl+\) ignore
    signal(SIGQUIT, SIG_IGN);
}