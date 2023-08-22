import smart_pointers as ptr
import matplotlib.pyplot as plt
import numpy as np
import time
from random import randint


def set_lables(ax, n):
    ax.set_ylabel("Time")
    ax.set_xlabel("Pointer Type")
    ax.set_title(f"Quantity was {n}")
    ax.legend()

    return ax


def plot_bars(raw: tuple, smart: tuple) -> None:
    N = ("10", "10**2", "10**3", "10**4", "10**5", "10**6")
    pointer_type = {
        "Raw": raw,
        "Smart Pointers": smart,
    }

    width = 0.25  # the width of the bars

    fig, ((ax0, ax1, ax2), (ax3, ax4, ax5)) = plt.subplots(
        nrows=2, ncols=3, layout="constrained"
    )

    axs: list = [ax0, ax1, ax2, ax3, ax4, ax5]
    meas: list = []

    for _, measurment in pointer_type.items():
        meas.append(measurment)

    raw: list = []
    for meassure in meas[0]:
        raw.append(meassure)

    smart: list = []
    for meassure in meas[1]:
        smart.append(meassure)

    raw_vs_smart: list = []
    for r, s in zip(raw, smart):
        raw_vs_smart.append((r, s))

    rad = 1
    for ax, raw_meassure, smart_meassure in zip(axs, raw, smart):
        ax.bar(
            rad, raw_meassure, color="blue", width=width, edgecolor="black", label="Raw"
        )
        ax.bar(
            rad + width,
            smart_meassure,
            color="orange",
            width=width,
            edgecolor="black",
            label="Smart",
        )

    for ax, n in zip(axs, N):
        ax = set_lables(ax, n)

    plt.show()


def generate_seed(size: int) -> list[int]:
    result: list = []

    for i in range(size):
        result.append(randint(0, 10 * size))

    return result


def measure_raw(seeds: list) -> tuple:
    N: list = [10, 10**2, 10**3, 10**4, 10**5, 10**6]

    results: list = []

    for n in N:
        current_record = 0

        for seed in seeds:
            array = ptr.generate_raw(n, seed)
            start = time.time()
            ptr.sort_raw(array, n)
            end = time.time()

            current_record += end - start

        results.append(current_record / len(seeds))

    return (results[0], results[1], results[2], results[3], results[4], results[5])


def measure_smart(seeds: list) -> tuple:
    N: list = [10, 10**2, 10**3, 10**4, 10**5, 10**6]

    results: list = []

    for n in N:
        current_record = 0

        for seed in seeds:
            array = ptr.generate_myimpl(n, seed)
            start = time.time()
            ptr.sort_myimpl(array, n)
            end = time.time()

            current_record += end - start

        results.append(current_record / len(seeds))

    return (results[0], results[1], results[2], results[3], results[4], results[5])


def save_record_raw(records: tuple) -> str:
    serialized_record: str = ""

    for record in records:
        serialized_record = serialized_record + str(record) + " "

    file_name: str = "raw_pointers.txt"
    file = open(file_name, "w")

    file.seek(0, 0)
    file.write(serialized_record)

    file.close()

    return file_name


def save_record_smart(records: tuple) -> str:
    serialized_record: str = ""

    for record in records:
        serialized_record = serialized_record + str(record) + " "

    file_name: str = "smart_pointers.txt"
    file = open(file_name, "w")

    file.seek(0, 0)
    file.write(serialized_record)

    file.close()

    return file_name


def deserialise_record(file) -> tuple:
    return tuple(map(float, list(file.readline().split(" "))[:-1]))


# seeds = generate_seed(250)
#
# save_record_raw(measure_raw(seeds))
# save_record_smart(measure_smart(seeds))
#
# file_with_raw = open("raw_pointers.txt", "r+")
# file_with_smart = open("smart_pointers.txt", "r+")
#
# plot_bars(deserialise_record(file_with_raw), deserialise_record(file_with_smart))
#
# file_with_raw.close()
# file_with_smart.close()
