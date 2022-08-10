from sys import stdin,stdout


def readln():
 return stdin.readline().rstrip()


def outln(n):
 stdout.write(str(n))
 stdout.write("\n")


def chess(x, y, j):
    v = 0
    if board[x + 200][y + 200] == 0: # adiciona-se metade do tamanho da board porque só existem índices positivos no array board
        board[x + 200][y + 200] = 1
        v = 1
    if j > 0:
        v += chess(x + 2, y + 1, j - 1) + chess(x + 2, y - 1, j - 1) + chess(x - 2, y + 1, j - 1) + chess(x - 2, y - 1, j - 1) + chess(x + 1, y + 2, j - 1) + chess(x - 1, y + 2, j - 1) + chess(x + 1, y - 2, j - 1) + chess(x - 1, y - 2, j - 1)

    return v


if __name__ == "__main__":
    n = readln().split()
    n = int(n[0])

    count = 0
    board = []
    for i in range(n):
        for j in range(401):
            aux = []
            for t in range(401):
                aux.append(0)
            board.append(aux)

        x, y , j = [int(i) for i in readln().split()]
        count += chess(x, y, j)

    outln(count)