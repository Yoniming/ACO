#! /bin/python
import os
from pprint import pprint
import time

dir = "/run/media/baoyixing/NYTOSHIBA/ACO_data"
cmd = './cmake-build-debug/code -t'

file_list = [os.path.join(dir, f) for f in os.listdir(dir) if os.path.isfile(os.path.join(dir, f)) and not f.startswith(
    "N")]
pprint(file_list)

for f in file_list:
    f_name = os.path.splitext(f)[0]
    f_n = f_name.split('/')[-1]
    f_name1 = os.path.join(dir, "tmp", f_n)

    f_out_name = f_name1 + '_c' + '.out'
    cmd_str = cmd + ' -c -m 0 ' + f + ' ' + f_out_name  # + ' | ' + ' tee ' + os.path.join(dir, 'tmp', 'result.log')
    print(cmd_str)
    ret = os.system(cmd_str)
    if ret:
        print("run successfully!!!")
        print("------------------------------------------------------------------------------")
    else:
        print("exit")
