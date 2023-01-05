B = 1
R = 2

def analysis_pattern(pattern_fn, all_data):
    exist_B_win = 0
    exist_R_win = 0
    not_exist_B_win = 0
    not_exist_R_win = 0
    for data, label in all_data:
        if pattern_fn(data):
            if label == B:
                exist_B_win += 1
            else:
                exist_R_win += 1
        else:
            if label == B:
                not_exist_B_win += 1
            else:
                not_exist_R_win += 1

    return [exist_B_win, exist_R_win, not_exist_B_win, not_exist_R_win]

def analysis_pattern_param(pattern_fn, all_data, args):
    exist_B_win = 0
    exist_R_win = 0
    not_exist_B_win = 0
    not_exist_R_win = 0
    for data, label in all_data:
        if pattern_fn(data, args):
            if label == B:
                exist_B_win += 1
            else:
                exist_R_win += 1
        else:
            if label == B:
                not_exist_B_win += 1
            else:
                not_exist_R_win += 1

    return [exist_B_win, exist_R_win, not_exist_B_win, not_exist_R_win]