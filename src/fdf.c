/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blackrider <blackrider@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/10 20:09:11 by blackrider        #+#    #+#             */
/*   Updated: 2024/04/12 11:49:04 by blackrider       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../hdrs/fdf.h"
#include "../minilibx-linux/mlx.h"
#include <stdio.h>
#include <math.h>

void	brensenhem(t_mlxdata *app, t_crd *crd)
{
	long			color;
	int				z;
	int				z1;
	t_dt			dt;

	z = app->map->crd[(int)(crd->y)][(int)(crd->x)].z;
	z1 = app->map->crd[(int)(crd->yf)][(int)(crd->xf)].z;
	color = setcolor(app, crd);
	scale_crd(crd, app->sc);
	isometric(&crd->x, &crd->y, z);
	isometric(&crd->xf, &crd->yf, z1);
	setvenue(app, crd);
	dt.dx = crd->xf - crd->x;
	dt.dy = crd->yf - crd->y;
	dt.max = MAX(MOD(dt.dx), MOD(dt.dy));
	dt.dx /= dt.max;
	dt.dy /= dt.max;
	while ((int)(crd->xf - crd->x) || (int)(crd->yf - crd->y))
	{
		setpixel(app, crd->x, crd->y, color);
		crd->x += dt.dx;
		crd->y += dt.dy;
	}
}

void	drawmap(t_mlxdata *app)
{
	int		x;
	int		y;
	t_crd	crd;

	if (!app)
		return ;
	y = 0;
	while (y < app->map->size_y)
	{
		x = 0;
		while (x < app->map->size_x)
		{
			if (x < app->map->size_x - 1)
				brensenhem(app, setcrd_xy(&crd, x, y, 0));
			if (y < app->map->size_y - 1)
				brensenhem(app, setcrd_xy(&crd, x, y, 1));
			++x;
		}
		++y;
	}
	mlx_clear_window(app->app, app->wnd);
	mlx_put_image_to_window(app->app, app->wnd, app->img->img_ptr, 0, 0);
}

t_scale	*crtscale(t_map *map, int size_x, int size_y, float k)
{
	t_scale *scale;

	scale = malloc(sizeof(t_scale));
	scale->xscale = 1 * ((float)size_x / k) / (float)(map->size_x);
	scale->yscale = 1 * (((float)size_y / k) / (float)(map->size_y));
	return (scale);
}

int	main(void)
{
	int			alfa;
	t_scale		*scale;
	t_map		*map;
	t_mlxdata	*app;

	map = createmap("../maps/test_maps/t1.fdf");
	scale = crtscale(map, SIZE_X, SIZE_Y, 2);
	app = crt_mlxdata(map, scale);
	drawmap(app);
	mlx_hook(app->wnd, 17, 1L<<3, exitapp, app);
	mlx_loop(app->app);
	return (0);
}
