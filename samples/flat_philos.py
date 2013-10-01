#! /usr/bin/env python3.3
from string import Template

places = Template(\
"""<module name="Philo$id">
    <place id="Fork$id" name="Fork$id" marking="1" />
    <place id="Idle$id" name="Idle$id" marking="1" />
    <place id="WaitLeft$id" name="WaitLeft$id" marking="0" />
    <place id="WaitRight$id" name="WaitRight$id" marking="0" />
    <place id="HasLeft$id" name="HasLeft$id" marking="0" />
    <place id="HasRight$id" name="HasRight$id" marking="0" />
</module>
""")

transitions = Template(\
"""<transition name="GoEat$id" id="GoEat$id">
    <pre ref="Idle$id" weight="1" />
    <post ref="WaitLeft$id" weight="1" />
    <post ref="WaitRight$id" weight="1" />
</transition>
<transition name="GoLeft$id" id="GoLeft$id">
    <pre ref="WaitLeft$id" weight="1" />
    <pre ref="Fork$previous_id" weight="1" />
    <post ref="HasLeft$id" weight="1" />
</transition>
<transition name="GoRight$id" id="GoRight$id">
    <pre ref="WaitRight$id" weight="1" />
    <pre ref="Fork$id" weight="1" />
    <post ref="HasRight$id" weight="1" />
</transition>
<transition name="Release$id" id="Release$id">
    <pre ref="HasLeft$id" weight="1" />
    <pre ref="HasRight$id" weight="1" />
    <post ref="Idle$id" weight="1" />
    <post ref="Fork$previous_id" weight="1" />
    <post ref="Fork$id" weight="1" />
</transition>
""")

def philo_places(id):
    return places.substitute(id=id)

def philo_transitions(id, max):
    previous = id - 1
    if id == 0:
        previous = max - 1
    return transitions.substitute(id=id, previous_id=previous)

def main(nb, outfile):
    with open(outfile, 'w') as f:
        f.write("""<net name="Philo">\n""")
        f.write("<places>\n")
        for id in range(nb):
            f.write(philo_places(id))
        f.write("</places>\n")
        f.write("<transitions>\n")
        for id in range(nb):
            f.write(philo_transitions(id, nb))
        f.write("</transitions>\n")
        f.write("</net>")

if __name__ == '__main__':
    nb = 2000
    outfile="/Users/hal/Desktop/philo" + str(nb) + ".xml"
    main(nb, outfile)
