import string
import numpy as np
digs = string.digits + string.ascii_letters


def int2base(x, base):
    if x < 0:
        sign = -1
    elif x == 0:
        return digs[0]
    else:
        sign = 1

    x *= sign
    digits = []

    while x:
        digits.append(digs[x % base])
        x = x // base

    if sign < 0:
        digits.append('-')

    digits.reverse()

    return ''.join(digits)

def cal_prob(state):
    prob = [0, 0, 0, 0, 0, 0]
    
    for i in range(6):
        if(state[i] == '1'):
            prob[i] += 1
        else:
            for j in range(i+1, 6, 1):
                if(state[j] == '1'):
                    prob[j] += 1
                    break
            for j in range(i-1, -1, -1):
                if(state[j] == '1'):
                    prob[j] += 1
                    break
    
    return prob
            

if __name__ == "__main__":
    state_list = []
    for i in range(64):
        state = int2base(i, 2).zfill(6)
        state_list.append(state)
    
    prob_list = []
    for state in state_list:
        prob = cal_prob(state)
        prob_list.append(prob)
    
    prob_list = np.array(prob_list)
    print(prob_list)