import numpy as np
import pandas as pd
from glob import glob
from tqdm import tqdm
import itertools
from analyze import *
from utils import *

B = 1
R = 2

# ///////////////////  distance  ///////////////////////
def blue_distance_better(data):
    return blue_distance_score(data) >= red_distance_score(data)

def red_distance_better(data):
    return red_distance_score(data) >= blue_distance_score(data)

def blue_distance_score(data):
    blue_pos, blue_serial, blue_manhattan = get_nearest_blue(data)
    
    if blue_pos == None:
        return 8
    
    return 8 - blue_manhattan

def red_distance_score(data):
    red_pos, red_serial, red_manhattan = get_nearest_red(data)
    
    if red_pos == None:
        return 8
    
    return 8 - red_manhattan

def red_furthest_distance(data):
    _, _, red_manhattan = get_furthest_red(data)
    
    if red_manhattan == None:
        return 0
    return red_manhattan

def blue_furthest_distance(data):
    _, _, blue_manhattan = get_furthest_blue(data)
    
    if blue_manhattan == None:
        return 0
    return blue_manhattan

#  ////////////////  step  //////////////////////
def get_blue_step_to_win(data):
    blue_pos, _, _ = get_nearest_blue(data)
    if blue_pos == None:
        return 0
    
    return 4 - max(blue_pos//5, blue_pos%5)

def get_red_step_to_win(data):
    red_pos, _, _ = get_nearest_red(data)
    if red_pos == None:
        return 0
    return 4 - max((24-red_pos)//5, (24-red_pos)%5)

# ////////////////  enemy compare  //////////////////////
def enemy_for_red_more_than_enemy_for_blue(data):
    return red_enemy_in_front_of_blue(data) < blue_enemy_in_front_of_red(data)

def enemy_for_blue_more_than_enemy_for_red(data):
    return red_enemy_in_front_of_blue(data) > blue_enemy_in_front_of_red(data)

def red_enemy_in_front_of_blue(data):
    blue_pos, blue_serial, blue_manhattan = get_nearest_blue(data)
    
    if blue_pos == None:
        return 6
    
    min_row = blue_pos// 5
    min_col = blue_pos% 5
    red_position = get_red_position(data)
    enemy = 0
    for position in red_position:
        if( position//5 <= min_row and position%5 <= min_col):
            enemy += 1
    
    return enemy

def blue_enemy_in_front_of_red(data):
    red_pos, red_serial, red_manhattan = get_nearest_red(data)
    
    if red_pos == None:
        return 6
    
    max_row = red_pos// 5
    max_col = red_pos% 5
    blue_position = get_blue_position(data)
    enemy = 0
    for position in blue_position:
        if( position//5 >= max_row and position%5 >= max_col):
            enemy += 1
    
    return enemy

# ////////////////  ally count  //////////////////////
def blue_number_at_same_row(data):
    blue_pos, _, _ = get_nearest_blue(data)
    
    if blue_pos == None:
        return 0
    blue_row = blue_pos // 5
    
    return B_at_row(data, blue_row)

def red_number_at_same_row(data):
    red_pos, _, _ = get_nearest_red(data)
    
    if red_pos == None:
        return 0
    red_row = red_pos // 5
    
    return R_at_row(data, red_row)

def blue_number_at_same_col(data):
    blue_pos, _, _ = get_nearest_blue(data)
    
    if blue_pos == None:
        return 0
    blue_col = blue_pos % 5
    
    return B_at_col(data, blue_col)

def red_number_at_same_col(data):
    red_pos, _, _ = get_nearest_red(data)
    
    if red_pos == None:
        return 0
    red_col = red_pos % 5
    
    return R_at_col(data, red_col)

#  ////////////////  row, col compare  //////////////////////
def red_more_than_blue_row(data):
    red_pos, _, _ = get_nearest_red(data)
    
    if red_pos == None:
        return False
    red_row = red_pos // 5
    
    return R_at_row(data, red_row) > B_at_row(data, red_row)

def blue_more_than_red_row(data):
    blue_pos, _, _ = get_nearest_blue(data)
    
    if blue_pos == None:
        return False
    
    blue_row = blue_pos // 5
    
    return R_at_row(data, blue_row) < B_at_row(data, blue_row)

def red_more_than_blue_col(data):
    red_pos, _, _ = get_nearest_red(data)
    if red_pos == None:
        return False
    red_col = red_pos % 5
    
    return R_at_col(data, red_col) > B_at_col(data, red_col)

def blue_more_than_red_col(data):
    blue_pos, _, _ = get_nearest_blue(data)
    
    if blue_pos == None:
        return False
    
    blue_col = blue_pos % 5
    
    return R_at_col(data, blue_col) < B_at_col(data, blue_col)

def R_at_row(data, row):
    return get_number_of_red_cubes(data[row*5:row*5+5])

def B_at_row(data, row):
    return get_number_of_blue_cubes(data[row*5:row*5+5])

def R_at_col(data, col):
    return get_number_of_red_cubes(data[col:21+col:5])

def B_at_col(data, col):
    return get_number_of_blue_cubes(data[col:21+col:5])

# ////////////////  nearest serial number //////////////////////
def red_nearest_num(data, number):
    _, red_serial, _ = get_nearest_red(data)
    if red_serial == None:
        return False
    return red_serial == number

def blue_nearest_num(data, number):
    _, blue_serial, _ = get_nearest_blue(data)
    if blue_serial == None:
        return False
    return blue_serial == number

# ////////////////  protection  //////////////////////
def blue_protection_num(data):
    protection = 0
    blue_pos, _, _ = get_nearest_blue(data)
    
    if blue_pos == None:
        return protection
    
    blue_position = get_blue_position(data)
    if(blue_pos//5 < 4 and blue_pos%5 < 4):
        if blue_pos+6 in blue_position:
            protection += 1
    if( blue_pos//5 < 4 ):
        if blue_pos+5 in blue_position:
            protection += 1
    if( blue_pos%5 < 4 ):
        if blue_pos+1 in blue_position:
            protection += 1
    return protection

def red_protection_num(data):
    protection = 0
    red_pos, _, _ = get_nearest_red(data)
    
    if red_pos == None:
        return protection
    
    red_position = get_red_position(data)
    if(red_pos//5 > 0 and red_pos%5 > 0):
        if red_pos-6 in red_position:
            protection += 1
    if( red_pos//5 > 0 ):
        if red_pos-5 in red_position:
            protection += 1
    if( red_pos%5 > 0 ):
        if red_pos-1 in red_position:
            protection += 1
    return protection

# ////////////////  corner  //////////////////////
def red_more_than_blue_corner(data):
    return red_at_corner(data) > blue_at_corner(data)

def blue_more_than_red_corner(data):
    return red_at_corner(data) < blue_at_corner(data)

def red_at_corner(data):
    count = 0
    red_position = get_red_position(data)
    
    for position in red_position:
        if position == 0 or position == 1 or position == 5:
            count += 1
    return count

def blue_at_corner(data):
    count = 0
    blue_position = get_blue_position(data)
    for position in blue_position:
        if position == 24 or position == 23 or position == 19:
            count += 1
    
    return count

# ////////////////  serial prob  //////////////////////
def get_blue_serial_prob(data):
    _, blue_serial, _ = get_nearest_blue(data)
    if blue_serial == None:
        return 0
    
    return get_serial_prob(data, blue_serial)

def get_red_serial_prob(data):
    _, red_serial, _ = get_nearest_red(data)
    if red_serial == None:
        return 0
    return get_serial_prob(data, red_serial)

def get_blue_prob_sum(data):
    prob = 0
    for i in range(1, 7):
        prob += get_serial_prob(data, i)
    return prob

def get_red_prob_sum(data):
    prob = 0
    for i in range(7, 13):
        prob += get_serial_prob(data, i)
    return prob

def get_blue_second_prob(data):
    blue_serial = get_blue_serial(data)
    if len(blue_serial) < 2:
        return 0
    
    prob_list = list()
    for serial in blue_serial:
        prob_list.append(get_serial_prob(data, serial))
    sorted(prob_list)
    
    return prob_list[1]

def get_red_second_prob(data):
    red_serial = get_red_serial(data)
    if len(red_serial) < 2:
        return 0
    
    prob_list = list()
    for serial in red_serial:
        prob_list.append(get_serial_prob(data, serial))
    sorted(prob_list)
    
    return prob_list[1]

def prob_of_blue_near_than_red(data):
    _, _, red_manhattan = get_nearest_red(data)
    blue_pos = get_blue_position(data)
    
    blue_serial = data[blue_pos]
    
    if red_manhattan == None:
        return 6
    
    if blue_pos.shape[0] == 0:
        return -6
    blue_manhattan = np.array([ pos//5+pos%5 for pos in blue_pos ])
    near_index = np.where( blue_manhattan <= red_manhattan )[0]
    
    prob = 0
    for index in near_index:
        prob += get_serial_prob(data, blue_serial[index])
    
    return prob

def prob_of_red_near_than_blue(data):
    _, _, blue_manhattan = get_nearest_blue(data)
    red_pos = get_red_position(data)
    
    red_serial = data[red_pos]
    
    if blue_manhattan == None:
        return 6
    
    if red_pos.shape[0] == 0:
        return -6
    
    red_manhattan = np.array([ (24-pos)//5+(24-pos)%5 for pos in red_pos ])
    near_index = np.where( red_manhattan <= blue_manhattan )[0]
    
    prob = 0
    for index in near_index:
        prob += get_serial_prob(data, red_serial[index])
    
    return prob

# ////////////////  number of cube will be eaten  //////////////////////
def number_of_blue_can_eat_red(data):
    enemy = blue_enemy_in_front_of_red(data)
    if enemy == 0:
        return 0
    
    red_pos, _, _ = get_nearest_red(data)
    blue_pos = get_blue_position(data)
    
    if red_pos == None:
        return 1
    if blue_pos.shape[0] == 0:
        return 0
    
    red_danger_zone = []
    if red_pos // 5 < 4 and red_pos % 5 < 4:
        red_danger_zone.append(red_pos+6)
    if red_pos // 5 < 4:
        red_danger_zone.append(red_pos+5)
    if red_pos % 5 < 4:
        red_danger_zone.append(red_pos+1)
    
    danger = 0
    for pos in blue_pos:
        if( pos in red_danger_zone ):
            danger+=1
    return danger/enemy

def number_of_red_can_eat_blue(data):
    enemy = red_enemy_in_front_of_blue(data)
    if enemy == 0:
        return 0

    blue_pos, _, _ = get_nearest_blue(data)
    red_pos = get_red_position(data)
    
    if blue_pos == None:
        return 1
    
    if red_pos.shape[0] == 0:
        return 0
    
    blue_danger_zone = []
    if blue_pos // 5 > 0 and blue_pos % 5 > 0:
        blue_danger_zone.append(blue_pos-6)
    if blue_pos // 5 > 0:
        blue_danger_zone.append(blue_pos-5)
    if blue_pos % 5 < 4:
        blue_danger_zone.append(blue_pos-1)
    
    danger = 0
    for pos in red_pos:
        if( pos in blue_danger_zone ):
            danger+=1
    return danger/enemy
    

def blue_average_distance_score(data):
    blue_pos = get_blue_position(data)
    
    if blue_pos.shape[0] == 0:
        return 8
    distance = 0
    for pos in blue_pos:
        manhattan = pos//5+pos%5
        distance += manhattan
    
    return 8 - distance/blue_pos.shape[0]

def red_average_distance_score(data):
    red_pos = get_red_position(data)
    
    if red_pos.shape[0] == 0:
        return 8
    distance = 0
    for pos in red_pos:
        manhattan = pos//5+pos%5
        distance += manhattan
    
    return 8 - distance/red_pos.shape[0]

# def R_at_position(data, args):
#     return data[args[0]] > 6

# def B_at_position(data, args):
#     return data[args[0]] > 0 and data[args[0]] <= 6

# def blue_num(data, args):
#     return get_blue_num(data) == args[0]

# def red_num(data, args):
#     return get_red_num(data) == args[0]

# def B_more_than_R(data):
#     return get_blue_num(data) > get_red_num(data)

# def R_more_than_B(data):
#     return get_blue_num(data) < get_red_num(data)

# def number_generation(start_number):
#     powerSet = []
#     range_list = [i for i in range(start_number, start_number+6)]
#     for k in range(1, 7):
#         powerSet.extend(itertools.combinations(range_list, k))
    
#     return powerSet

# def exist_blue_cube(data, target_list):
#     exist_blue = get_blue(data)
#     return exist_blue == target_list

# def exist_red_cube(data, target_list):
#     exist_red = get_red(data)
#     return exist_red == target_list



# def generate_analysis(all_data):
#     pass
    # print("B_more_than_R", analysis_pattern(B_more_than_R, all_data))
    # print("R_more_than_B", analysis_pattern(R_more_than_B, all_data))
    
    # for i in [0, 1, 5]:
    #     print(f"R_at_position {i}", analysis_pattern_param(R_at_position, all_data, (i,)))
    # for i in [19, 23, 24]:
    #     print(f"B_at_position {i}", analysis_pattern_param(B_at_position, all_data, (i,)))
        
    # for i in range(1, 13):
    #     print(f"{i} alive", analysis_pattern_param(number_exist, all_data, i))
    
    # for i in range(7, 13):
    #     for j in [0, 1, 5]:
    #         print(f"{i} alive at pos {j}", analysis_pattern_param(number_exist_at_position, all_data, (i, j)))
            
    # for i in range(1, 7):
    #     for j in [19, 23, 24]:
    #         print(f"{i} alive at pos {j}", analysis_pattern_param(number_exist_at_position, all_data, (i, j)))
        
    # for i in range(1, 7):
    #     print(f"Numbers of Blue cube {i}", analysis_pattern_param(blue_num, all_data, (i,)))
    #     print(f"Numbers of Red cube {i}", analysis_pattern_param(red_num, all_data, (i,)))
        
    # for i in range(5):
    #     for j in range(0, 4):
    #         print(f"R {j} numbers at col {i}", analysis_pattern_param(R_number_at_col, all_data, (i,j)))
            
    # for i in range(5):
    #     for j in range(0, 4):
    #         print(f"B {j} numbers at col {i}", analysis_pattern_param(B_number_at_col, all_data, (i,j)))
    
    # for j in range(0, 5):
    #     print(f" B more than R at row {j}", analysis_pattern_param(blue_more_than_red_at_row, all_data, j))
    # for j in range(0, 5):
    #     print(f" B more than R at col {j}", analysis_pattern_param(blue_more_than_red_at_col, all_data, j))
        
    # for j in range(0, 5):
    #     print(f" R more than B at row {j}", analysis_pattern_param(red_more_than_blue_at_row, all_data, j))
    # for j in range(0, 5):
    #     print(f" R more than B at col {j}", analysis_pattern_param(red_more_than_blue_at_col, all_data, j))
    
    # combination_list = number_generation(1)
    # for combination in combination_list:
    #     combination = list(combination)
    #     print(f"B {combination} alive", analysis_pattern_param(exist_blue_cube, all_data, combination))
        
    # combination_list = number_generation(7)
    # for combination in combination_list:
    #     combination = list(combination)
    #     print(f"R {combination} alive", analysis_pattern_param(exist_red_cube, all_data, combination))
        
    # print(f"blue score more than red", analysis_pattern(blue_distance_better, all_data))
    # print(f"red score more than blue", analysis_pattern(red_distance_better, all_data))


# def generate_dataset(all_data):
#     blue_combination_list = number_generation(1)
#     red_combination_list = number_generation(7)
    
#     all_feat = []
#     for data, label in all_data:
#         feat = []
        
#         # 0
#         feat.append(B_more_than_R(data))
#         # 1
#         feat.append(R_more_than_B(data))
#         # 2, 3, 4
#         for i in [0, 1, 5]:
#             feat.append(R_at_position(data, (i, )))
#         # 5, 6, 7
#         for i in [19, 23, 24]:
#             feat.append(B_at_position(data, (i, )))
#         # 8 - 19
#         for i in range(1, 13):
#             feat.append(number_exist(data, i))
#         # 20 - 37
#         for i in range(7, 13):
#             for j in [0, 1, 5]:
#                 feat.append(number_exist_at_position(data, (i, j)))
#         # 38 - 55
#         for i in range(1, 7):
#             for j in [24, 23, 19]:
#                 feat.append(number_exist_at_position(data, (i, j)))
#         # 56 - 61
#         for i in range(1, 7):
#             feat.append(blue_num(data, (i, )))
#         # 62 - 67
#         for i in range(1, 7):
#             feat.append(red_num(data, (i, )))
#         # 68 - 87
#         for i in range(5):
#             for j in range(0, 4):
#                 feat.append(R_number_at_row(data, (i,j)))
#         # 88 - 107
#         for i in range(5):
#             for j in range(0, 4):
#                 feat.append(B_number_at_row(data, (4-i,j)))
        
#         # 108
#         feat.append(blue_distance_score(data))
#         # 109
#         feat.append(red_distance_score(data))
                
#         # # 110 - 173
#         for combination in blue_combination_list:
#             combination = list(combination)
#             feat.append(exist_blue_cube(data, combination))
#         # # 174 - 237
#         for combination in red_combination_list:
#             combination = list(combination)
#             feat.append(exist_red_cube(data, combination))
        
#         # GT
#         feat.append(label==R)
        
#         all_feat.append(feat)
    
#     return all_feat