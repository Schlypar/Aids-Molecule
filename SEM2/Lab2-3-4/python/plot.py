from logic import plot_bars, deserialise_record

file_with_raw = open("raw_pointers.txt", "r+")
file_with_smart = open("smart_pointers.txt", "r+")

plot_bars(deserialise_record(file_with_raw), deserialise_record(file_with_smart))

file_with_raw.close()
file_with_smart.close()
