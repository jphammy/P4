LOAD 2
STORE V0
LOAD 1
SUB V0
BRNEG L0
BRPOS L0
LOAD 1
STORE V1
WRITE V1
L0: NOOP
STOP
V0 0
V1 0
