import random
import time
import sys

def display(sol):
    #�b���w����m�L�a���ӦZ���ѽL
    n = len(sol)
    for i in range(0,n):
        for j in range(0,n):
            if sol[i] == j:
                print(' Q ', end=' ')
            else:
                print(" = ", end=' ')
        print()


def diagonals(sol):
    #�T�w�ѽL�C���﨤�u�W���ӦZ�ơC
    n = len(sol)        
    ndiag = 2*n-1       
    diag_up = [0 for i in range(ndiag)]
    diag_dn = [0 for i in range(ndiag)]

    # �p��C�ӹ﨤�u�Q����������
    for i in range(n):
        d = i + sol[i] 
        diag_up[d] += 1
        d = (n-1) + sol[i] - i  
        diag_dn[d] += 1

    return diag_up, diag_dn


def collisions(diag):
    #��^�﨤�u�W���I���`��
    ncolls = 0
    for i in diag:
        if i>1:         
            ncolls += i-1
    return ncolls


def exchange(i, j, sol, diag_up, diag_dn):

    n = len(sol)

    # �﨤�u���A�������
    d = i + sol[i]; diag_up[d] -=  1
    d = j + sol[j]; diag_up[d] -=  1

    d = (n-1) - i + sol[i]; diag_dn[d] -= 1
    d = (n-1) - j + sol[j]; diag_dn[d] -= 1

    # �洫��m 'i' �M 'j'
    sol[i], sol[j] = sol[j], sol[i]

    #�}�l����������﨤�u
    d = i + sol[i]; diag_up[d] +=  1
    d = j + sol[j]; diag_up[d] +=  1

    d = (n-1) - i + sol[i]; diag_dn[d] += 1
    d = (n-1) - j + sol[j]; diag_dn[d] += 1


def decrease(di,dj,ni,nj):
    #��ӦZ�Q�����ɡA�p��I���Q�����C
    delta = 0 
    if ni >= 2: delta -= 1
    if nj >= 2: delta -= 1
    if di == dj and ni == 2: delta += 1 # �W�L�@�ӡA��
    return delta


def increase(di,dj,ni,nj):
    #��ӦZ�w��ɭp��s���I���C
    #- di, dj --�ӦZ��m�﨤�u�W
    #- ni, nj --�﨤�u�W���ӦZ�ƥ�
    
    delta = 0
    if ni >= 1: delta += 1
    if nj >= 1: delta += 1
    if di == dj and ni == 0: delta += 1 #�b�P�@�﨤�u�W
    return delta
    

def evaluate_move(i, j, sol, diag_up, diag_dn):
#������ i ��ӦZ�P�� j ��ӦZ���洫�C

    delta = 0
    n = len(sol)

# �p�G���ʳQ�����A�﨤�u���A�������
    upi = i + sol[i]            # �� i ��ӦZ����e�V�W�﨤�u
    upj = j + sol[j]            
    delta += decrease(upi,upj,diag_up[upi],diag_up[upj])

    dni = (n-1) + sol[i] - i    # �� i ��ӦZ����e�V�U�﨤�u
    dnj = (n-1) + sol[j] - j    
    delta += decrease(dni,dnj,diag_dn[dni],diag_dn[dnj])

    # �}�l�Q�������﨤�u
    upi = i + sol[j]            # �� i ��ӦZ���s�V�W�﨤�u
    upj = j + sol[i]            
    delta += increase(upi,upj,diag_up[upi],diag_up[upj])
    
    dni = (n-1) + sol[j] - i    # �� i ��ӦZ���s�V�U�﨤�u
    dnj = (n-1) + sol[i] - j    
    delta += increase(dni,dnj,diag_dn[dni],diag_dn[dnj])

    return delta


    
