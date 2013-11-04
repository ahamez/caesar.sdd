#!/usr/bin/env python

import sys

#####################################################

one_philo_places = \
"""
pl P%(id)dThink     (1)
pl P%(id)dHasLeft   (0)
pl P%(id)dHasRight  (0)
pl P%(id)dEat       (0)
pl P%(id)dFork      (1)
"""

#####################################################

one_philo_arcs = \
"""
# Internal evolution of philosopher %(id)d
tr P%(id)dTakeLeft1   P%(id)dThink     P%(id)dFork  ->    P%(id)dHasLeft
tr P%(id)dTakeRight1  P%(id)dThink     P%(di)dFork  ->    P%(id)dHasRight
tr P%(id)dTakeRight2  P%(id)dHasLeft   P%(di)dFork  ->    P%(id)dEat
tr P%(id)dTakeLeft2   P%(id)dHasRight  P%(id)dFork  ->    P%(id)dEat
tr P%(id)dGoThink     P%(id)dEat    ->    P%(id)dThink P%(id)dFork P%(di)dFork   
"""

#####################################################

nb_philos = 2
if len(sys.argv) > 1:
  nb_philos = int(sys.argv[1])

f = open("philos" + str(nb_philos) + ".net", "w" )

nodes    = []
arcs     = []

for p in range(nb_philos):
  
  nodes    += [one_philo_places  % {"id":p}]

  di = (p-1) % (nb_philos)
  arcs     += [one_philo_arcs    % {"id":p , "di":di}]

f.write("net philos\n")

for n in nodes:
  f.write(n)

for a in arcs:
  f.write(a)

