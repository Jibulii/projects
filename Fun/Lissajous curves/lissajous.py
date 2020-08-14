import sys, random, pygame
import pygame.gfxdraw
import math
from math import pi, sin, cos
from colour import Color
pygame.init()

# This function creates a colorlist to color the different curves.
def create_colorlist(num):
	violet = Color("red")
	colors = list(violet.range_to(Color("violet"), num))
	clist = []

	# The colors have to be converted to values which Pygame understands.
	for ele in colors:
		temp = [ele.rgb[0], ele.rgb[1], ele.rgb[2]]
		temp[0] = temp[0] * 255
		temp[1] = temp[1] * 255
		temp[2] = temp[2] * 255
		temp2 = (temp[0], temp[1], temp[2])
		clist.append(temp2)
	return clist

# The circle class is used to create the rotating circles.
class Circle():
	def __init__(self, x, y, speed):
		self.speed = speed
		self.x = x
		self.y = y
		self.angle = 0

	def update(self):
		self.angle += self.speed
		if self.angle >= 2*pi:
			self.angle = 0
		px = self.x+radius*cos(self.angle)
		py = self.y+radius*sin(-self.angle)
		return math.floor(px), math.floor(py)


black = 0, 0, 0
white = 255, 255, 255
red = 255, 0, 0

radius = 30

speed = 0.01
a_angle = 0

# The screen size is created dynamically depending on the number of circles.
n_x = 8
n_y = 6
Bs = 3*radius
Is = 2.5*radius
size = width, height = int(Bs+Is*n_x), int(Bs+Is*n_y)

screen = pygame.display.set_mode(size)
bg = pygame.Surface(screen.get_size())
bg.fill(black)
screen.blit(bg, (0,0))
pygame.display.flip()
clock=pygame.time.Clock()

# forms-list stores the drawn lissajous curves.
forms = []

# xlist and ylist store the rotating circles.
xlist = []
ylist = []
for i in range(2,n_x+2):
	xlist.append(Circle(i*2*(radius+int(radius/8)), radius+10, (i-1)*speed))

for i in range(2, n_y+2):
	ylist.append(Circle(radius+10, i*2*(radius+int(radius/8)),(i-1)*speed))
	
for x in xlist:
	for y in ylist:
		forms.append([])

colorlist = create_colorlist(len(forms))

while True:
	for event in pygame.event.get():
		if event.type == pygame.QUIT:
			sys.exit()

	screen.blit(bg, (0,0))
	xpoints = []
	ypoints = []

	# This loop draws the xlist-circles and calculates the x-coordinates for the interceptions.
	for i in range (len(xlist)):
		pygame.gfxdraw.aacircle(screen, xlist[i].x, xlist[i].y, radius, white)
		x0, y0 = xlist[i].update()
		pygame.gfxdraw.aacircle(screen, x0, y0, 3, white)
		pygame.draw.aaline(screen, white, (x0, 0), (x0, height))
		xpoints.append(x0)

	# This loop draws the ylist-circles and calculates the x-coordinates for the interceptions.
	for i in range(len(ylist)):
		pygame.gfxdraw.aacircle(screen, ylist[i].x, ylist[i].y, radius, white)
		x1, y1 = ylist[i].update()
		pygame.gfxdraw.aacircle(screen, x1, y1, 3, white)
		pygame.draw.aaline(screen, white, (0,y1), (width, y1))
		ypoints.append(y1)

	i = 0
	# The intercept-points are added into the forms-list.
	for xp in xpoints:
		for yp in ypoints:
			forms[i].append((xp, yp))
			i += 1
	
	color = 0
	# The resulting lissajous-curves are drawn from the forms-list.
	for form in forms:
		c = 0
		for point in form:
			if c == 0:
				point0 = point
			else:
				pygame.draw.aaline(screen, colorlist[color], point0, point)
				point0 = point
			c += 1
		color+=1
	
	clock.tick(150)
	pygame.display.flip()

	# The forms-list is cleared after the slowest circles complete a full rotation.
	a_angle += speed
	if a_angle >= 2*pi:
		forms = []
		for x in xlist:
			x.angle = 0
			for y in ylist:
				forms.append([])
		for y in ylist:
			y.angle = 0
		a_angle = 0
		clock.tick(5)
