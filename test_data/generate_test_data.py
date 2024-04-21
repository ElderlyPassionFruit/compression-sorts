from random import seed, randint
from os import mkdir

def gen_random_array(len, generator):
    array = []
    for i in range(len):
        array.append(generator())
    return array

def gen_random_integer_array_tests():
    def get_random_integer():
        return randint(-10**9, 10**9)
    # 20 массивов из случайных чисел с длинами 2^n
    maxn = 20
    for pw in range(1, maxn + 1):
        array = gen_random_array(2**pw, get_random_integer)
        with open(f"test_data/int/test_random_integer_array_{2**pw}.txt", "w") as w:
            for i in array:
                print(i, file = w)

def gen_random_small_integer_array_tests():
    def get_random_small_integer():
        return randint(-100, 100)
    # 20 массивов из случайных чисел маленьких чисел с длинами 2^n
    maxn = 20
    for pw in range(1, maxn):
        array = gen_random_array(2**pw, get_random_small_integer)
        with open(f"test_data/int/test_random_small_integer_array_{2**pw}.txt", "w") as w:
            for i in array:
                print(i, file = w)

if __name__ == "__main__":
    seed(179)
    mkdir("test_data/int")
    gen_random_integer_array_tests()
    gen_random_small_integer_array_tests()