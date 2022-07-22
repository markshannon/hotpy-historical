if __name__ == '__main__':
    from parse_grammar import read_rules
    import sys
    rules = read_rules(open(sys.argv[1]))
    for r in rules.values():
        print r.name, r.optional()
        print r.la1()
        print
