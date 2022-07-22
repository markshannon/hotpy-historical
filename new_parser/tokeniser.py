
import re
import copy

class Tokeniser:

    def __init__(self, f):
        self.f = f
        self.reader = iter(f)
        self.current_line = self.next_line()
        self.brackets = 0

    def __iter__(self):
        return self

    def next_line(self):
        line = self.reader.next().lstrip()
        while (not line) or line[0] == '#':
            line = self.reader.next().lstrip()
        return line

    def next(self):
        #Return text token.
        self.current_line = self.current_line.lstrip()
        if not self.current_line:
            self.current_line = self.next_line()
            if not self.brackets:
                return '\n'
        c = self.current_line[0]
        if c == '#':
            self.current_line = self.next_line()
            if self.brackets:
                c = self.current_line[0]
            else:
                return '\n'
        if c in '([':
            self.current_line = self.current_line[1:]
            self.brackets += 1
            return c
        if c in ')]':
            self.current_line = self.current_line[1:]
            self.brackets -= 1
            return c
        if c in '|+*?:\n':
            self.current_line = self.current_line[1:]
            return c
        if c == '"':
            end = self.current_line.find('"', 1) +1
            result = self.current_line[:end]
            self.current_line = self.current_line[end:]
            return result
        if c == "'":
            end = self.current_line.find("'", 1) +1
            result = self.current_line[:end]
            self.current_line = self.current_line[end:]
            return result
        end = 1
        while end < len(self.current_line) and self.current_line[end] in "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz_1234567890":
            end += 1
        result = self.current_line[:end]
        self.current_line = self.current_line[end:]
        return result

