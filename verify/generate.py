
f = open("All_log_new_5.txt", "r")
all_log = f.readlines()

input = open("input.txt", "w")
ans = open("gt_ans.txt", "w")
for idx, log in enumerate(all_log):
    # if idx < 100:
    #     continue
    if idx%2 == 0:
        input.write(log)
    else:
        ans.write(log)
        
    if idx > 100:
        break
        