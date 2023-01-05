import numpy as np
import pandas as pd
from glob import glob
from analyze import *
from strategy import *

B = 1
R = 2

def grab(filename):
    File = open(filename, "r")

    init = None
    moves = []
    move_id = 1
    for string in File.readlines():
        string = string.rstrip()
        if string.startswith("* ini "):
            init = string[len("* ini "):].split(" ")
        elif string.startswith(f"* {move_id}. "):
            moves.append(string[len(f"* {move_id}. "):].split(" ")[-2:])
            move_id += 1
        elif string.endswith(" wins"):
            win_condition = string.split(" ")[1]
            winner = string.split(" ")[-2]
            
    result = R if winner == "a1" else B
    
    if win_condition not in ["ChessTotheCorner", "NoMove"]:
        return []

    board = np.zeros(25, dtype=np.int8)

    MAP = {'A': 0, 'B': 1, 'C': 2, 'D': 3, 'E': 4,
           '1': 0, '2': 1, '3': 2, '4': 3, '5': 4}
    
    for i in range(1, 13):
        position = MAP[init[i-1][1]] * 5 + MAP[init[i-1][0]]
        board[position] = i

    output = []
    total_move = len(moves)
    for i, (f, t) in enumerate(moves):
        _prev = MAP[f[1]] * 5 + MAP[f[0]]
        _next = MAP[t[1]] * 5 + MAP[t[0]]
        
        piece = board[_prev]
        board[_next] = board[_prev]
        board[_prev] = 0
        
        # if total_move-i <= 5:
        if i > 5:
            output.append( [board.copy(), result, piece] )
        
    return output, total_move

def generate_analysis(all_data):
    # # 0, 1
    # print("blue distance score more than red", analysis_pattern(blue_distance_better, all_data))
    # print("red distance score more than blue", analysis_pattern(red_distance_better, all_data))
    
    # # 2, 3
    # print("enemy for blue more than enemy for red", analysis_pattern(enemy_for_blue_more_than_enemy_for_red, all_data))
    # print("enemy for red more than enemy for blue", analysis_pattern(enemy_for_red_more_than_enemy_for_blue, all_data))
    
    # # 4, 5
    # print("blue more than red at row", analysis_pattern(blue_more_than_red_row, all_data))
    # print("red more than blue at row", analysis_pattern(red_more_than_blue_row, all_data))
    
    # # 6, 7
    # print("blue more than red at col", analysis_pattern(blue_more_than_red_col, all_data))
    # print("red more than blue at col", analysis_pattern(red_more_than_blue_col, all_data))
    
    # # 8, 13
    # for i in range(1, 7):
    #     print(f"blue nearest number {i}", analysis_pattern_param(blue_nearest_num, all_data, i))
        
    # # 14, 19
    # for i in range(7, 13):
    #     print(f"red nearest number {i}", analysis_pattern_param(red_nearest_num, all_data, i))
        
    # # 20, 21
    # print("blue protected", analysis_pattern(blue_protection, all_data))
    # print("red protected", analysis_pattern(red_protection, all_data))
    
    # 22, 23
    print("red more than blue corner", analysis_pattern(red_more_than_blue_corner, all_data))
    print("blue more than red corner", analysis_pattern(blue_more_than_red_corner, all_data))

def generate_dataset(all_data):
    
    all_feat = []
    for data, label, piece in all_data:
        feat = []
        # 0~2
        # feat.append( ( red_distance_score(data) -  blue_distance_score(data)))
        feat.append( (get_red_step_to_win(data) - get_blue_step_to_win(data)))
        feat.append( (get_red_serial_prob(data) - get_blue_serial_prob(data)))
        
        feat.append( red_average_distance_score(data) )
        feat.append( blue_average_distance_score(data) )
        
        feat.append( 1-((piece-1)//6) - 0.5 )
        # feat.append( piece%6 )
        # feat.append( number_of_red_can_eat_blue(data) )
        # feat.append( number_of_blue_can_eat_red(data) )
        
        # feat.append( red_furthest_distance(data) - blue_furthest_distance(data) )
        
        # feat.append( prob_of_red_near_than_blue(data) )
        # feat.append( prob_of_blue_near_than_red(data) )
        
        # feat.append( number_of_blue_near_than_red(data) )
        # feat.append( (blue_enemy_in_front_of_red(data) == 0) * 1)
        # feat.append( (red_enemy_in_front_of_blue(data) == 0) * 1)
        # feat.append( (red_protection_num(data) - blue_protection_num(data)) )
        
        
        # feat.append( (blue_at_corner(data) - red_at_corner(data)) )
        # feat.append( (get_blue_prob_sum(data) - get_red_prob_sum(data)) )
        
        # feat.append( blue_number_at_same_row(data) )
        # feat.append( red_number_at_same_row(data) )
        # feat.append( blue_number_at_same_col(data) )
        # feat.append( red_number_at_same_col(data) )
        
        # 1
        # feat.append( (red_enemy_in_front_of_blue(data) - blue_enemy_in_front_of_red(data)) )
        # feat.append( red_enemy_in_front_of_blue(data) )
        # feat.append( blue_enemy_in_front_of_red(data) )
        
        # 8, 13
        # for i in [1, 2, 3, 4, 5, 6]:
        #     feat.append( blue_nearest_num(data, i) * 1)
            
        # # 14, 19
        # for i in [7, 8, 9, 10, 11, 12]:
        #     feat.append( red_nearest_num(data, i) * 1)
        
        # 16
        # feat.append( (blue_protection_num(data) - red_protection_num(data)) )
        # feat.append( blue_protection_num(data) )
        # feat.append( red_protection_num(data) )
    
        # 17
        # feat.append( blue_at_corner(data) )
        # feat.append( red_at_corner(data) )
        
        # 18
        # feat.append( get_blue_serial_prob(data) )
        # feat.append( get_red_serial_prob(data) )
        
        # GT: 18
        feat.append( (label == R ) * 1)
        
        all_feat.append(feat)
    return all_feat

if __name__ == "__main__":
    all_data = []
    move_count = []
    for filename in glob("EinSteinChess/*/*.txt"):
        data, move = grab(filename)
        all_data.extend(data)
        move_count.append(move)
    move_count = np.array(move_count)
    print("The average move step is ", np.mean(move_count))
        
    # generate_analysis(all_data)
    # data = all_data[77][0]
    
    feat = generate_dataset(all_data) * 1
    df = pd.DataFrame(feat)
    df.to_csv("Einstein_dataset.csv", index=False)