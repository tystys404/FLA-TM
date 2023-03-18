#Q = {q0,halt_accept,add1,add2,cmp,mh,halt_reject,accept,accept1,accept2,accept3,accept4,reject,reject1,reject2,reject3,reject4,reject5}
#S = {1}
#G = {1,_,t,r,u,e,f,a,l,s}
#q0 = q0
#B = _
#F = {accept}
#N = 2

q0 1_ 11 ** cmp
cmp 11 11 rr cmp
cmp 1_ 1_ ** add1
add1 1_ 11 *r add2
add2 1_ 11 ** mh
mh 11 11 *l mh
mh 1_ 1_ *r cmp
cmp _1 _1 ll halt_reject
cmp __ __ ll halt_accept
halt_accept 11 __ ll halt_accept
halt_accept 1_ __ l* halt_accept
halt_accept __ __ r* accept1
accept1 __ t_ r* accept2
accept2 __ r_ r* accept3
accept3 __ u_ r* accept4
accept4 __ e_ r* accept
halt_reject 11 __ ll halt_reject
halt_reject 1_ __ l* halt_reject
halt_reject __ __ r* reject1
reject1 __ f_ r* reject2
reject2 __ a_ r* reject3
reject3 __ l_ r* reject4
reject4 __ s_ r* reject5
reject5 __ e_ r* reject