def find_move(n_iter, tabu, best_colls, sol, diag_up, diag_dn, ncolls):
    #��^�㦳�̨β��ʪ����ա]i�Aj�^�C

    #�ˬd��e�ѨM��פ��Ҧ��i�઺���ʡA�ÿ�ܤ@��:
    # *���OTABU
    # *�O TABU �������@��з�

    #�Կ�C��ѩҦ����i��ʲզ�


    n = len(sol)
    best_delta = n      #�̨εo�{���ʪ�����
    for i in range(0,n-1):
        for j in range(i+1,n):
            delta = evaluate_move(i,j,sol,diag_up,diag_dn)
            print("move %d-%d" % (i,j), "-> delta=%d;" % delta, end=' ')
            if tabu[i] >= n_iter:
                print("move is tabu;", end=' ')
                if ncolls + delta < best_colls:
                    print("aspiration criterion is satisfied;", end=' ')
            print()
                
            if (tabu[i] < n_iter         # ���OTabu�A�����@��з�
                or ncolls + delta < best_colls):
                if delta < best_delta:
                    best_delta = delta
                    best_i = i
                    best_j = j

    return best_i, best_j, best_delta


def tabu_search(tabulen, maxiter, sol):
    #�T�ҷj���G�q sol �}�l�j�����ѨM��סC

    #�ѼơG
    #-tabulen�G���ʧ����᪺�T�ҭ��N����
    #-maxiter -�]����^�̤j���N����
    #-sol -�̦n���ѨM���
    #"""
    n = len(sol)
    diag_up,diag_dn = diagonals(sol)
    print("\ninitial board")
    display(sol)
    ncolls = collisions(diag_up) + collisions(diag_dn)
    print("collisions:", ncolls)

    n_iter = 0          
    iter_best = 0       
    best = list(sol)    
    best_colls = ncolls
    start_time1 = time.time()
    tabu = [0 for i in range(n)]

    while (n_iter < maxiter)  and  (best_colls != 0):
        #start_time2 = time.time()
        n_iter += 1
        print("\niteration", n_iter)

        (i,j,delta) = find_move(n_iter, tabu, best_colls, sol, diag_up, diag_dn, ncolls)
        print("best move is: change rows ", i, "and", j, ", delta", delta)
        exchange(i, j, sol, diag_up, diag_dn)
        ncolls += delta
        tabu[i] = n_iter + tabulen

        if ncolls < best_colls:
            iter_best = n_iter
            best = list(sol)
            best_colls = ncolls

        display(sol)
        #end_time2 = time.time() - start_time2
        #print("collisions:", ncolls)
        #print("Execution Time: {:.3f} s".format(end_time2))

    #�ƻs��쪺�̨θѨM���
    sol = best  
    ncolls = best_colls
    end_time1 = time.time() - start_time1
    print()
    print("final solution (found at iteration", iter_best, ")")
    print("final tabu list:")
    print(tabu)
    display(sol)
    #print("collisions:", ncolls)
    print("Total Time: {:.3f} s".format(end_time1))
    print("Avg time:{:.3f} s".format(end_time1 / n_iter))
    return ncolls

def main():
    if len(sys.argv) == 1:
        n = 8
    elif  len(sys.argv) == 2:
        n = int(sys.argv[1])
    elif  len(sys.argv) == 3:
        n = int(sys.argv[1])
        rnd = int(sys.argv[2])
        random.seed(rnd)
        print("random seed:", rnd)
    else:
        print("usage:", sys.argv[0], "[board_size [rnd_seed]]")
        exit(-1)
        
    print("*** graph partitioning problem ***")
    print()

    print("instance randomly created")
    print("board size:", n)
    sol = list(range(n))      # �]�m�� [0,1,...,n-1]
    random.shuffle(sol) 
    print("initial solution (random):")
    print("array:" , sol)
    display(sol)
    up,dn = diagonals(sol)
    print("queens on upward diagonals:", up)
    print("queens on downward diagonals:", dn)
    ncolls = collisions(up) + collisions(dn)

    if ncolls != 0:
        print("\n\n\nstarting tabu search")
        tabulen = n/2
        maxiter = 10000
        ncolls = tabu_search(tabulen, maxiter, sol)
    
    else:
        display(sol)
        print("\nno further improvements are possible, not doing tabu search")
    exit(0)
