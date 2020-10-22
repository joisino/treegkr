from treegkr import treegkr

def main():
    n = int(input())

    vs = []
    us = []
    cost = []
    for i in range(n-1):
        l = input().split()
        vs.append(int(l[0]) - 1)
        us.append(int(l[1]) - 1)
        cost.append(int(l[2]))

    creation = []
    destruction = []
    for i in range(n):
        c, d = map(float, input().split())
        creation.append(c)
        destruction.append(d)

    x = []
    y = []
    for i in range(n):
        a, b = map(float, input().split())
        x.append(a)
        y.append(b)

    print(treegkr(vs, us, cost, creation, destruction, x, y))
        
if __name__ == '__main__':
    main()


