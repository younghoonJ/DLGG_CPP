import random
import itertools

MAX63 = 0x7FFF_FFFF_FFFF_FFFF
if __name__ == "__main__":
    mx_range = 20
    lst = set()

    while len(lst) < mx_range * mx_range * 2 + 1:
        lst.add(random.randint(0, MAX63))

    with open("zob.txt", 'w') as w:
        w.write("constexpr std::array<uin64_t, 20 * 20 * 2 + 1> zob_ = {\n")
        for x in lst:
            w.write(hex(x) + ',\n')
        w.write("};")
