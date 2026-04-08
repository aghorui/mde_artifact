from argparse import ArgumentParser

if __name__ == '__main__':
    parser = ArgumentParser()
    parser.add_argument("--NAIVE_MODE", help="Enable Naive Implementation", action="store_true")
    parser.add_argument("--ZDD_MODE", help="Enable ZDD Implementation", action="store_true")
    parser.add_argument("--SPARSEBV_MODE", help="Enable Sparse Bit Vector-based Implementation", action="store_true")
    parser.add_argument("--SHOW_OUTPUT", help="Show Analysis Output", action="store_true")
    parser.add_argument("--PRINT", help="Print all", action="store_true")
    parser.add_argument("--FUNPTR", help="Handle function pointers", action="store_true")
    parser.add_argument("--PRINTSTATS", help="Print statistics after analysis", action="store_true")
    parser.add_argument("--SIMPLE_PRINT", help="Prints required data", action="store_true")
    parser.add_argument("--SLIMIR", help="Prints SLIM instructions", action="store_true")
    res = parser.parse_args()

    flag_set = []

    if res.PRINT:
        flag_set.append("PRINT")

    if res.FUNPTR:
        flag_set.append("FUNPTR")

    if res.PRINTSTATS:
        flag_set.append("PRINTSTATS")

    if res.SIMPLE_PRINT:
        flag_set.append("SIMPLE_PRINT")

    if res.SHOW_OUTPUT:
        flag_set.append("SHOW_OUTPUT")

    result = ";".join([ f"-D{flag}" for flag in flag_set ])

    print(result)
