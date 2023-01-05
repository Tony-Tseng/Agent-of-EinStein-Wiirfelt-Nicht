import numpy as np

def get_nearest_blue(data):
    target = 0
    blue_position = get_blue_position(data)
    blue_distance = np.abs( blue_position - target)
    blue_distance_manhattan = [ (distance // 5 + distance % 5) for distance in blue_distance ]
    
    if len(blue_distance_manhattan) == 0:
        return None, None, None
    
    manhattan_min_index = np.argmin(blue_distance_manhattan)
    min_blue_position = blue_position[manhattan_min_index]
    
    return min_blue_position, data[min_blue_position], blue_distance_manhattan[manhattan_min_index]

def get_nearest_red(data):
    target = 24
    red_position = get_red_position(data)
    red_distance = np.abs( red_position - target)
    red_distance_manhattan = [ (distance // 5 + distance % 5) for distance in red_distance ]
    
    if len(red_distance_manhattan) == 0:
        return None, None, None
    
    manhattan_min_index = np.argmin(red_distance_manhattan)
    min_red_position = red_position[manhattan_min_index]
    
    return min_red_position, data[min_red_position], red_distance_manhattan[manhattan_min_index]

def get_furthest_blue(data):
    target = 0
    blue_position = get_blue_position(data)
    blue_distance = np.abs( blue_position - target)
    blue_distance_manhattan = [ (distance // 5 + distance % 5) for distance in blue_distance ]
    
    if len(blue_distance_manhattan) == 0:
        return None, None, 8
    
    manhattan_max_index = np.argmax(blue_distance_manhattan)
    max_blue_position = blue_position[manhattan_max_index]
    
    return max_blue_position, data[max_blue_position], blue_distance_manhattan[manhattan_max_index]

def get_furthest_red(data):
    target = 24
    red_position = get_red_position(data)
    red_distance = np.abs( red_position - target)
    red_distance_manhattan = [ (distance // 5 + distance % 5) for distance in red_distance ]
    
    if len(red_distance_manhattan) == 0:
        return None, None, 8
    
    manhattan_max_index = np.argmax(red_distance_manhattan)
    max_red_position = red_position[manhattan_max_index]
    
    return max_red_position, data[manhattan_max_index], red_distance_manhattan[manhattan_max_index]

def get_number_of_blue_cubes(data):
    blue_num_and_empty = (data<=6).sum()
    empty = (data==0).sum()
    return blue_num_and_empty - empty

def get_number_of_red_cubes(data):    
    return (data>6).sum()

def get_blue_position(data):
    return np.array(np.where( (data<=6) & (data>0) ))[0]

def get_red_position(data):
    return np.array(np.where( data>6 ))[0]

def get_blue_serial(data):
    candidate = np.extract(data<=6, data)
    return sorted(np.extract(candidate!=0, candidate))

def get_red_serial(data):
    return sorted(np.extract(data>6, data))

def serial_exist(data, serial):
    return (data == serial).sum() > 0

def serial_exist_at_pos(data, args):
    serial, pos = args[0], args[1]
    data[pos] == serial

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

def get_serial_prob(data, serial):
    state = ""
    if(serial <= 6):
        serial = serial - 1
        blue_serial = get_blue_serial(data)
        for i in range(1, 7):
            if(i in blue_serial):
                state += '1'
            else:
                state += '0'
            
    else:
        serial = serial - 7
        red_serial = get_red_serial(data)
        for i in range(7, 13):
            if(i in red_serial):
                state += '1'
            else:
                state += '0'
    
    return cal_prob(state)[serial]