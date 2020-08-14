import sys, random, pygame
import pygame.gfxdraw
import math
from math import pi, sin, cos
from colour import Color
pygame.init()

size = width, height = 600, 600
trail_length = 400

# The starting position is randomized.
a1 = random.randint(int(pi/2),int(2*pi))
a2 = random.randint(int(pi/2),int(2*pi))

# The control-variables are intitialized
acc1 = 0
acc2 = 0
v1 = 0
v2 = 0
r1 = 120
r2 = 80
m1 = 6
m2 = 3
g = 0.1
x0 = width//2
y0 = height//3
x1,y1 = x0, y0 + r1
x2,y2 = x1, y1 + r2

black = 0, 0, 0
white = 255, 255, 255
red = 255, 0, 0

# The calculation for the acceleration of the first pendulum.
def update_acc1(g, m1, m2, a1, a2, v2, v1, r2, r1):
	nom1 = -g*(2*m1+m2)*sin(a1)
	nom2 = -m2*g*sin(a1-2*a2)
	nom3 = -2*sin(a1-a2)*m2
	nom4 = v2*v2*r2+v1*v1*r1*cos(a1-a2)
	den = r1*(2*m1+m2-m2*cos(2*a1-2*a2))
	return (nom1+nom2+nom3*nom4) / den

# The calculation for the acceleration of the second pendulum.
def update_acc2(g, m1, m2, a1, a2, v2, v1, r2, r1):
	nom1 = 2*sin(a1-a2)
	nom2 = v1*v1*r1*(m1+m2)
	nom3 = g*(m1+m2)*cos(a1)
	nom4 = v2*v2*r2*m2*cos(a1-a2)
	den = r2*(2*m1+m2-m2*cos(2*a1-2*a2))
	return nom1*(nom2 + nom3 + nom4) / den

# This function creates a colorlist to color the pendulum's route.
def create_colorlist():
	violet = Color("red")
	colors = list(violet.range_to(Color("violet"), trail_length))
	clist = []
	for ele in colors:
		temp = [ele.rgb[0], ele.rgb[1], ele.rgb[2]]
		temp[0] = temp[0] * 255
		temp[1] = temp[1] * 255
		temp[2] = temp[2] * 255
		temp2 = (temp[0], temp[1], temp[2])
		clist.append(temp2)
	return clist

# This function adds the new point to the points-list.
def update_points(new_x, new_y, points):
	return [(new_x, new_y)] + points[:-1]

# This function calculates a new angle for a pendulum.
def update_angle(x, y, r, angle):
	new_x = x+r*cos(angle)
	new_y = y+r*sin(angle)

	return new_x, new_y

screen = pygame.display.set_mode(size)
bg = pygame.Surface(screen.get_size())
bg.fill(black)
screen.blit(bg, (0,0))
pygame.display.flip()
clock=pygame.time.Clock()
clist = create_colorlist()

points = []
i = 0

# This loop handles the vizualization
while True:
	for event in pygame.event.get():
		if event.type == pygame.QUIT: 
			sys.exit()

	# New accelerations are calculated.
	acc1 = update_acc1(g, m1, m2, a1, a2, v2, v1, r2, r1)
	acc2 = update_acc2(g, m1, m2, a1, a2, v2, v1, r2, r1)
	
	# The speed and positions are updated accordingly
	v1 += acc1
	v2 += acc2
	a1 += v1
	a2 += v2
	a1_temp = a1 + pi/2
	a2_temp = a2 + pi/2

	# gamepy needs the coordinates as integers.
	x0,y0,x1,y1,x2,y2 = int(x0),int(y0),int(x1),int(y1),int(x2),int(y2)

	if i > 1:
		if len(points) < trail_length:
			points = [(x2,y2)] + points
		else:
			points = update_points(x2, y2, points)
		i = 0

	x1, y1 = update_angle(x0,y0,r1,a1_temp)
	x2, y2 = update_angle(x1,y1,r2,a2_temp)

	screen.blit(bg, (0,0))

	point0 = 0,0
	c = 0
	
	# This loop draws the trailing points.
	for point in points:
		if c == 0:
			point0 = point
		else:
			# Optionally a line could be drawn between the trailing points.
			#pygame.draw.aaline(screen, clist[c], point0, point)
			point0 = point
		pygame.gfxdraw.aacircle(screen, point[0],point[1], 2, clist[c])
		pygame.gfxdraw.filled_circle(screen, point[0], point[1], 2, clist[c])
		c += 1

	# The heads of the pendulum are drawn with the gfxdraw-module to get smooth circles.
	pygame.draw.aaline(screen, white, (x0,y0), (x1,y1))
	pygame.gfxdraw.aacircle(screen, int(x1), int(y1), m1, white)
	pygame.gfxdraw.filled_circle(screen, int(x1), int(y1), m1, white)

	pygame.draw.aaline(screen, white, (x1,y1), (x2,y2))
	pygame.gfxdraw.aacircle(screen, int(x2), int(y2), m2, white)
	pygame.gfxdraw.filled_circle(screen, int(x2), int(y2), m2, white)

	i += 1
	
	clock.tick(200)
	pygame.display.flip()


