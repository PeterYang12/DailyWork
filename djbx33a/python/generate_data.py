#!/usr/bin/env python3
import random
import string

# 生成一个长度为1到7的随机字符串
def generate_random_string():
    length = random.randint(1, 7)
    return ''.join(random.choices(string.ascii_letters, k=length))

# 创建包含10000个随机字符串的列表
random_strings_list = [generate_random_string() for _ in range(10000000)]

# 将随机字符串写入文本文件
file_path = 'random_strings.txt'
with open(file_path, 'w') as file:
    for random_string in random_strings_list:
        file.write(random_string + '\n')

print(f"随机字符串已保存到 {file_path}")
