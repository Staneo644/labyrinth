/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   labyrinthe.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajossera <ajossera@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/21 11:55:40 by ajossera          #+#    #+#             */
/*   Updated: 2021/11/21 11:55:40 by ajossera         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <sys/random.h>

int     **ft_malloc(int sizex, int sizey)
{
    int    **rend;
    int     i;

    sizey = sizey * 2 + 1;
    sizex = sizex * 2 + 1;
    i = 0;
    rend = malloc(sizeof(int *) * sizex);
    if (!rend)
        return (0);
    while (i < sizex)
    {
        rend[i] = malloc(sizeof(int) * sizey);
        if (!rend[i])
        {
            while (i-- >= 0)
            {
                free(rend[i]);
            }
            return (NULL);
        }
        i++;
    }
    return (rend);
}

int    **ft_rempli(int sizex, int sizey, int a, int **tab)
{
    int i;
    int Y;
    int X;

    i = 257;
    sizey = sizey * 2 + 1;
    sizex = sizex * 2 + 1;
    X = 0;
    Y = 0;
    while (X < sizex)
    {
        Y = 0;
        while(Y < sizey)
        {
            if (Y % 2 == 0 || X % 2 == 0)
                tab[X][Y] = a;
            else
            {
                i++;
                tab[X][Y] = i;
            }
            Y++;
        }
        X++;
    }
    return (tab);
}

int     ft_check_tab(int **tab, int sizex, int sizey)
{
    int nb;
    int Y;

    Y = sizey;
    nb = tab[1][1];
    while (sizex > 0)
    {
        sizey = Y;
        while (sizey > 0)
        {
            if (tab[sizex * 2 - 1][sizey * 2 - 1] != nb)
                return (1);
            
            sizey --;
        }
        sizex --;
    }
    return (0);
}

int     ft_alea()
{
    int i;
    int a;

    a = rand();
    i = rand();
    if (i > a)
        return (1);
    if (a > i)
        return (-1);
    return (ft_alea());
}

int     **ft_recursive_change(int   **tab, int  i, int nbx, int nby, int a)
{
    tab[nbx][nby] = i;
    if (tab[nbx + 1][nby] != a && tab[nbx + 1][nby] != i)
        tab = ft_recursive_change(tab, i, nbx + 1, nby, a);
    if (tab[nbx - 1][nby] != a && tab[nbx - 1][nby] != i)
        tab = ft_recursive_change(tab, i, nbx - 1, nby, a);
    if (tab[nbx][nby + 1] != a && tab[nbx][nby + 1] != i)
        tab = ft_recursive_change(tab, i, nbx, nby + 1, a);
    if (tab[nbx][nby - 1] != a && tab[nbx][nby - 1] != i)
        tab = ft_recursive_change(tab, i, nbx, nby - 1, a);
    return (tab);
}

int     **ft_check_one(int **tab, int nbx, int nby, int sizex, int sizey, int a)
{
    int orientationX;
    int orientationY;

    if (ft_alea() == 1)
    {
        orientationX = ft_alea();
        orientationY = 0;
    }
    else
    {
        orientationX = 0;
        orientationY = ft_alea();
    }
    if (orientationX == -1 && nbx == 1)
        return (tab);
    if (orientationY == -1 && nby == 1)
        return (tab);
    if (orientationX == 1 && nbx == sizex * 2 - 1)
        return (tab);
    if (orientationY == 1 && nby == sizey * 2 - 1)
        return (tab);
    if (tab[nbx + 2 * orientationX][nby + 2 * orientationY] == tab[nbx][nby])
        return (tab);
    else
    {
        tab = ft_recursive_change(tab, tab[nbx][nby], nbx + orientationX, nby + orientationY, a);
    }
    return (tab);
}

void    ft_putchar(char c)
{
    //dprintf(1, "%c", c);
    write(1, &c, 1);
}

void    ft_write(int **tab, int sizex, int sizey, int a, int b)
{
    int i;
    int j;

    i = 0;
    j = 0;
    while (i <= sizex * 2)
    {
        j = 0;
        while( j <= sizey * 2)
        {
            if (tab[i][j] != a)
            {
                ft_putchar((char)b);
                ft_putchar((char)b);
            }
            else
            {
                ft_putchar((char)a);
                ft_putchar((char)a);
            }
            j++;
        }
        ft_putchar('\n');
        i++;
    }
}

int     **ft_change_alea(int sizex, int sizey, int a, int **tab)
{
    int i;
    int j;

    i = 1;
    j = 1;
    while (i <= sizex)
    {
        j = 1;
        while (j <= sizey)
        {
            ft_write(tab, sizex, sizey, a, ' ');
            ft_putchar('\n');
            tab = ft_check_one(tab, i * 2 - 1, j * 2 - 1, sizex, sizey, a);
            j ++;
        }
        i ++;
    }
    return (tab);
}

void ft_next_time(int a, int b, int sizex, int sizey)
{
    int    **tab;

    
    tab = ft_malloc(sizex, sizey);
    //ft_write(tab, sizex, sizey, a, b);
    if (!tab)
        return ;
    tab = ft_rempli(sizex, sizey, a, tab);
    ft_write(tab, sizex, sizey, a, b);
    while (ft_check_tab(tab, sizex, sizey))
        tab = ft_change_alea(sizex, sizey, a, tab);
    tab[1][0] = b;
    tab[sizex * 2 - 1][sizey * 2] = b;
    write(1, "\n <<RENDU FINAL : >> \n \n \n", 26);
    ft_write(tab, sizex, sizey, a, b);

}

int     main(int argc, char **argv)
{
    char    a;
    char    b;
    int     sizex;
    int     sizey;

    if (argc != 5)
    {
        if (argc == 1)
        {
            write(1, "le premier argument est le caractère mur, le deuxième est le caractère vide, le troisième est la taille x, le quatrième est la taille y ", 142);
            return (0);
        }
        write(1, "nombre d'argument invalide", 27);
        return (0);
    }
    srand(time(NULL));
    a = argv[1][0];
    b = argv[2][0];
    sizex = atoi(argv[3]);
    sizey = atoi(argv[4]);
    if (sizey < 1 || sizex < 1)
    {
        write(1, "impossible de faire un labyrinthe qui a une taille inférieur à 1\n", 68);
        return (0);
    }
    ft_next_time(a, b, sizex, sizey);
    return (0);
}
