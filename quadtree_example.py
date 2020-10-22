from treegkr import treegkr
from quadtree import quadtree

def main():
    xp = [(0.2, 0.2, 0.3), (0.4, 0.1, 0.2)]
    yp = [(0.2, 0.3, 0.3), (0.9, 0.3, 0.9), (0.2, 0.4, 0.4)]
    ps = xp + yp
    dimensions = 3
    bbox = [[0, 0, 0], [1, 1, 1]] # You can randomly translate bbox
    depth = 8
    vs = []
    us = []
    cost = []
    coordinate_map = {}
    ind = [0]
    quadtree(ps, dimensions, bbox, depth, vs, us, cost, coordinate_map, ind)
    xs = [0.0 for i in range(ind[0])]
    for x, y, z in xp:
        xs[coordinate_map[(x, y, z)]] += 1
    ys = [0.0 for i in range(ind[0])]
    for x, y, z in yp:
        ys[coordinate_map[(x, y, z)]] += 1
    creation = [0.2] * ind[0]
    deletion = [0.3] * ind[0]
    print(treegkr(vs, us, cost, creation, deletion, xs, ys))

if __name__ == '__main__':
    main()
