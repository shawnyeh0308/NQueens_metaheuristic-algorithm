import random
import time
import sys

def display(sol):
    #在給定的位置印帶有皇后的棋盤
    n = len(sol)
    for i in range(0,n):
        for j in range(0,n):
            if sol[i] == j:
                print(' Q ', end=' ')
            else:
                print(" = ", end=' ')
        print()


def diagonals(sol):
    #確定棋盤每條對角線上的皇后數。
    n = len(sol)        
    ndiag = 2*n-1       
    diag_up = [0 for i in range(ndiag)]
    diag_dn = [0 for i in range(ndiag)]

    # 計算每個對角線被攻擊的次數
    for i in range(n):
        d = i + sol[i] 
        diag_up[d] += 1
        d = (n-1) + sol[i] - i  
        diag_dn[d] += 1

    return diag_up, diag_dn


def collisions(diag):
    #返回對角線上的碰撞總數
    ncolls = 0
    for i in diag:
        if i>1:         
            ncolls += i-1
    return ncolls


def exchange(i, j, sol, diag_up, diag_dn):

    n = len(sol)

    # 對角線不再受到攻擊
    d = i + sol[i]; diag_up[d] -=  1
    d = j + sol[j]; diag_up[d] -=  1

    d = (n-1) - i + sol[i]; diag_dn[d] -= 1
    d = (n-1) - j + sol[j]; diag_dn[d] -= 1

    # 交換位置 'i' 和 'j'
    sol[i], sol[j] = sol[j], sol[i]

    #開始受到攻擊的對角線
    d = i + sol[i]; diag_up[d] +=  1
    d = j + sol[j]; diag_up[d] +=  1

    d = (n-1) - i + sol[i]; diag_dn[d] += 1
    d = (n-1) - j + sol[j]; diag_dn[d] += 1


def decrease(di,dj,ni,nj):
    #當皇后被移除時，計算碰撞被移除。
    delta = 0 
    if ni >= 2: delta -= 1
    if nj >= 2: delta -= 1
    if di == dj and ni == 2: delta += 1 # 超過一個，更換
    return delta


def increase(di,dj,ni,nj):
    #當皇后定位時計算新的碰撞。
    #- di, dj --皇后放置對角線上
    #- ni, nj --對角線上的皇后數目
    
    delta = 0
    if ni >= 1: delta += 1
    if nj >= 1: delta += 1
    if di == dj and ni == 0: delta += 1 #在同一對角線上
    return delta
    

def evaluate_move(i, j, sol, diag_up, diag_dn):
#評估第 i 行皇后與第 j 行皇后的交換。

    delta = 0
    n = len(sol)

# 如果移動被接受，對角線不再受到攻擊
    upi = i + sol[i]            # 第 i 行皇后的當前向上對角線
    upj = j + sol[j]            
    delta += decrease(upi,upj,diag_up[upi],diag_up[upj])

    dni = (n-1) + sol[i] - i    # 第 i 行皇后的當前向下對角線
    dnj = (n-1) + sol[j] - j    
    delta += decrease(dni,dnj,diag_dn[dni],diag_dn[dnj])

    # 開始被攻擊的對角線
    upi = i + sol[j]            # 第 i 行皇后的新向上對角線
    upj = j + sol[i]            
    delta += increase(upi,upj,diag_up[upi],diag_up[upj])
    
    dni = (n-1) + sol[j] - i    # 第 i 行皇后的新向下對角線
    dnj = (n-1) + sol[i] - j    
    delta += increase(dni,dnj,diag_dn[dni],diag_dn[dnj])

    return delta


    
def find_move(n_iter, tabu, best_colls, sol, diag_up, diag_dn, ncolls):
    #返回具有最佳移動的元組（i，j）。

    #檢查當前解決方案中所有可能的移動，並選擇一個:
    # *不是TABU
    # *是 TABU 但滿足願望標準

    #候選列表由所有的可能性組成


    n = len(sol)
    best_delta = n      #最佳發現移動的價值
    for i in range(0,n-1):
        for j in range(i+1,n):
            delta = evaluate_move(i,j,sol,diag_up,diag_dn)
            print("move %d-%d" % (i,j), "-> delta=%d;" % delta, end=' ')
            if tabu[i] >= n_iter:
                print("move is tabu;", end=' ')
                if ncolls + delta < best_colls:
                    print("aspiration criterion is satisfied;", end=' ')
            print()
                
            if (tabu[i] < n_iter         # 不是Tabu，滿足願望標準
                or ncolls + delta < best_colls):
                if delta < best_delta:
                    best_delta = delta
                    best_i = i
                    best_j = j

    return best_i, best_j, best_delta


def tabu_search(tabulen, maxiter, sol):
    #禁忌搜索：從 sol 開始搜索找到解決方案。

    #參數：
    #-tabulen：移動完成後的禁忌迭代次數
    #-maxiter -（絕對）最大迭代次數
    #-sol -最好的解決方案
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

    #複製找到的最佳解決方案
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
    sol = list(range(n))      # 設置解 [0,1,...,n-1]
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
