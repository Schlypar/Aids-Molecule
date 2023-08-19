import smart_pointers as ptr

arr = ptr.generate_raw(5)
ptr.print_raw(arr, 5)

arr2 = ptr.generate_myimpl(5)
ptr.print_myimpl(arr2, 5)
