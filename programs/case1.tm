#Q = {q0,del,del1,del0,q1,halt_accept}
#S = {0,1}
#G = {0,1,_}
#q0 = q0
#B = _
#F = {halt_accept}
#N = 1

q0 * * r q0
q0 _ _ l del
del 0 _ l del0
del 1 _ l del1
del0 * * l del0
del0 _ 0 r q1
del1 * * l del1
del1 _ 1 r q1
q1 * * r q1
q1 _ _ l halt_accept