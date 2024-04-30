from random import seed, randint
from shutil import rmtree
from pathlib import Path

def gen_random_array(len, generator):
    array = []
    for i in range(len):
        array.append(generator())
    return array

def gen_random_integer_array_tests(tests_data_folder):
    def get_random_integer():
        return randint(-10**9, 10**9)
    # 20 массивов из случайных чисел с длинами 2^n
    tests_data_folder.mkdir(parents=True, exist_ok=True)

    maxn = 20
    for pw in range(1, maxn + 1):
        array = gen_random_array(2**pw, get_random_integer)
        with open(f"{tests_data_folder}/{2**pw}.csv", "w") as w:
            for i in array:
                print(i, file = w)

def gen_random_small_integer_array_tests(tests_data_folder):
    def get_random_small_integer():
        return randint(-100, 100)
    # 20 массивов из случайных чисел маленьких чисел с длинами 2^n
    
    tests_data_folder.mkdir(parents=True, exist_ok=True)
    maxn = 20
    for pw in range(1, maxn):
        array = gen_random_array(2**pw, get_random_small_integer)
        with open(f"{tests_data_folder}/{2**pw}.csv", "w") as w:
            for i in array:
                print(i, file = w)

def gen_integer_array_tests():
    tests_data_folder = Path("tests_data/int")
    if tests_data_folder.exists():
        rmtree(tests_data_folder)
    gen_random_integer_array_tests(tests_data_folder / "random_big")
    gen_random_small_integer_array_tests(tests_data_folder / "random_small")

if __name__ == "__main__":
    seed(179)
    gen_integer_array_tests()