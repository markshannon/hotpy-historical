def next_num(num):
    if num & 1:
        return 3 * num + 1
    else:
        return num // 2

#MAX_NUM = 1000000
MAX_NUM = 1000000

lengths = {1: 0}

def series_length(num):
    global lengths
    if num in lengths:
        return lengths[num]
    else:
        num2 = next_num(num)
        result = 1 + series_length(num2)
        lengths[num] = result
        return result

def main():
    num_with_max_length = 1
    max_length = 0
    for ii in range(1, MAX_NUM):
        length = series_length(ii)
        if length > max_length:
            max_length = length
            num_with_max_length = ii
    print(num_with_max_length, max_length)

if __name__ == "__main__":
    main()

