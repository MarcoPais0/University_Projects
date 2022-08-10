import random
import sys
import numpy as np
import copy
import time

puzzle = []
maxValue = 999


def random_puzzle(x, y):
    str = "1\n"
    str += (x*y).__str__() + " " + (x).__str__() + " " + (y).__str__() + "\n"

    for _ in range(x*y):
        n1 = random.randint(1, maxValue).__str__()
        n2 = random.randint(1, maxValue).__str__()
        n3 = random.randint(1, maxValue).__str__()
        n4 = random.randint(1, maxValue).__str__()
    
        str += n1 + " " + n2 + " " + n3 + " " + n4 + "\n"

    return str


def generate_random_piece(array = [0, 0, 0, 0]):
    piece = copy.deepcopy(array)

    for i in range(len(piece)):
        if (piece[i] == 0):
            piece[i] = random.randint(1, maxValue)
    return piece


def repeated_piece(array):
    for i in range(len(puzzle)):
        for j in range(len(puzzle[0])):
            if ((puzzle[i][j] == [0, 0, 0, 0]).all()):
                return False
            comp = array == puzzle[i][j]
            if (comp.all()):
                return True
    return False


def generate_pos_puzzle(lines, columns):
    global puzzle

    for line in range(lines):
        for column in range(columns):
            if (line == 0 and column == 0):
                piece = generate_random_piece()
            elif (line == 0 and column > 0):
                piece = generate_random_piece([puzzle[line][column - 1][1], 0, 0, puzzle[line][column - 1][2]])

            elif (column == 0 and line > 0):
                piece = generate_random_piece([puzzle[line - 1][column][3], puzzle[line - 1][column][2], 0, 0])
            else:
                piece = generate_random_piece([puzzle[line - 1][column][3], puzzle[line - 1][column][2], 0, puzzle[line][column - 1][2]])
            
            if (repeated_piece(piece)):
                print("Piece equal")
                column -= 1

            
            puzzle[line][column] = piece


def rotate_random(array):
    piece = copy.deepcopy(array)
    for _ in range(random.randint(0, 3)):
        piece[0], piece[1], piece[2], piece[3] = piece[3], piece[0], piece[1], piece[2]

    return piece


def convert_puzzle_to_txt(lines, columns):
    str = "1\n" + (lines * columns).__str__() + " " + lines.__str__() + " " + columns.__str__() + "\n"
    array_temp = []
    for i in range(lines):
        for j in range(columns):
            temp = puzzle[i][j]
            if (i == 0 and j == 0):
                piece_rot = rotate_random(temp)
                str += piece_rot[0].__str__() + " " + piece_rot[1].__str__() + " " + piece_rot[2].__str__() + " " + piece_rot[3].__str__() + "\n"
            else:
                piece_rot = rotate_random(temp)
                str_line = piece_rot[0].__str__() + " " + piece_rot[1].__str__() + " " + piece_rot[2].__str__() + " " + piece_rot[3].__str__() + "\n"
                array_temp.append(str_line)
    
    #random.shuffle(array_temp)
    for i in range(len(array_temp)):
        str += array_temp[i]

    return str


def init_puzzle(lines, columns):
    global puzzle
    puzzle = np.zeros((lines, columns, 4), dtype = int)




def main():
    f = open("p10", "w")

    global maxValue
    maxValue = 999
    lines = 50
    columns = 50
    isPossible = True
    init_time = time.time()

    if (isPossible == False):
        str = random_puzzle(lines, columns)
    
    else:
        init_puzzle(lines, columns)
        generate_pos_puzzle(lines, columns)
        str = convert_puzzle_to_txt(lines, columns)
    

    f.write(str)

    print("Time: ", time.time() - init_time)

    f.close()



if __name__ == '__main__':
    main()